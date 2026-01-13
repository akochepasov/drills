#include "drillheader.h"
#include <gtest/gtest.h>


bool ValidParentheses(const std::string& str) {
    std::stack<char> stack;
    const std::string opening = {'[', '{', '('};
    const std::string closing = {']', '}', ')'};

    FOR(i, 0, SZ(str)) {
        if (opening.find(str[i]) != string::npos) {
            stack.push(str[i]);
        } else {
            if (stack.empty())
                return false;
            if (opening[closing.find(str[i])] != stack.top())
                return false;
            stack.pop();
        }
    }

    return stack.empty();
}


class StringBoolParams: public ::testing::TestWithParam<std::pair<std::string, bool>> {};

TEST_P(StringBoolParams, ValidParentheses) {
    auto [str_param, exp_res] = GetParam();

    auto act_res = ValidParentheses(str_param);

    EXPECT_EQ(exp_res, act_res);
}

INSTANTIATE_TEST_SUITE_P(
    StringAlgos,        // Test suite name prefix
    StringBoolParams,   // Fixture class
    ::testing::Values(  // Values
        std::pair{"(})", false},
        std::pair{"}}}", false},
        std::pair{"(]", false},
        std::pair{"()", true},
        std::pair{"((((", false},
        std::pair{"()[]{}", true},
        std::pair{"([])}", false},
        std::pair{"([])", true},
        std::pair{"([)]", false}
    )
);

