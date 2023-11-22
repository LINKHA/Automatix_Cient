#pragma once

#define AMX_DEBUG 1
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
	void init();
	void tick();
	void exit();

#if AMX_DEBUG
	test::automatix_test _test;
#endif

};

}