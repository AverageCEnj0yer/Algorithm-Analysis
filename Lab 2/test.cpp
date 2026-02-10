
#include <iostream>
#include <gmpxx.h> // Includes the C++ wrapper
#include <chrono>
#include <cmath>
#include <vector>

using bigInt = mpz_class;


namespace Recursive {
    bigInt fib(const bigInt& n) {
        if (n == 0) return 0;
        if (n == 1) return 1;
        return fib(n - 1) + fib(n - 2);
    }
}

namespace Dynamic {
    bigInt fib(size_t n) {
        if (n == 0) return 0;
        if (n == 1) return 1;
        std::vector<bigInt> dp(n + 1);
        dp[0] = 0;
        dp[1] = 1;
        for (size_t i = 2; i <= n; ++i) {
            dp[i] = dp[i - 1] + dp[i - 2];
        }
        return dp[n];
    }
}

namespace Matrix {
    void multiply(bigInt F[2][2], bigInt M[2][2]) {
        bigInt x = F[0][0]*M[0][0] + F[0][1]*M[1][0];
        bigInt y = F[0][0]*M[0][1] + F[0][1]*M[1][1];
        bigInt z = F[1][0]*M[0][0] + F[1][1]*M[1][0];
        bigInt w = F[1][0]*M[0][1] + F[1][1]*M[1][1];
        F[0][0] = x;
        F[0][1] = y;
        F[1][0] = z;
        F[1][1] = w;
    }
    void power(bigInt F[2][2], const bigInt& n) {
        bigInt M[2][2] = {{1, 1}, {1, 0}};
        for (bigInt i = 2; i <= n; i++) {
            multiply(F, M);
        }
    }
    bigInt fib(const bigInt& n) {
        if (n == 0) return 0;
        bigInt F[2][2] = {{1, 1}, {1, 0}};
        power(F, n - 1);
        return F[0][0];
    }
}

namespace Binet {
    bigInt fib(const bigInt& n) {
        double sqrt5 = sqrt(5);
        double phi = (1 + sqrt5) / 2;
        double psi = (1 - sqrt5) / 2;
        mpf_class result = (pow(phi, n.get_d()) - pow(psi, n.get_d())) / sqrt5;
        return bigInt(result + 0.5); // round to nearest integer
    }
}

namespace Doubling {
    std::pair<bigInt, bigInt> helper(const bigInt& n) {
        if (n == 0) return {0, 1};
        auto p = helper(n / 2);
        bigInt a = p.first;
        bigInt b = p.second;
        bigInt c = a * (2 * b - a);
        bigInt d = a * a + b * b;
        if (n % 2 == 0)
            return {c, d};
        else
            return {d, c + d};
    }
    bigInt fib(const bigInt& n) {
        return helper(n).first;
    }
}

int main() {
    // Empirical analysis
    // Recursive tests
    std::vector<size_t> rec_tests = {5, 10, 15, 20, 25, 30, 35, 40};
    std::cout << "\nRecursive Method Results:\n";
    std::cout << "n\tAvg Time (ms)\n";
    for (size_t n : rec_tests) {
        double total = 0;
        for (int i = 0; i < 3; ++i) {
            auto start = std::chrono::steady_clock::now();
            bigInt res = Recursive::fib(bigInt(n));
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            total += duration.count();
        }
        std::cout << n << "\t" << (total / 3.0) << "\n";
    }

    // // Dynamic tests
    std::vector<size_t> dyn_tests = {10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};
    std::cout << "\nDynamic Method Results:\n";
    std::cout << "n\tAvg Time (ms)\n";
    for (size_t n : dyn_tests) {
        double total = 0;
        for (int i = 0; i < 3; ++i) {
            auto start = std::chrono::steady_clock::now();
            bigInt res = Dynamic::fib(n);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            total += duration.count();
        }
        std::cout << n << "\t" << (total / 3.0) << "\n";
    }

    // // Matrix tests
    std::vector<size_t> mat_tests = {10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};
    std::cout << "\nMatrix Method Results:\n";
    std::cout << "n\tAvg Time (ms)\n";
    for (size_t n : mat_tests) {
        double total = 0;
        for (int i = 0; i < 3; ++i) {
            auto start = std::chrono::steady_clock::now();
            bigInt res = Matrix::fib(bigInt(n));
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            total += duration.count();
        }
        std::cout << n << "\t" << (total / 3.0) << "\n";
    }

    // // Binet tests
    std::vector<size_t> binet_tests = {20, 40, 60, 80, 100, 120, 140};
    std::cout << "\nBinet Method Results:\n";
    std::cout << "n\tAvg Time (ms)\n";
    for (size_t n : binet_tests) {
        double total = 0;
        for (int i = 0; i < 3; ++i) {
            auto start = std::chrono::steady_clock::now();
            bigInt res = Binet::fib(bigInt(n));
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            total += duration.count();
        }
        std::cout << n << "\t" << (total / 3.0) << "\n";
    }

    // Fast Doubling tests
    std::vector<size_t> dbl_tests = {1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000, 9000000, 10000000};
    std::cout << "\nFast Doubling Method Results:\n";
    std::cout << "n\t\tAvg Time (ms)\n";
    for (size_t i = 0; i < dbl_tests.size(); ++i) {
        size_t n = dbl_tests[i];
        double total = 0;
        for (int j = 0; j < 3; ++j) {
            auto start = std::chrono::steady_clock::now();
            bigInt res = Doubling::fib(bigInt(n));
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            total += duration.count();
        }
        if (i == dbl_tests.size() - 1) {
            std::cout << n << "\t" << (total / 3.0) << "\n";
        } else {
            std::cout << n << "\t\t" << (total / 3.0) << "\n";
        }
    }

    return 0;
}
