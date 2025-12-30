#include <drillheader.h>
#include <gtest/gtest.h>

using ArrayGenerator = std::function<void(std::vector<int>&)>;

// Array generators
namespace gen {
    void uniform_random(std::vector<int>& arr) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(-100, 100);
        std::generate(ALL(arr), [&](){ return dist(gen); });
    }

    void sequential(std::vector<int>& arr) {
        std::iota(arr.begin(), arr.end(), 0);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(arr.begin(), arr.end(), gen);
    }
}

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

struct ArrayParams {
    ArrayGenerator generator;
    size_t size;
    std::string name;

    ArrayParams(ArrayGenerator gen, size_t sz, std::string n) 
        : generator(gen), size(sz), name(std::move(n)) {}
};

class SortingTest : public testing::TestWithParam<ArrayParams> {
protected:
    std::vector<int> arr;
    std::vector<int> expected;

public:
    void SetUp() override {
        const auto& params = GetParam();
        arr.resize(params.size);
        params.generator(arr);
        
        expected.assign(ALL(arr));
        std::sort(ALL(expected));
    }
};

TEST_P(SortingTest, InsertionSort) {
    InsertionSort(arr);
    EXPECT_EQ(arr, expected);
}

TEST_P(SortingTest, BubbleSort) {
    BubbleSort(arr);
    EXPECT_EQ(arr, expected);
}

std::vector<ArrayParams> GenerateTestCases() {
    return {
        ArrayParams(gen::uniform_random, 100, "UniformRandom100"),
        ArrayParams(gen::uniform_random, 1'000, "UniformRandom1M"),
        ArrayParams(gen::sequential, 100, "Sequential100"),
        ArrayParams(gen::sequential, 1'000, "Sequential1M"),
    };
}

INSTANTIATE_TEST_SUITE_P(
    SortingTestSuite,
    SortingTest,
    testing::ValuesIn(GenerateTestCases()),
    [](const testing::TestParamInfo<ArrayParams>& info) {
        return info.param.name;
    }
);