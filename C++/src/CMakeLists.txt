include(../cmake/init.cmake)

set ( SOURCES main.cpp DLA.cpp)


 # -------  Set up the static dmc library
add_library(trees_lib STATIC ${SOURCES})
include_qmc_external_directories( trees_lib )
target_include_directories( trees_lib PUBLIC
                           "${PROJECT_SOURCE_DIR}/dmc"
                           )
#set_target_properties(  pimc_lib PROPERTIES COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} )
target_link_libraries(trees_lib PRIVATE gtest)

add_executable(trees main.cpp)
target_link_libraries(trees PUBLIC stdc++fs  trees_lib)


#set_target_properties( pimc PROPERTIES COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} )
#set_target_properties( pimc PROPERTIES LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS_ADDITIONAL} )
link_qmc_external_libraries(trees)
include_qmc_external_directories(trees)

