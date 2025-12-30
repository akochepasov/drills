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
    std::vector<std::string> resulting_tokens; resulting_tokens.reserve(256);

    if (str.size() == 0) {
        resulting_tokens.push_back(str);
        return resulting_tokens;
    }

    size_t strsz = str.size();
    size_t b = 0, e = 0;
    while (b < strsz) {   // go across the whole strings
        while (e < strsz && str[e] != delimiter)   // go till the delimilter
            e++;

        resulting_tokens.emplace_back(str.substr(b, e - b));
        b = e + 1;
        e++;
    }
    
    return resulting_tokens;
}

VS SplitWithLibs(const std::string& str, char delimiter) {
	VS result; result.reserve(256);
	std::istringstream ss(str);
    std::string token;
    while (getline(ss, token, delimiter)) {
        result.PB(token);
    }

    return result;
}

class TokenizeText : public ::testing::Test {
protected:
    vector<VS> param_tokens =
    {
        VS{"one,two,three", "one", "two", "three"},
        VS{ "hello", "hello" },
        VS{ "", "" }
    };
};

TEST_F(TokenizeText, SplitSimpleWithDelimiter) {
    char delimiter = ',';

    for (auto& str_param : param_tokens) {
        const std::string& str = str_param[0];
        VS expected_tokens(str_param.begin() + 1, str_param.end());
        std::vector<std::string> resulting_tokens = SplitSimpleWithDelimiter(str, delimiter);

        EXPECT_EQ(resulting_tokens, expected_tokens);
    }
}

TEST_F(TokenizeText, SplitWithLibs) {
    char delimiter = ',';

    for (auto& str_param : param_tokens) {
        const std::string& str = str_param[0];
        VS expected_tokens(str_param.begin() + 1, str_param.end());
        std::vector<std::string> resulting_tokens = SplitSimpleWithDelimiter(str, delimiter);

        EXPECT_EQ(resulting_tokens, expected_tokens);
    }
}

