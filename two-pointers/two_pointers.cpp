#include <drillheader.h>

#include <gtest/gtest.h>


class SortedIntersectionTest : public testing::Test {
    // If unsorted, sort them first
    protected:
        std::vector<int> a = {-100, -20, 3, 40, 500};
        std::vector<int> b = {-50, -20, -3, 40, 57, 800};
        std::vector<int> expected = {-20, 40};
};

TEST_F(SortedIntersectionTest, STL) {
    std::vector<int> result;
    std::unordered_set<int> a_set(a.begin(), a.end());
    std::unordered_set<int> b_set(b.begin(), b.end());

    for (auto e : a) {
        if (EXIST(b_set, e)) {
            result.PB(e);
        }
    }

    EXPECT_EQ(result, expected);
}

TEST_F(SortedIntersectionTest, Raw) {
    std::vector<int> result;

    size_t i = 0, j = 0;
    while (i < SZ(a) && j < SZ(b)) {
        if (a[i] < b[j]) {
            i++;
        } else if (a[i] > b[j]) {
            j++;
        } else if (a[i] == b[j]) {
            result.PB(a[i]);
            i++; j++;
        }
    }

    EXPECT_EQ(result, expected);
}