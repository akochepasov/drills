#include <gtest/gtest.h>

int cas_algorithm(int *a, int expected, int desired) {

    int old_a = *a;
    if (old_a == expected) {
        *a = desired;
     }
    return old_a;
}


TEST(SystemTest, Empty) {
}
