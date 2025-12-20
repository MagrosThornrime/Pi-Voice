#pragma once

#include <Types.hpp>

namespace filters {

struct FilterParams {
	enum Value: u32 {
		cutoff,
		quality,
		gainDB,
		order,
		_count,
	};

	template<Value P>
	struct _Type {
		using type = float;
	};

	template<Value P>
	using type = _Type<P>::type;
};

#define PARAM_TYPE(param, T) template<> struct FilterParams::_Type<param> { using type = T; }

PARAM_TYPE(FilterParams::order, u32);

#undef PARAM_TYPE

}