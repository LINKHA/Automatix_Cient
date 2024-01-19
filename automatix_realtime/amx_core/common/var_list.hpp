#pragma once
#include <initializer_list>
#include <any>
#include <vector>
#include <iostream>

namespace amx {

class var_list {
public:
	var_list(std::initializer_list<std::any> args) : values_(args) {}

	template <typename _Ty>
	_Ty get(size_t index) const {
		if (index < values_.size()) {
			try {
				return std::any_cast<_Ty>(values_[index]);
			}
			catch (const std::bad_any_cast& e) {
				std::cerr << "Error: " << e.what() << std::endl;
			}
		}
		return _Ty{};
	}

private:
	std::vector<std::any> values_;
};
// example
//int main() {
//	var_list varList = { 1, "hello", 3.14, 'c' };
//
//	// 获取指定索引处的值
//	int intValue = varList.get<int>(0);
//	std::string stringValue = varList.get<std::string>(1);
//	double doubleValue = varList.get<double>(2);
//	char charValue = varList.get<char>(3);
//
//	// 输出获取到的值
//	std::cout << "Int Value: " << intValue << std::endl;
//	std::cout << "String Value: " << stringValue << std::endl;
//	std::cout << "Double Value: " << doubleValue << std::endl;
//	std::cout << "Char Value: " << charValue << std::endl;
//
//	return 0;
//}

}