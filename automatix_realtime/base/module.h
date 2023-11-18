#pragma once

namespace amx {

class module {
public:
	module(const char* name);
	virtual ~module();
	virtual void init()= 0;
	virtual void load() = 0;
	virtual void unload() = 0;
private:
};

}