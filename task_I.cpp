#include <iostream>
#include <vector>
#include <string>



#define matrix std::vector<std::vector<size_t>>


bool check_profiles(size_t profile_from, size_t profile_to, size_t m) {
    for (size_t i = 0; i + 1 < m; i++) {
        size_t first_from = (profile_from >> i) & 1;
        size_t second_from = (profile_from >> (i + 1)) & 1;
        size_t first_to = (profile_to >> i) & 1;
        size_t second_to = (profile_to >> (i + 1)) & 1;

        if (first_from == second_from &&
            first_from == first_to &&
            first_from == second_to) {

            return false;
        }
    }

    return true;
}


matrix multiply(matrix &first, matrix &second, size_t mod) {
    size_t size = first.size();

    matrix result(size, std::vector<size_t>(size, 0));

    for (size_t i = 0; i < size; i++) {
        for (size_t k = 0; k < size; k++) {
            if (first[i][k] == 0) {
                continue;
            }

            for (size_t j = 0; j < size; j++) {
                result[i][j] += first[i][k] * second[k][j];
                result[i][j] %= mod;
            }
        }
    }

    return result;
}


std::vector<size_t> multiply(matrix &mat, std::vector<size_t> &dp, size_t mod) {

    size_t size = mat.size();

    std::vector<size_t> result(size, 0);

    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            result[i] += mat[i][j] * dp[j];
            result[i] %= mod;
        }
    }

    return result;
}


void minus_one(std::string &number) {
    size_t i = number.size();

    while (i > 0) {
        i--;

        if (number[i] != '0') {
            number[i]--;
            break;
        }

        number[i] = '9';
    }

    while (number.size() > 1 && number[0] == '0') {
        number.erase(number.begin());
    }
}


bool is_zero(std::string &number) {
    return number == "0";
}


bool is_odd(std::string &number) {
    return (number.back() - '0') % 2;
}


void divide_by_two(std::string &number) {
    size_t carry = 0;

    for (size_t i = 0; i < number.size(); i++) {
        size_t cur = carry * 10 + number[i] - '0';

        number[i] = char(cur / 2 + '0');
        carry = cur % 2;
    }

    while (number.size() > 1 && number[0] == '0') {
        number.erase(number.begin());
    }
}


int main() {
    std::string n;
    size_t m = 0;
    size_t mod = 0;

    std::cin >> n >> m >> mod;

    size_t profile_limit = 1ULL << m;

    matrix transitions(profile_limit, std::vector<size_t>(profile_limit, 0));


    for (size_t profile_from = 0; profile_from < profile_limit; profile_from++) {

        for (size_t profile_to = 0; profile_to < profile_limit; profile_to++) {

            if (check_profiles(profile_from, profile_to, m)) {
                transitions[profile_to][profile_from] = 1;
            }
        }
    }


    std::vector<size_t> dp(profile_limit, 1);

    minus_one(n);


    while (!is_zero(n)) {
        if (is_odd(n)) {
            dp = multiply(transitions, dp, mod);
        }

        transitions = multiply(transitions, transitions, mod);

        divide_by_two(n);
    }


    size_t answer = 0;

    for (size_t profile = 0; profile < profile_limit; profile++) {
        answer += dp[profile];
        answer %= mod;
    }

    std::cout << answer;
}
