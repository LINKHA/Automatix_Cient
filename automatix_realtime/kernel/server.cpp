#include "server.h"

namespace amx {

	void print_logo()
	{
		std::cout << "***********************************	\n";
		std::cout << "*        automatix-realtime       *	\n";
		std::cout << "*---------------------------------*	\n";
		std::cout << "*   version: 0.0.1                *   \n";
		std::cout << "***********************************	\n";
	}

	server::~server() {

	}

	int server::init(int argc, char** argv, uint32_t worker_num)
	{
		print_logo();
		// create id 0 context
		std::shared_ptr<context> background = context_manager_.create_context();
		background->with_value("argc", argc);
		background->with_value("argv", argv);

		worker_num = (worker_num == 0) ? 1 : worker_num;

	#if AMX_DEBUG
		test_.init();
	#endif
		return 0;
	}

	int server::run()
	{
		for (;;) {
			if (exitflag_ < 0)
			{
				break;
			}

			if (1) {

			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		return exitflag_;
	}

	void server::exit(int exitflag)
	{
		exitflag_ = exitflag;
	}


}