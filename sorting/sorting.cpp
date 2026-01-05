#include <array>
#include <drillheader.h>
#include <gtest/gtest.h>

// Array generators
namespace gen {
    using ArrayGenerator = std::function<void(std::vector<int>&)>;

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
    FOR(i, 0, SZ(arr)) {
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

size_t LowerBound(std::vector<int>& arr, int v) {
    
    if (arr.size() == 0) return arr.size();

    size_t l = 0, r = arr.size();

    while (l < r) {
        auto m = l + (r - l) / 2;

        if (arr[m] < v)
            l = m + 1;
        else
            r = m;
    }

    return (l < arr.size() && arr[l] < v) ? r + 1 : l;
}

struct ArrayParams {
    gen::ArrayGenerator generator;
    size_t size;
    std::string name;

    ArrayParams(gen::ArrayGenerator gen, size_t sz, std::string n) 
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

TEST_P(SortingTest, LowerBound) {
    std::sort(ALL(arr));

    int m = arr.size() / 2;
    int m2 = min<int>(3, m / 2);
    int m3 = min<int>(2, 2 * m / 3);
    std::array<int, 5> ndxs = {0, static_cast<int>(arr.size()), 1, m, m2};

    for (auto i : ndxs) {
        auto v = (i < arr.size()) ? arr[i] : i;
        size_t lb = LowerBound(arr, v);
        auto it = std::lower_bound(ALL(arr), v);
        size_t lbe = (it != arr.end()) ? it - arr.begin() : arr.size();
        EXPECT_EQ(lb, lbe);
    }
}

std::vector<ArrayParams> GenerateTestCases() {
    return {
        ArrayParams(gen::uniform_random, 1, "OneElement1"),
        ArrayParams(gen::uniform_random, 1, "OneElement2"),
        ArrayParams(gen::uniform_random, 1, "OneElement3"),
        ArrayParams(gen::uniform_random, 100, "UniformRandom100"),
        ArrayParams(gen::uniform_random, 1'000, "UniformRandom1M"),
        ArrayParams(gen::sequential, 100, "Sequential100"),
        ArrayParams(gen::sequential, 1'000, "Sequential1M"),
        ArrayParams(gen::uniform_random, 0, "Empty"),
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
