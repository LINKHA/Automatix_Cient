#pragma once
#include "common/common.hpp"

namespace amx {

	class worker {
		DECLARE_CLASS(worker, worker)
	public:
		explicit worker(uint32_t id);

		~worker();
	private:
		uint32_t workerid_ = 0;
	};

}