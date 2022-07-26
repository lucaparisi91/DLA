#include "gtest/gtest.h"

int main(int argc, char **argv) {

  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();  

  ::testing::InitGoogleTest(&argc, argv);
  
  
  int ret= RUN_ALL_TESTS();


  return ret;

}
