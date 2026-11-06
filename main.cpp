/*******************************************************************************
 * File:          main.cpp
 * Course:        EECS 348 - Software Engineering, Fall 2026
 * Assignment:    Lab 10 - C++ Programming (Matrix Operations)
 * Author:        Hunter Noravong
 * KU ID:         3162520
 * Date Created:  November 4, 2026
 *
 * Description:   Defines a Matrix class and uses it to read two N x N matrices from a file, then
 *                demonstrates: addition and multiplication via operator
 *                overloading, diagonal sums, row/column swapping, and element
 *                updates. The swap/update functions take the Matrix by value so
 *                the caller's original matrix is never modified.
 *
 * Inputs:        Name of a text file entered by the user. File format:
 *                  line 1: integer N (matrix size)
 *                  next N lines: first N x N matrix
 *                  next N lines: second N x N matrix
 *
 * Outputs:       Both matrices, A + B, A * B, diagonal sums of A, and the
 *                results of swapping rows, swapping columns, and updating an
 *                element of A, all printed with aligned columns.
 *
 * Collaborators: None
 * Other Sources: None
 ******************************************************************************/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <string>
 
 
 
class Matrix {
private:
    int n_;       // matrix is n_ x n_
    int* data_;   // row-major storage
 
public:
    explicit Matrix(int n = 0) : n_(n), data_(n > 0 ? new int[n * n]() : nullptr) {}
    Matrix(const Matrix& o) : n_(o.n_), data_(o.n_ > 0 ? new int[o.n_ * o.n_] : nullptr) {
        std::copy(o.data_, o.data_ + n_ * n_, data_);
    }
    Matrix& operator=(Matrix o) { std::swap(n_, o.n_); std::swap(data_, o.data_); return *this; }
    ~Matrix() { delete[] data_; }
 
    int size() const { return n_; }
    int get(int r, int c) const { return data_[r * n_ + c]; }
    void set(int r, int c, int v) { data_[r * n_ + c] = v; }
    bool valid(int i) const { return i >= 0 && i < n_; }
 
    // Reads N, then two N x N matrices. Returns false on a bad file.
    static bool readFromFile(const std::string& name, Matrix& a, Matrix& b) {
        std::ifstream in(name);
        int n;
        if (!(in >> n) || n <= 0) return false;
        a = Matrix(n);
        b = Matrix(n);
        for (int i = 0; i < n * n; ++i) if (!(in >> a.data_[i])) return false;
        for (int i = 0; i < n * n; ++i) if (!(in >> b.data_[i])) return false;
        return true;
    }
 
    // Element-wise addition.
    Matrix operator+(const Matrix& r) const {
        Matrix res(n_);
        for (int i = 0; i < n_ * n_; ++i) res.data_[i] = data_[i] + r.data_[i];
        return res;
    }
 
    // Matrix multiplication: row of this dotted with column of r.
    Matrix operator*(const Matrix& r) const {
        Matrix res(n_);
        for (int i = 0; i < n_; ++i)
            for (int j = 0; j < n_; ++j)
                for (int k = 0; k < n_; ++k)
                    res.data_[i * n_ + j] += get(i, k) * r.get(k, j);
        return res;
    }
 
    int mainDiagonalSum() const {
        int s = 0;
        for (int i = 0; i < n_; ++i) s += get(i, i);
        return s;
    }
 
    int secondaryDiagonalSum() const {
        int s = 0;
        for (int i = 0; i < n_; ++i) s += get(i, n_ - 1 - i);
        return s;
    }
 
    // Prints with aligned columns (width at least 4, or longest entry + 1).
    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        size_t w = 4;
        for (int i = 0; i < m.n_ * m.n_; ++i)
            w = std::max(w, std::to_string(m.data_[i]).size() + 1);
        for (int i = 0; i < m.n_; ++i) {
            for (int j = 0; j < m.n_; ++j) os << std::setw(static_cast<int>(w)) << m.get(i, j);
            os << '\n';
        }
        return os;
    }
};
 
// Practices 5-7 take the Matrix by value, so the caller's copy is never modified.
Matrix swapRows(Matrix m, int r1 = 0, int r2 = 1) {
    if (!m.valid(r1) || !m.valid(r2)) { std::cout << "Error: invalid row index.\n"; return m; }
    for (int j = 0; j < m.size(); ++j) {
        int t = m.get(r1, j);
        m.set(r1, j, m.get(r2, j));
        m.set(r2, j, t);
    }
    return m;
}
 
Matrix swapCols(Matrix m, int c1 = 0, int c2 = 1) {
    if (!m.valid(c1) || !m.valid(c2)) { std::cout << "Error: invalid column index.\n"; return m; }
    for (int i = 0; i < m.size(); ++i) {
        int t = m.get(i, c1);
        m.set(i, c1, m.get(i, c2));
        m.set(i, c2, t);
    }
    return m;
}
 
Matrix updateElement(Matrix m, int row = 0, int col = 0, int value = 100) {
    if (!m.valid(row) || !m.valid(col)) { std::cout << "Error: invalid index.\n"; return m; }
    m.set(row, col, value);
    return m;
}
 
int main() {
    std::string filename;
    std::cout << "Enter input filename: ";
    std::cin >> filename;
 
    Matrix A, B;
    if (!Matrix::readFromFile(filename, A, B)) {
        std::cerr << "Error: could not read '" << filename << "'.\n";
        return 1;
    }
 
    std::cout << "\nMatrix A:\n" << A << "\nMatrix B:\n" << B;
    std::cout << "\nA + B:\n" << (A + B);
    std::cout << "\nA * B:\n" << (A * B);
    std::cout << "\nDiagonal sums for Matrix A:\n"
              << "Main diagonal sum: " << A.mainDiagonalSum() << '\n'
              << "Secondary diagonal sum: " << A.secondaryDiagonalSum() << '\n';
    std::cout << "\nProblem 5 - Rows 0 and 2 swapped:\n" << swapRows(A, 0, 2);
    std::cout << "\nProblem 6 - Columns 0 and 2 swapped:\n" << swapCols(A, 0, 2);
    std::cout << "\nProblem 7 - Updated matrix:\n" << updateElement(A, 1, 2, 99);
    return 0;
}
 