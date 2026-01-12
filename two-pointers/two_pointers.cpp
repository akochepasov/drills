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

// Simple String Split
// Question: Implement a function that splits a string by a delimiter character
// and returns a vector of strings.

VS SplitSimpleWithDelimiter(const std::string& str, char delimiter) {
    std::vector<std::string> result; result.reserve(256);

    size_t strsz = SZ(str);
    if (strsz == 0) {
        result.push_back(str);
        return result;
    }

    size_t e = 0;
    FOR(b, 0, strsz) {   // go across the whole strings till the delimilter
        while (e < strsz && str[e] != delimiter) e++;

        result.emplace_back(str.substr(b, e - b));

        b = e; e++;
    }

    return result;
}

VS SplitWithLibs(const std::string& str, char delimiter) {
    VS result; result.reserve(256);
    if (SZ(str) == 0) {
        result.push_back(str);
        return result;
    }

	std::istringstream ss(str);
    std::string token;
    while (getline(ss, token, delimiter)) {
        result.PB(token);
    }

    return result;
}


class VectorStringParams: public ::testing::TestWithParam<std::vector<std::string>> {};

TEST_P(VectorStringParams, SplitSimpleWithDelimiter) {
    char delimiter = ',';
    auto str_param = GetParam();
    const std::string& str = str_param[0];
    VS exp_tokens(str_param.begin() + 1, str_param.end());

    std::vector<std::string> act_tokens1 = SplitSimpleWithDelimiter(str, delimiter);
    std::vector<std::string> act_tokens2 = SplitWithLibs(str, delimiter);

    EXPECT_EQ(act_tokens1, exp_tokens);
    EXPECT_EQ(act_tokens2, exp_tokens);
}

INSTANTIATE_TEST_SUITE_P(
    TwoPointers,        // Test suite name prefix
    VectorStringParams, // Fixture class
    ::testing::Values(  // Values
        VS{"one,two,three", "one", "two", "three"},
        VS{ "hello", "hello" },
        VS{ "", "" }
    )
);


int CheckCollision(const std::string& positions) {
    if (positions.size() < 2)
        return 0;

    int posLen = positions.size();

    // Skip all first L and last R
    int lpos = 0;
    while (lpos < posLen && positions[lpos] == 'L')
        lpos++;
    int rpos = posLen - 1;
    while (rpos != 0 && positions[rpos] == 'R')
        rpos--;

    if (lpos == rpos)
        return 0;

    int count = 0;
    for (int i = lpos; i <= rpos; i++) {
        count += (positions[i] != 'S');
    }

    return count;
}

class StringIntParams: public ::testing::TestWithParam<std::pair<std::string, int>> {};

TEST_P(StringIntParams, CheckCollision) {
    auto [positions, res] = GetParam();
    EXPECT_EQ(res, CheckCollision(positions));
}

INSTANTIATE_TEST_SUITE_P(
    TwoPointers,        // Test suite name prefix
    StringIntParams,    // Fixture class
    ::testing::Values(  // Values
        std::make_pair("RL", 2),
        std::make_pair("LLLL", 0),
        std::make_pair("RLRSLL", 5),
        std::make_pair("RRR", 0),
        std::make_pair("LLSRR", 0),
        std::make_pair("", 0)
    )
);
