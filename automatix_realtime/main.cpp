#include "kernel/server.h"
#include "common/time.hpp"
#include "common/common.hpp"
#include "common/lua_utility.hpp"
#include "common/file.hpp"

using namespace amx;

static std::weak_ptr<server>  wk_server;

enum class state
{
	unknown,
	init,
	ready,
	stopping,
	stopped
};

#if TARGET_PLATFORM == PLATFORM_WINDOWS
static BOOL WINAPI ConsoleHandlerRoutine(DWORD dwCtrlType)
{
	auto svr = wk_server.lock();
	if (nullptr == svr)
	{
		return TRUE;
	}

	switch (dwCtrlType)
	{
	case CTRL_C_EVENT:
		svr->stop(dwCtrlType);
		return TRUE;
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_LOGOFF_EVENT://atmost 10 second,will force closed by system
		svr->stop(dwCtrlType);
		while (svr->get_state() != amx::state::stopped)
		{
			std::this_thread::yield();
		}
		return TRUE;
	default:
		break;
	}
	return FALSE;
}
#else
static void signal_handler(int signal)
{
	auto svr = wk_server.lock();
	if (nullptr == svr)
	{
		return;
	}

	std::string_view msg;
	switch (signal)
	{
	case SIGTERM:
		msg = "Received SIGTERM,shutdown...\n";
		break;
	case SIGINT:
		msg = "Received SIGINT,shutdown...\n";
		break;
	default:
		msg = "Received shutdown signal,shutdown...\n";
		break;
	}
	svr->stop(signal);
	[[maybe_unused]] auto n = write(STDERR_FILENO, msg.data(), msg.size());
}
#endif

static void register_signal(int argc, char* argv[])
{
#if TARGET_PLATFORM == PLATFORM_WINDOWS
	SetConsoleCtrlHandler(ConsoleHandlerRoutine, TRUE);
	std::string str;
	for (int i = 0; i < argc; ++i)
	{
		str.append(argv[i]);
		if (i == 0)
		{
			str.append("(PID: ");
			str.append(std::to_string(GetCurrentProcessId()));
			str.append(")");
		}
		str.append(" ");
	}
	SetConsoleTitle(str.data());
#else
	std::signal(SIGHUP, SIG_IGN);
	std::signal(SIGQUIT, SIG_IGN);
	std::signal(SIGPIPE, SIG_IGN);
	std::signal(SIGINT, signal_handler);
	std::signal(SIGTERM, signal_handler);
#endif
}

int main(int argc, char** argv) {
	time::timezone();
	register_signal(argc, argv);

#ifdef LUA_CACHELIB
	luaL_initcodecache();
#endif

	uint32_t thread_count = std::thread::hardware_concurrency();
	std::shared_ptr<server> rt_server = std::make_shared<server>();

	bool enable_stdout = true;
	std::string logfile;
	std::string bootstrap = "../example/main_game.lua";
	std::string loglevel;

	int argn = 1;
	if (argc <= argn)
	{
		return -1;
	}
	bootstrap = argv[argn++];

	if (fs::path(bootstrap).extension() != ".lua")
	{
		return -1;
	}

	std::string arg = "return {";
	for (int i = argn; i < argc; ++i)
	{
		arg.append("'");
		arg.append(argv[i]);
		arg.append("',");
	}
	arg.append("}");

	if (file::read_all(bootstrap, std::ios::in).substr(0, 11) == "---__init__") {
		std::unique_ptr<lua_State, state_deleter> lua{ luaL_newstate() };
		lua_State* L = lua.get();
		luaL_openlibs(L);
		lua_pushboolean(L, true);
		lua_setglobal(L, "__init__");
		lua_pushcfunction(L, traceback);
		assert(lua_gettop(L) == 1);

		int r = luaL_loadfile(L, bootstrap.data());
		r = luaL_dostring(L, arg.data());
		r = lua_pcall(L, 1, 1, 1);

		thread_count = lua_opt_field<uint32_t>(L, -1, "thread", thread_count);
		logfile = lua_opt_field<std::string>(L, -1, "logfile");
		enable_stdout = lua_opt_field<bool>(L, -1, "enable_stdout", enable_stdout);
		loglevel = lua_opt_field<std::string>(L, -1, "loglevel", loglevel);
		std::string path = lua_opt_field<std::string>(L, -1, "path", "");

		if (!path.empty()) {
			path = amx::format("package.path='%s;'..package.path;", path.data());
			rt_server->set_env("PATH", path);
		}
	}
	rt_server->init(thread_count, "");
	rt_server->run();

	return 0;
}