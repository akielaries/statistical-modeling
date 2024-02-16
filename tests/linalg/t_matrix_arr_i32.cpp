/*************************************************************************
 * Testing Mtx class operations
 ************************************************************************/
#include "../../include/linalg/mtx.hpp"
#include "../../include/linalg/mtx_tmpl.hpp"
#include <cmath>
#include <cstdint>
#include <gtest/gtest.h>
#include <iostream>
#include <limits.h>
#include <string>
#include <vector>

using namespace gpmp;
#define TEST_COUT std::cerr << "\033[32m[          ] [ INFO ] \033[0m"

namespace {

// utility test helper function to compare two matrices. used for verifying
// accelerated/non-standard implementations to the simple naive algorithm
// for matrix arithmetic operations
template <typename T>
bool mtx_verif(const std::vector<std::vector<T>> &A,
               const std::vector<std::vector<T>> &B) {
    if (A.size() != B.size() || A[0].size() != B[0].size()) {
        return false;
    }

    for (size_t i = 0; i < A.size(); ++i) {
        for (size_t j = 0; j < A[i].size(); ++j) {
            if (std::abs(A[i][j] - B[i][j]) >
                std::numeric_limits<T>::epsilon()) {
                return false;
            }
        }
    }
    return true;
}

template <typename T>
bool mtx_verif(const std::vector<T> &A, const std::vector<T> &B) {
    int rows = A.size();
    int cols = rows > 0 ? A.size() / rows : 0;

    if (A.size() != B.size()) {
        return false;
    }

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (A[i * cols + j] != B[i * cols + j]) {
                return false;
            }
        }
    }
    return true;
}

template <typename T>
bool mtx_verif(const T *A, const T *B, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (std::abs(A[i * cols + j] - B[i * cols + j]) >
                std::numeric_limits<T>::epsilon()) {
                return false;
            }
        }
    }
    return true;
}

template <typename T> void print_matrix(const T *matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << matrix[i * cols + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

TEST(MatrixArrayTestI32, AdditionComparisonSmall) {
    int mtx_size = 184;
    // define input matrices A and B
    int *A = new int[mtx_size * mtx_size];
    int *B = new int[mtx_size * mtx_size];
    int *expected = new int[mtx_size * mtx_size];
    int *result = new int[mtx_size * mtx_size];

    // initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(1, 100);

    // populate matrices A and B with random values
    for (int i = 0; i < mtx_size; ++i) {
        for (int j = 0; j < mtx_size; ++j) {
            A[i * mtx_size + j] = distribution(gen);
            B[i * mtx_size + j] = distribution(gen);
        }
    }

    gpmp::linalg::Mtx mtx;
    // expected result using the naive implementation
    mtx.std_mtx_add(A, B, expected, mtx_size, mtx_size);

    // result using the intrinsics implementation
    mtx.mtx_add(A, B, result, mtx_size, mtx_size);

    /*
    std::cout << "Matrix A:" << std::endl;
    print_matrix(A, mtx_size, mtx_size);

    std::cout << "Matrix B:" << std::endl;
    print_matrix(B, mtx_size, mtx_size);

    std::cout << "Matrix EXPECTED after addition:" << std::endl;
    print_matrix(expected, mtx_size, mtx_size);

    std::cout << "Matrix RESULT after addition:" << std::endl;
    print_matrix(result, mtx_size, mtx_size);
  */
    // compare the results
    ASSERT_TRUE(mtx_verif(expected, result, mtx_size, mtx_size));
    delete[] A;
    delete[] B;
    delete[] expected;
    delete[] result;
}

TEST(MatrixArrayTestI32, AdditionComparisonLarge) {
    int mtx_size = 1024;
    // define input matrices A and B
    int *A = new int[mtx_size * mtx_size];
    int *B = new int[mtx_size * mtx_size];
    int *expected = new int[mtx_size * mtx_size];
    int *result = new int[mtx_size * mtx_size];

    // initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(1, 100);

    // populate matrices A and B with random values
    for (int i = 0; i < mtx_size; ++i) {
        for (int j = 0; j < mtx_size; ++j) {
            A[i * mtx_size + j] = distribution(gen);
            B[i * mtx_size + j] = distribution(gen);
        }
    }

    gpmp::linalg::Mtx mtx;
    // expected result using the naive implementation
    mtx.std_mtx_add(A, B, expected, mtx_size, mtx_size);

    // result using the intrinsics implementation
    mtx.mtx_add(A, B, result, mtx_size, mtx_size);

    /*
        std::cout << "Matrix EXPECTED after addition:" << std::endl;
        for (int i = 0; i < mtx_size; ++i) {
            for (int j = 0; j < mtx_size; ++j) {
                std::cout << expected[i][j] << " ";
            }
            std::cout << std::endl;
        }

        std::cout << "Matrix RESULT after addition:" << std::endl;
        for (int i = 0; i < mtx_size; ++i) {
            for (int j = 0; j < mtx_size; ++j) {
                std::cout << result[i][j] << " ";
            }
            std::cout << std::endl;
        }
    */

    // compare the results
    ASSERT_TRUE(mtx_verif(expected, result, mtx_size, mtx_size));
    delete[] A;
    delete[] B;
    delete[] expected;
    delete[] result;
}

TEST(MatrixArrayTestI32, AdditionPerformanceComparison) {
    int mtx_size = 1024;
    TEST_COUT << "Matrix size      : " << mtx_size << std::endl;
    // define input matrices A and B
    int *A = new int[mtx_size * mtx_size];
    int *B = new int[mtx_size * mtx_size];
    int *expected = new int[mtx_size * mtx_size];
    int *result = new int[mtx_size * mtx_size];

    // initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(1, 100);

    // populate matrices A and B with random values
    for (int i = 0; i < mtx_size; ++i) {
        for (int j = 0; j < mtx_size; ++j) {
            A[i * mtx_size + j] = distribution(gen);
            B[i * mtx_size + j] = distribution(gen);
        }
    }

    gpmp::linalg::Mtx mtx;
    auto start_std = std::chrono::high_resolution_clock::now();

    // expected result using the naive implementation
    mtx.std_mtx_add(A, B, expected, mtx_size, mtx_size);

    auto end_std = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds_std = end_std - start_std;

    auto start_intrin = std::chrono::high_resolution_clock::now();

    // result using the intrinsics implementation
    mtx.mtx_add(A, B, result, mtx_size, mtx_size);
    auto end_intrin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds_intrin =
        end_intrin - start_intrin;

    TEST_COUT << "INTRINSIC Matrix Addition Time      : "
              << elapsed_seconds_intrin.count() << " seconds" << std::endl;
    TEST_COUT << "STANDARD  Matrix Addition Time      : "
              << elapsed_seconds_std.count() << " seconds" << std::endl;

    // compare the results
    ASSERT_TRUE(mtx_verif(expected, result, mtx_size, mtx_size));
    delete[] A;
    delete[] B;
    delete[] expected;
    delete[] result;
}
} // namespace
