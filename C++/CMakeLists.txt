cmake_minimum_required(VERSION 3.10)


# set the project name
project( trees VERSION 3.0)



set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Compilation mode")
set_property(CACHE CMAKE_BUILD_TYPE  PROPERTY STRINGS Release Debug)
mark_as_advanced( CMAKE_INSTALL_PREFIX)

set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_RELEASE "")



enable_testing()
add_subdirectory(external/pybind11)

add_subdirectory(src)


include(GoogleTest)
add_subdirectory(tst)

