include(../cmake/init.cmake)

set ( SOURCES DLA.cpp lattice/lattice.cpp lattice/particles.cpp moves.cpp tools.cpp)


 # -------  Set up the static dmc library
add_library(dla_lib STATIC ${SOURCES})
include_qmc_external_directories( dla_lib )
target_include_directories( dla_lib PUBLIC
                           "${PROJECT_SOURCE_DIR}/dmc"
                           )
#set_target_properties(  pimc_lib PROPERTIES COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} )
target_link_libraries( dla_lib PRIVATE gtest)

pybind11_add_module( py_dla py_tools.cpp py_dla.cpp )
pybind11_add_module( dla_cpp py_tools.cpp py_dlaMesh.cpp )



target_link_libraries(py_dla PUBLIC dla_lib)
target_link_libraries(dla_cpp PUBLIC dla_lib)


install( TARGETS py_dla dla_cpp EXPORT dla 
LIBRARY DESTINATION lib 
   )

install(   FILES "${CMAKE_SOURCE_DIR}/../python/dla.py"     DESTINATION python )