include(../cmake/init.cmake)

set(SOURCES tst-DLA.cpp tst-randomGenerator.cpp tst-lattice.cpp
)


add_googletest()
add_executable(tst main.cpp ${SOURCES})
target_link_libraries(tst PUBLIC dla_lib)
target_link_libraries(tst PRIVATE gtest)
link_qmc_external_libraries(tst)
include_qmc_external_directories(tst)