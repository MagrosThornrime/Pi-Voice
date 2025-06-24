#pragma once

#include <functional>
#include <tuple>

namespace utils {

template<class Fn>
struct CallableTraits;

template<class R, class... Params>
struct CallableTraits<R(Params...)> {
	using Return = R;
	using Args = std::tuple<Params...>;

	static constexpr bool isCallable = true;
	static constexpr bool isFunction = true;
	static constexpr bool isMember = false;
	static constexpr bool isFunctor = false;
	static constexpr bool isNoexcept = false;
};

template<class R, class... Params>
struct CallableTraits<R(Params...) noexcept> {
	using Return = R;
	using Args = std::tuple<Params...>;

	static constexpr bool isCallable = true;
	static constexpr bool isFunction = true;
	static constexpr bool isMember = false;
	static constexpr bool isFunctor = false;
	static constexpr bool isNoexcept = true;
};

template<class R, class... Params>
struct CallableTraits<R (*)(Params...)> {
	using Return = R;
	using Args = std::tuple<Params...>;

	static constexpr bool isCallable = true;
	static constexpr bool isFunction = true;
	static constexpr bool isMember = false;
	static constexpr bool isFunctor = false;
	static constexpr bool isNoexcept = false;
};

template<class R, class... Params>
struct CallableTraits<R (*)(Params...) noexcept> {
	using Return = R;
	using Args = std::tuple<Params...>;

	static constexpr bool isCallable = true;
	static constexpr bool isFunction = true;
	static constexpr bool isMember = false;
	static constexpr bool isFunctor = false;
	static constexpr bool isNoexcept = true;
};

template<class R, class... Params>
struct CallableTraits<R (&)(Params...)> {
	using Return = R;
	using Args = std::tuple<Params...>;

	static constexpr bool isCallable = true;
	static constexpr bool isFunction = true;
	static constexpr bool isMember = false;
	static constexpr bool isFunctor = false;
	static constexpr bool isNoexcept = false;
};

template<class R, class... Params>
struct CallableTraits<R (&)(Params...) noexcept> {
	using Return = R;
	using Args = std::tuple<Params...>;

	static constexpr bool isCallable = true;
	static constexpr bool isFunction = true;
	static constexpr bool isMember = false;
	static constexpr bool isFunctor = false;
	static constexpr bool isNoexcept = true;
};

template<class R, class... Params>
struct CallableTraits<R (&&)(Params...)> {
	using Return = R;
	using Args = std::tuple<Params...>;

	static constexpr bool isCallable = true;
	static constexpr bool isFunction = true;
	static constexpr bool isMember = false;
	static constexpr bool isFunctor = false;
	static constexpr bool isNoexcept = false;
};

template<class R, class... Params>
struct CallableTraits<R (&&)(Params...) noexcept> {
	using Return = R;
	using Args = std::tuple<Params...>;

	static constexpr bool isCallable = true;
	static constexpr bool isFunction = true;
	static constexpr bool isMember = false;
	static constexpr bool isFunctor = false;
	static constexpr bool isNoexcept = true;
};

template<class T, class R, class... Params>
struct CallableTraits<R (T::*)(Params...)> {
	using Return = R;
	using Args = std::tuple<Params...>;

	static constexpr bool isCallable = true;
	static constexpr bool isFunction = false;
	static constexpr bool isMember = true;
	static constexpr bool isFunctor = false;
	static constexpr bool isNoexcept = false;
	static constexpr bool isConst = false;
};

template<class T, class R, class... Params>
struct CallableTraits<R (T::*)(Params...) noexcept> {
	using Return = R;
	using Args = std::tuple<Params...>;

	static constexpr bool isCallable = true;
	static constexpr bool isFunction = false;
	static constexpr bool isMember = true;
	static constexpr bool isFunctor = false;
	static constexpr bool isNoexcept = true;
	static constexpr bool isConst = false;
};

template<class T, class R, class... Params>
struct CallableTraits<R (T::*)(Params...) const> {
	using Return = R;
	using Args = std::tuple<Params...>;

	static constexpr bool isCallable = true;
	static constexpr bool isFunction = false;
	static constexpr bool isMember = true;
	static constexpr bool isFunctor = false;
	static constexpr bool isNoexcept = false;
	static constexpr bool isConst = true;
};

template<class T, class R, class... Params>
struct CallableTraits<R (T::*)(Params...) const noexcept> {
	using Return = R;
	using Args = std::tuple<Params...>;

	static constexpr bool isCallable = true;
	static constexpr bool isFunction = false;
	static constexpr bool isMember = true;
	static constexpr bool isFunctor = false;
	static constexpr bool isNoexcept = true;
	static constexpr bool isConst = true;
};

namespace _details {

template<class Fn>
concept HasConcreteCallOperator = requires { &Fn::operator(); };

struct CallableTraitsBase {  
	static constexpr bool isCallable = false;
};

} // namespace _details

template<class T>
requires(_details::HasConcreteCallOperator<std::remove_reference_t<T>>)
struct CallableTraits<T> {
	using Return = CallableTraits<decltype(&T::operator())>::Return;
	using Args = CallableTraits<decltype(&T::operator())>::Args;

	static constexpr bool isCallable = true;
	static constexpr bool isFunction = false;
	static constexpr bool isMember = false;
	static constexpr bool isFunctor = true;
	static constexpr bool isNoexcept = CallableTraits<decltype(&T::operator())>::isNoexcept;
};

template<class T>
struct CallableTraits : std::conditional_t<CallableTraits<std::remove_reference_t<T>>::isCallable, CallableTraits<std::remove_reference_t<T>>, _details::CallableTraitsBase> {};

}