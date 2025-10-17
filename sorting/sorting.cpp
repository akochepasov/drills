#include <drillheader.h>

#include <gtest/gtest.h>


void InsertionSort(std::vector<int>& arr) {
    FOR(i, 1, SZ(arr)) {
        int k = arr[i];
        int j = i - 1;
        while (j >= 0 && k < arr[j]) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = k;
    }
}

void BubbleSort(std::vector<int>& arr) {
    bool swapped;
    FOR(i, 0, SZ(arr)-1) {
        swapped = false;
        FOR(j, 0, SZ(arr)-i-1) {
            if (arr[j] > arr[j+1]) {
                std::swap(arr[j], arr[j+1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

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
    InsertionSort(arr);

    EXPECT_EQ(arr, expected);
}

TEST_F(SortingTest, BubbleSort) {
    BubbleSort(arr);

    EXPECT_EQ(arr, expected);
}