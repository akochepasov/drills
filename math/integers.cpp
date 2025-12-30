#include <drillheader.h>
#include <gtest/gtest.h>

class IntegerTest : public testing::Test {
protected:
    VI input = { -123, 123, 511, 120, 126324, 726324182, 1534236469, INT_MIN, INT_MAX };
    VI expected = { -321, 321, 115, 21, 423621, 281423627, 0, 0, 0 };
};

int reverse_integer_overflow(int n) {
    int sign = n < 0 ? -1 : 1;
    n = abs(n);

    int r = 0;
    while (n > 0) {
        r += n % 10;
        r *= 10;
        n /= 10;
    }

    return sign * r / 10;
}


int reverse_integer2(int n) {
    if (n == INT_MIN) return 0;

    int sign = n < 0 ? -1 : 1;
    n = abs(n);

    int r = 0;
    while (n > 0) {
        if (r > INT_MAX / 10) return 0;
        auto m = n % 10;
        r = r * 10 + m;
        n /= 10;
    }

    return sign * r;
}


TEST_F(IntegerTest, Reverse) {
    for (auto i = 0; i < input.size(); i++) {
        auto r = reverse_integer2(input[i]);
        EXPECT_EQ(r, expected[i]);
    }
}
