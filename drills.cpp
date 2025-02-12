#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <iostream>
#include <random>

#include <gtest/gtest.h>


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
