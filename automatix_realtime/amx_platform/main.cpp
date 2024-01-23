#include <amx_core/common/time.hpp>
#include <amx_core/net/socket_server.h>
#include <amx_core/kernel/server.h>

namespace amx {
	class socket_client
	{
	public:
		socket_client(std::shared_ptr<server> server) {
			server_ = server;
		}


	protected:

	private:
		std::shared_ptr<server> server_;
	};
}
