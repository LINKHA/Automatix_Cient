#pragma once
#include "common/common.hpp"

namespace amx {
namespace msg_type {

//Data Model
//The allowed type
//**********************************************************
//	signed and unsigned int of 8, 16, 32, and 64 bits
//	float, double, long double
//	byte, bool, string
//  std::vector
//  std::tuple¡¢std::array
//  std::map
//  custom struct
//**********************************************************

// ------------------------
struct foo {
	std::vector<int> a;
	int b;
};
template <typename _Ty>
bool inspect(_Ty& f, foo& x) {
	return f.object(x).fields(f.field("a", x.a), f.field("b", x.b));
}
// ------------------------
//supervisor
using add_monitor = string;
using add_monitors = vector<string>;
using add_monitor_by_id = int32_t;
using add_monitor_by_ids = vector<int32_t>;
// ------------------------
using worker_handle = string;
// ------------------------

}
}


CAF_BEGIN_TYPE_ID_BLOCK(amx_msg_type, first_custom_type_id)

	CAF_ADD_TYPE_ID(amx_msg_type, (amx::msg_type::foo))
	CAF_ADD_TYPE_ID(amx_msg_type, (std::vector<std::string>))
	CAF_ADD_TYPE_ID(amx_msg_type, (std::vector<std::int32_t>))

CAF_END_TYPE_ID_BLOCK(amx_msg_type)