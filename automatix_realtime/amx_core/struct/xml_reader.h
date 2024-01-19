#pragma once
#include <iguana/util.hpp>
#include <iguana/xml_reader.hpp>

namespace amx {

	template <typename T, typename View>
	inline void from_xml(T&& t, const View& str) {
		iguana::from_xml(std::forward<T>(t), str);
	}

	template <typename Num>
	inline Num get_number(std::string_view str) {
		return iguana::get_number<Num>(str);
	}

	template <typename T, typename map_type = std::unordered_map<std::string_view,
		std::string_view>>
		using xml_attr_t = iguana::xml_attr_t<T, map_type>;

	template <typename T>
	inline constexpr std::string_view type_string() {
		return iguana::type_string<T>();
	}

	template <auto T>
	inline constexpr std::string_view enum_string() {
		return iguana::enum_string<T>();
	}
} 