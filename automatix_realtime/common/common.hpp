#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "caf/all.hpp"

namespace amx {
template<typename _Ty> using s_ptr = std::shared_ptr<_Ty>;
template<typename _Ty> using u_ptr = std::unique_ptr<_Ty>;
template<typename _Ty> using w_ptr = std::weak_ptr<_Ty>;
template<typename _Ty1, typename _Ty2> s_ptr<_Ty1> p_static_cast(const s_ptr<_Ty2>& other) { return std::static_pointer_cast<_Ty1>(other); }
template<typename _Ty1, typename _Ty2> s_ptr<_Ty1> p_dynamic_cast(const s_ptr<_Ty2>& other) { return std::dynamic_pointer_cast<_Ty1>(other); }
template<class _Ty, class... _Types> s_ptr<_Ty> make_shared(_Types&&... args) { return std::make_shared<_Ty>(std::forward<_Types>(args)...); }
template<class _Ty, class... _Types> u_ptr<_Ty> make_unique(_Types&&... args) { return std::make_unique<_Ty>(std::forward<_Types>(args)...); }
template<typename _Ty> using enable_ptr_this = std::enable_shared_from_this<_Ty>;

using string = std::string;
template<typename _Ty> using vector = std::vector<_Ty>;
template<typename _Ty1, typename _Ty2> using unordered_map = std::unordered_map<_Ty1, _Ty2>;
template<typename _Ty> using unordered_set = std::unordered_set<_Ty>;


using event_based_actor = caf::event_based_actor;
using blocking_actor = caf::blocking_actor;
using scoped_actor = caf::scoped_actor;
using behavior = caf::behavior;
using actor_config = caf::actor_config;
using actor_system = caf::actor_system;

}