include_guard()

include("cmake/CPM.cmake")

get_property(orig_pic GLOBAL PROPERTY POSITION_INDEPENDENT_CODE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "Enable PIC for fmt" FORCE)
CPMAddPackage("gh:fmtlib/fmt#11.2.0")
if(orig_pic)
  set_property(GLOBAL PROPERTY POSITION_INDEPENDENT_CODE ${orig_pic})
else()
  unset(CMAKE_POSITION_INDEPENDENT_CODE CACHE)
endif()

if(fmt_ADDED)
    find_package(fmt)
else()
    message(ERROR "fmt not added")
endif()