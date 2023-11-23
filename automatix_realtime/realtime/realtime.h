#pragma once

#include "common/common.hpp"

#define AMX_DEBUG 0
#if AMX_DEBUG
#include "test/automatix_test.h"
#endif

namespace amx {

class realtime {
	friend class automatix;
public:
	explicit realtime() {}
	virtual ~realtime() {}

private:
	void launch();
	void init(const scoped_actor& bridge_actor);
	void tick(const scoped_actor& bridge_actor);
	void exit(const scoped_actor& bridge_actor);

#if AMX_DEBUG
	test::automatix_test _test;
#endif

};

}