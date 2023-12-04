#include "kernel/server.h"
#include "common/time.hpp"
#include "common/common.hpp"

using namespace amx;

static w_ptr<server>  wk_server;

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
		/*svr->stop(dwCtrlType);*/
		return TRUE;
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_LOGOFF_EVENT://atmost 10 second,will force closed by system
		/*svr->stop(dwCtrlType);
		while (svr->get_state() != state::stopped)
		{
			std::this_thread::yield();
		}*/
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

	uint32_t thread_count = std::thread::hardware_concurrency();
	
	s_ptr<server> rt_server = make_shared<server>();

	rt_server->init(argc, argv, thread_count);
	rt_server->run();

	return 0;
}