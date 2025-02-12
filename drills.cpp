#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <iostream>
#include <random>

#include <gtest/gtest.h>


TEST(Basic, Sum) {
    int32_t a = std::rand(), b = std::rand();
    int32_t c = 0;
    for (int j = 0; j < 10; j++) {
      c += a + b;
    }

    EXPECT_EQ(c, 10 * (a + b));
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
