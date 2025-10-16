#include <drillheader.h>

#include <gtest/gtest.h>


class SortingTest : public testing::Test {
    // If unsorted, sort them first
    protected:
        std::random_device rd;
        constexpr static size_t N = 100;
        std::vector<int> arr;
        std::vector<int> expected;
    public:
        void SetUp() {
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(-100, 100);

            arr.resize(N);

            // std::iota(ALL(arr), 0);
            // std::shuffle(ALL(arr), gen);
            std::generate_n(arr.begin(), N, [&](){ return dist(gen); });

            expected.assign(ALL(arr));
            std::sort(ALL(expected));
        };
};

TEST_F(SortingTest, InsertionSort) {
    FOR(i, 1, SZ(arr)) {
        int k = arr[i];
        int j = i - 1;
        while (j >= 0 && k < arr[j]) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = k;
    }

    EXPECT_EQ(arr, expected);
}