#pragma once

#include <exception>
#include <string>
#include <fmt/core.h>

/// @brief Exception thrown if assertion fails
class AssertionFailed : std::exception {
public:

    /// @brief Message constructor
    /// @param what message to pass
    inline AssertionFailed(std::string what): _what{std::move(what)}{}
    /// @brief Returns C-string of message
    inline const char* what() const noexcept override { return _what.c_str(); }

private:

    std::string _what;
};

#define ASSERT(what) if(not(what)) { throw AssertionFailed(fmt::format("Assertion failed ({}) {}@{}", #what, __FILE__, __LINE__)); }
#define ASSERT_FMT(what, ...) if(not(what)) { throw AssertionFailed(fmt::format("Assertion failed ({}) {}@{}: {}", #what, __FILE__, __LINE__, fmt::format(__VA_ARGS__))); }