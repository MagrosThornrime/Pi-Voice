#pragma once

#include <exception>
#include <string>
#include <format>

class AssertionFailed : std::exception {
public:

    inline AssertionFailed(std::string what): _what{std::move(what)}{}
    inline const char* what() const noexcept override { return _what.c_str(); }

private:

    std::string _what;
};

#define ASSERT(what) if(not(what)) { throw AssertionFailed(std::format("Assertion failed ({}) {}@{}", #what, __FILE__, __LINE__)); }
#define ASSERT_FMT(what, ...) if(not(what)) { throw AssertionFailed(std::format("Assertion failed ({}) {}@{}: {}", #what,, __FILE__, __LINE__, std::format(__VA_ARGS__))); }