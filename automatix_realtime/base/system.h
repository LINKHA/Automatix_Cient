#pragma once
#include <string>

namespace amx {

class ISystem {
public:
	ISystem(const std::string& name);
	virtual ~ISystem();
	virtual void Init() = 0;
	virtual void Load() = 0;
	virtual void Unload() = 0;

public:
	std::string name{};
};

}