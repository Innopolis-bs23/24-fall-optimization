/**
 * Mikhail Panteleev
 * aka
 *   ▄███████▄  ▄██████▄   ▄█        ▄█  ███▄▄▄▄      ▄████████ ███▄▄▄▄    ▄█    ▄▄▄▄███▄▄▄▄      ▄████████
 *   ███    ███ ███    ███ ███       ███  ███▀▀▀██▄   ███    ███ ███▀▀▀██▄ ███  ▄██▀▀▀███▀▀▀██▄   ███    ███
 *   ███    ███ ███    ███ ███       ███▌ ███   ███   ███    ███ ███   ███ ███▌ ███   ███   ███   ███    █▀
 *   ███    ███ ███    ███ ███       ███▌ ███   ███   ███    ███ ███   ███ ███▌ ███   ███   ███  ▄███▄▄▄
 * ▀█████████▀  ███    ███ ███       ███▌ ███   ███ ▀███████████ ███   ███ ███▌ ███   ███   ███ ▀▀███▀▀▀
 *   ███        ███    ███ ███       ███  ███   ███   ███    ███ ███   ███ ███  ███   ███   ███   ███    █▄
 *   ███        ███    ███ ███▌    ▄ ███  ███   ███   ███    ███ ███   ███ ███  ███   ███   ███   ███    ███
 *  ▄████▀       ▀██████▀  █████▄▄██ █▀    ▀█   █▀    ███    █▀   ▀█   █▀  █▀    ▀█   ███   █▀    ██████████
 */

#include <iostream>
#include <utility>
#include <vector>
#include <iomanip>
#include <cmath>
#include "simplex_wrapper.h"

#define ALPHA 0.5

// Class to represent matrix
//  ███╗░░░███╗░█████╗░████████╗██████╗░██╗██╗░░██╗
//  ████╗░████║██╔══██╗╚══██╔══╝██╔══██╗██║╚██╗██╔╝
//  ██╔████╔██║███████║░░░██║░░░██████╔╝██║░╚███╔╝░
//  ██║╚██╔╝██║██╔══██║░░░██║░░░██╔══██╗██║░██╔██╗░
//  ██║░╚═╝░██║██║░░██║░░░██║░░░██║░░██║██║██╔╝╚██╗
//  ╚═╝░░░░░╚═╝╚═╝░░╚═╝░░░╚═╝░░░╚═╝░░╚═╝╚═╝╚═╝░░╚═╝
template <typename T>
class Matrix
{
public:
    int columnsNumber;
    int rowsNumber;
    std::vector<std::vector<T>> data; // 2D vector to store matrix data
public:
    // Constructor to initialize matrix using row and column numbers
    Matrix(int n, int m)
        : rowsNumber(n), columnsNumber(m), data(n, std::vector<T>(m))
    {
    }

    // Constructor to initialize matrix using data
    Matrix(int n, int m, std::vector<std::vector<T>> data)
        : rowsNumber(n), columnsNumber(m), data(std::move(data))
    {
    }

    // Copy constructor
    Matrix(Matrix const &other)
        : rowsNumber(other.rowsNumber), columnsNumber(other.columnsNumber), data(std::move(other.data))
    {
    }

    // Default destructor
    ~Matrix() = default;

    // Overloaded operator "<<" to output matrix data to stream
    friend std::ostream &operator<<(std::ostream &output, const Matrix &matrix)
    {
        for (int i = 0; i < matrix.rowsNumber; i++)
        {
            for (int j = 0; j < matrix.columnsNumber; j++)
            {
                output << matrix.data[i][j];
                if (j < matrix.columnsNumber - 1)
                    output << " ";
            }
            output << "\n";
        }
        return output;
    }

    // Overloaded operator ">>" to input matrix data from stream
    friend std::istream &operator>>(std::istream &input, Matrix &matrix)
    {
        for (int i = 0; i < matrix.rowsNumber; i++)
        {
            for (int j = 0; j < matrix.columnsNumber; j++)
            {
                input >> matrix.data[i][j];
            }
        }
        return input;
    }

    // Overloaded assigning operator
    Matrix &operator=(const Matrix &other)
    {
        if (this != &other)
        {
            std::vector<std::vector<T>> newData(other.rowsNumber, std::vector<T>(other.columnsNumber));
            for (int i = 0; i < other.rowsNumber; ++i)
            {
                for (int j = 0; j < other.columnsNumber; ++j)
                    newData[i][j] = other.data[i][j];
            }
            data = std::move(newData);
            rowsNumber = other.rowsNumber;
            columnsNumber = other.columnsNumber;
        }
        return *this;
    }

    // Overloaded arithmetics operators
    //////////////////////////////////////////////////////////////////////////
    Matrix operator+(const Matrix &other) const
    {
        if (rowsNumber != other.rowsNumber || columnsNumber != other.columnsNumber)
            throw std::invalid_argument("Error: the dimensional problem occurred");

        Matrix output(rowsNumber, columnsNumber);
        for (int i = 0; i < rowsNumber; i++)
        {
            for (int j = 0; j < columnsNumber; j++)
            {
                output.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return output;
    }

    Matrix operator-(const Matrix &other) const
    {
        if (rowsNumber != other.rowsNumber || columnsNumber != other.columnsNumber)
            throw std::invalid_argument("Error: the dimensional problem occurred");

        Matrix output(rowsNumber, columnsNumber);
        for (int i = 0; i < rowsNumber; i++)
        {
            for (int j = 0; j < columnsNumber; j++)
            {
                output.data[i][j] = data[i][j] - other.data[i][j];
            }
        }
        return output;
    }

    Matrix operator*(const Matrix &other) const
    {
        if (columnsNumber != other.rowsNumber)
            throw std::invalid_argument("Error: the dimensional problem occurred");
        Matrix output(rowsNumber, other.columnsNumber);

        for (int i = 0; i < rowsNumber; i++)
        {
            for (int j = 0; j < other.columnsNumber; j++)
            {
                for (int k = 0; k < columnsNumber; k++)
                {
                    output.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return output;
    }

    const std::vector<T> &operator[](int index) const
    {
        return data[index];
    }

    //////////////////////////////////////////////////////////////////////////

    // Method to transpose this matrix
    Matrix transpose() const
    {
        std::vector<std::vector<T>> newData(columnsNumber,
                                            std::vector<T>(rowsNumber));

        for (int i = 0; i < columnsNumber; ++i)
        {
            for (int j = 0; j < rowsNumber; ++j)
            {
                newData[i][j] = this->data[j][i];
            }
        }
        return Matrix<T>(columnsNumber, rowsNumber, newData);
    }

    // Data getter
    const std::vector<std::vector<T>> &getData() const
    {
        return data;
    }

    // Columns number getter
    int getColumnsNumber() const
    {
        return columnsNumber;
    }

    // Rows number getter
    int getRowsNumber() const
    {
        return rowsNumber;
    }

    void setValue(int i, int j, T value)
    {
        data[i][j] = value;
    }
};

// Class to represent Square matrix
// This class is inherented from Matrix class
//    ░██████╗░██████╗░██╗░░░██╗░█████╗░██████╗░███████╗      ███╗░░░███╗░█████╗░████████╗██████╗░██╗██╗░░██╗
//    ██╔════╝██╔═══██╗██║░░░██║██╔══██╗██╔══██╗██╔════╝      ████╗░████║██╔══██╗╚══██╔══╝██╔══██╗██║╚██╗██╔╝
//    ╚█████╗░██║██╗██║██║░░░██║███████║██████╔╝█████╗░░      ██╔████╔██║███████║░░░██║░░░██████╔╝██║░╚███╔╝░
//    ░╚═══██╗╚██████╔╝██║░░░██║██╔══██║██╔══██╗██╔══╝░░      ██║╚██╔╝██║██╔══██║░░░██║░░░██╔══██╗██║░██╔██╗░
//    ██████╔╝░╚═██╔═╝░╚██████╔╝██║░░██║██║░░██║███████╗      ██║░╚═╝░██║██║░░██║░░░██║░░░██║░░██║██║██╔╝╚██╗
//    ╚═════╝░░░░╚═╝░░░░╚═════╝░╚═╝░░╚═╝╚═╝░░╚═╝╚══════╝      ╚═╝░░░░░╚═╝╚═╝░░╚═╝░░░╚═╝░░░╚═╝░░╚═╝╚═╝╚═╝░░╚═╝

template <typename T>
class SquareMatrix : public Matrix<T>
{
public:
    // Constructor  to initialize matrix using row and column numbers (Square matrix => n == m)
    explicit SquareMatrix(int size)
        : Matrix<T>(size, size)
    {
    }

    // Constructor to initialize matrix using row and column numbers and data
    SquareMatrix(int size, std::vector<std::vector<T>> data)
        : Matrix<T>(size, size, std::move(data))
    {
    }

    // Copy constructor
    explicit SquareMatrix(Matrix<T> other)
        : Matrix<T>(other)
    {
    }

    // Overloaded operators using using upcasting and downcasting via pointers
    //////////////////////////////////////////////////////////////////////////
    SquareMatrix operator+(const SquareMatrix &other) const
    {
        return (SquareMatrix)((Matrix<T>)*this + (Matrix<T>)other);
    }

    SquareMatrix operator-(const SquareMatrix &other) const
    {
        return (SquareMatrix)((Matrix<T>)*this - (Matrix<T>)other);
    }

    SquareMatrix operator*(const SquareMatrix &other) const
    {
        return (SquareMatrix)((Matrix<T>)*this * (Matrix<T>)other);
    }
    //////////////////////////////////////////////////////////////////////////
};

// Pretty simple class to represent identity matrix
// Just another initial data and one way to initialize
// Obv every else is the same as in Square matrix
//    ██╗██████╗░███████╗███╗░░██╗████████╗██╗████████╗██╗░░░██╗      ███╗░░░███╗░█████╗░████████╗██████╗░██╗██╗░░██╗
//    ██║██╔══██╗██╔════╝████╗░██║╚══██╔══╝██║╚══██╔══╝╚██╗░██╔╝      ████╗░████║██╔══██╗╚══██╔══╝██╔══██╗██║╚██╗██╔╝
//    ██║██║░░██║█████╗░░██╔██╗██║░░░██║░░░██║░░░██║░░░░╚████╔╝░      ██╔████╔██║███████║░░░██║░░░██████╔╝██║░╚███╔╝░
//    ██║██║░░██║██╔══╝░░██��╚████║░░░██║░░░██║░░░██║░░░░░╚██╔╝░░      ██║╚██╔╝██║██╔══██║░░░██║░░░██╔══██╗██║░██╔██╗░
//    ██║██████╔╝███████╗██║░╚███║░░░██║░░░██║░░░██║░░░░░░██║░░░      ██║░╚═╝░██║██║░░██║░░░██║░░░██║░░██║██║██╔╝╚██╗
//    ╚═╝╚═════╝░╚══════╝╚═╝░░╚══╝░░░╚═╝░░░╚═╝░░░╚═╝░░░░░░╚═╝░░░      ╚═╝░░░░░╚═╝╚═╝░░╚═╝░░░╚═╝░░░╚═╝░░╚═╝╚═╝╚═╝░░╚═╝
template <typename T>
class IdentityMatrix : public SquareMatrix<T>
{
public:
    explicit IdentityMatrix(int size)
        : SquareMatrix<T>(size, std::vector<std::vector<T>>(size, std::vector<T>(size, 0)))
    {
        for (int i = 0; i < size; i++)
            this->data[i][i] = 1;
    }
};

// Class to represent elimination matrix
// It is the same matrix as Square, but initial data is Identity matrix (that's wht we inherit it from identity)
// And elimination matrix has one special method - nulifyElement
//    ███████╗██╗░░░░░██╗███╗░░░███╗██╗███╗░░██╗░█████╗░████████╗██╗░█████╗░███╗░░██╗
//    ██╔════╝██║░░░░░██║████╗░████║██║████╗░██║██╔══██╗╚══██╔══╝██║██╔══██╗████╗░██║
//    █████╗░░██║░░░░░██║██╔████╔██║██║██╔██╗██║███████║░░░██║░░░██║██║░░██║██╔██╗██║
//    ██╔══╝░░██║░░░░░██║██║╚██╔╝██║██║██║╚████║██╔══██║░░░██║░░░██║██║░░██║██║╚████║
//    ███████╗███████╗██║██║░╚═╝░██║██║██║░╚███║██║░░██║░░░██║░░░██║╚█████╔╝██║░╚███║
//    ╚══════╝╚══════╝╚═╝╚═╝░░░░░╚═╝╚═╝╚═╝░░╚══╝╚═╝░░╚═╝░░░╚═╝░░░╚═╝░╚════╝░╚═╝░░╚══╝
//
//    ███╗░░░███╗░█████╗░████████╗██████╗░██╗██╗░░██╗
//    ████╗░████║██╔══██╗╚══██╔══╝██╔══██╗██║╚██╗██╔╝
//    ██╔████╔██║███████║░░░██║░░░██████╔╝██║░╚███╔╝░
//    ██║╚██╔╝██║██╔══██║░░░██║░░░██╔══██╗██║░██╔██╗░
//    ██║░╚═╝░██║██║░░██║░░░██║░░░██║░░██║██║██╔╝╚██╗
//    ╚═╝░░░░░╚═╝╚═╝���░╚═╝░░░╚═╝░░░╚═╝░░╚═╝╚═╝╚═╝░░╚═╝
template <typename T>
class EliminationMatrix : public IdentityMatrix<T>
{
public:
    explicit EliminationMatrix(int size)
        : IdentityMatrix<T>(size)
    {
    }

    // Method to nulify matrix[x][y] with pivoting by nearest row
    // It was used for task #3
    void nulifyElement(int x, int y, Matrix<T> matrix)
    {
        if (x < 0 || x >= this->rowsNumber || y < 0 || y >= this->rowsNumber || this->rowsNumber != matrix.getRowsNumber() || this->columnsNumber != matrix.getColumnsNumber() || this->rowsNumber == 1 || this->columnsNumber == 1)
            throw std::invalid_argument("Error: the dimensional problem occurred");
        if (matrix.getData()[x][y] == 0)
            return;
        // check division by zero
        if (x - 1 >= 0 && matrix.getData()[(x - 1)][y] == 0)
            return;
        if (x - 1 < 0 && matrix.getData()[(x + 1)][y] == 0)
            return;
        this->data[x][y] = -matrix.getData()[x][y] / matrix.getData()[(x - 1) >= 0 ? (x - 1) : (x + 1)][y];
    }

    // Method to nulify matrix[column][y] using element matrix[column][y] and given pivot
    // Used for Tasks #4-6 (maybe more)
    void nulifyElement(int x, int y, int column, double pivot, Matrix<T> matrix)
    {
        if (x < 0 || x >= this->rowsNumber || y < 0 || y >= this->rowsNumber || this->rowsNumber != matrix.getRowsNumber() || this->columnsNumber != matrix.getColumnsNumber() || this->rowsNumber == 1 || this->columnsNumber == 1)
            throw std::invalid_argument("Error: the dimensional problem occurred");
        if (pivot == 0)
            return;
        this->data[column][x] = -matrix.getData()[column][y] / pivot;
    }
};

// Class to represent permutation matrix
// It is the same matrix as Square, but initial data is Identity matrix (that's wht we inherit it from identity)
// And elimination matrix has one special method - swapRows
//    ██████╗░███████╗██████╗░███╗░░░███╗██╗░░░██╗████████╗░█████╗░████████╗██╗░█████╗░███╗░░██╗
//    ██╔══██╗██╔════╝██╔══██╗████╗░████║██║░░░██║╚══██╔══╝██╔══██╗╚══██╔══╝██║██╔══██╗████╗░██║
//    ██████╔╝█████╗░░██████╔╝██╔████╔██║██║░░░██║░░░██║░░░███████║░░░██║░░░██║██║░░██║██╔██╗██║
//    ██╔═══╝░██╔══╝░░██╔══██╗██║╚██╔╝██║██║░░░██║░░░██║░░░██╔══██║░░░██║░░░██║██║░░██║██║╚████║
//    ██║░░░░░███████╗██║░░██║██║░╚═╝░██║╚██████╔╝░░░██║░░░██║░░██║░░░██║░░░██║╚█████╔╝██║░╚███║
//    ╚═╝░░░░░╚══════╝╚═╝░░╚═╝╚═╝░░░░░╚═╝░╚═════╝░░░░╚═╝░░░╚═╝░░╚═╝░░░╚═╝░░░╚═╝░╚════╝░╚═╝░░╚══╝
//
//    ███╗░░░███╗░█████╗░████████╗██████╗░██╗██╗░░██╗
//    ████╗░████║██╔══██╗╚══██╔══╝██╔══██╗██║╚██╗██╔╝
//    ██╔████╔██║███████║░░░██║░░░██████╔╝██║░╚███╔╝░
//    ██║╚██╔╝██║██╔══██║░░░██║░░░██╔══██╗██║░██╔��█╗░
//    ██║░╚═╝░██║██║░░██║░░░██║░░░██║░░██║██║██╔╝╚██╗
//    ╚═╝░░░░░╚═╝╚═╝░░╚═╝░░░╚═╝░░░╚═╝░░╚═╝╚═╝╚═╝░░╚═╝

template <typename T>
class PermutationMatrix : public IdentityMatrix<T>
{
public:
    explicit PermutationMatrix(int size)
        : IdentityMatrix<T>(size)
    {
    }

    // Method to add to this matrix permutations of matrix[x] and matrix[y]
    // (maybe it is easier to swap PermutationMatrix[x] and PermutationMatrix[y]...)
    void swapRows(int x, int y)
    {
        if (x < 0 || x >= this->rowsNumber || y < 0 || y >= this->rowsNumber)
            throw std::invalid_argument("Error: the dimensional problem occurred");
        this->data[x][x] = 0;
        this->data[y][y] = 0;
        this->data[x][y] = 1;
        this->data[y][x] = 1;
    }
};

// Class to represent augmented matrix
// It takes two matrices of any sizes...
// maybe it is better to check rows numbers...
//    ░█████╗░██╗░░░██╗░██████╗░███╗░░░███╗███████╗███╗░░██╗████████╗███████╗██████╗░
//    ██╔══██╗██║░░░██║██╔════╝░████╗░████║██╔════╝████╗░██║╚══██╔══╝██╔════╝██╔══██╗
//    ███████║██║░░░██║██║░░██╗░██╔████╔██║█████╗░░██╔██╗██║░░░██║░░░█████╗░░██║░░██║
//    ██╔══██║██║░░░██║██║░░╚██╗██║╚██╔╝██║██╔══╝░░██║╚████║░░░██║░░░██╔══╝░░██║░░██║
//    ██║░░██║╚██████╔╝╚██████╔╝██║░╚═╝░██║███████╗██║░╚███║░░░██║░░░███████╗██████╔╝
//    ╚═╝░░╚═╝░╚═════╝░░╚═════╝░╚═╝░░░░░╚═╝╚══════╝╚═╝░░╚══╝░░░╚═╝░░░╚══════╝╚═════╝░
//
//    ███╗░░░███╗░█████╗░████████╗██████╗░██╗██╗░░██╗
//    ████╗░████║██╔══██╗╚══██╔══╝██╔══██╗██║╚██╗██╔╝
//    ██╔████╔██║███████║░░░██║░░░██████╔╝██║░╚███╔╝░
//    ██║╚██╔╝██║██╔══██║░░░██║░░░██╔══██╗██║░██╔██╗░
//    ██║░╚═╝░██║██║░░██║░░░██║░░░██║░░██║██║██╔╝╚██╗
//    ╚═╝░░░░░╚═╝╚═╝░░╚═╝░░░╚═╝░░░╚═╝░░╚═╝╚═╝╚═╝░░╚═╝

template <typename T>
class AugmentedMatrix
{

protected:
    Matrix<T> leftMatrix;
    Matrix<T> rightMatrix;

public:
    AugmentedMatrix(Matrix<T> leftMatrix, Matrix<T> rightMatrix)
        : leftMatrix(leftMatrix), rightMatrix(rightMatrix)
    {
    }

    // Overloaded arithmetics operators
    // Just do the same operation with left and right matrices
    //////////////////////////////////////////////////////////////////////////
    AugmentedMatrix operator+(const AugmentedMatrix &other) const
    {
        return AugmentedMatrix<T>(other + leftMatrix, other + rightMatrix);
    }

    AugmentedMatrix operator-(const AugmentedMatrix &other) const
    {
        return AugmentedMatrix<T>(other - leftMatrix, other - rightMatrix);
    }

    AugmentedMatrix operator*(const Matrix<T> &other) const
    {
        return AugmentedMatrix<T>(other * leftMatrix, other * rightMatrix);
    }
    //////////////////////////////////////////////////////////////////////////

    // Some useful (?) getters:
    //////////////////////////////////////////////////////////////////////////
    // Data is left matrix
    // Why? Because I found it intuitively correct
    // and in some functions method matrix.getData was used and I didnt want to rewrite it...
    const std::vector<std::vector<T>> &getData() const
    {
        return leftMatrix.getData();
    }

    const std::vector<std::vector<T>> &getAugmentationData() const
    {
        return rightMatrix.getData();
    }

    Matrix<T> getLeftMatrix() const
    {
        return leftMatrix;
    }

    Matrix<T> getRightMatrix() const
    {
        return rightMatrix;
    }

    int getColumnsNumber() const
    {
        return leftMatrix.getColumnsNumber();
    }

    int getRowsNumber() const
    {
        return leftMatrix.getRowsNumber();
    }
    //////////////////////////////////////////////////////////////////////////

    // Method to represent augmented matrix as one matrix with left and right matrices concatenated
    // Used in output<< operator
    const Matrix<T> asMatrix() const
    {

        std::vector<std::vector<T>> data(leftMatrix.getRowsNumber(),
                                         std::vector<T>(
                                             leftMatrix.getColumnsNumber() + rightMatrix.getColumnsNumber()));
        for (int i = 0; i < leftMatrix.getRowsNumber(); i++)
        {
            for (int j = 0; j < leftMatrix.getColumnsNumber(); j++)
            {
                data[i][j] = leftMatrix.getData()[i][j];
            }
            for (int j = 0; j < rightMatrix.getColumnsNumber(); j++)
            {
                data[i][j + leftMatrix.getColumnsNumber()] = rightMatrix.getData()[i][j];
            }
        }
        return Matrix<T>(leftMatrix.getRowsNumber(),
                         leftMatrix.getColumnsNumber() + rightMatrix.getColumnsNumber(),
                         data);
    }

    // Obviously i guess
    friend std::ostream &operator<<(std::ostream &output, const AugmentedMatrix<T> &matrix)
    {
        output << matrix.asMatrix();
        return output;
    }
};

// Class to represent column vector
// It is the same as matrix (n x 1)
//    ░█████╗░░█████╗░██╗░░░░░██╗░░░██╗███╗░░░███╗███╗░░██╗      ██╗░░░██╗███████╗░█████╗░████████╗░█████╗░██████╗░
//    ██╔══██╗██╔══██╗██║░░░░░██║░░░██║████╗░████║████╗░██║      ██║░░░██║██╔════╝██╔══██╗╚══██╔══╝██╔══██╗██╔══██╗
//    ██║░░╚═╝██║░░██║██║░░░░░██║░░░██║██╔████╔██║██╔██╗██║      ╚██╗░██╔╝█████╗░░██║░░╚═╝░░░██║░░░██║░░██║██████╔╝
//    ██║░░██╗██║░░██║██║░░░░░██║░░░██║██║╚██╔╝██║██║╚████║      ░╚████╔╝░██╔══╝░░██║░░██╗░░░██║░░░██║░░██║██╔══██╗
//    ╚█████╔╝╚█████╔╝███████╗╚██████╔╝██║░╚═╝░██║██║░╚███║      ░░╚██╔╝░░███████╗╚█████╔╝░░░██║░░░╚█████╔╝██║░░██║
//    ░╚════╝░░╚════╝░╚���═════╝░╚═════╝░╚═╝░░░░╚═╝╚═╝░░╚══╝      ░░░╚═╝░░░╚══════╝░╚════╝░░░░╚═╝░░░░╚════╝░╚═╝░░╚═╝
template <typename T>
class ColumnVector : public Matrix<T>
{
public:
    explicit ColumnVector(int size)
        : Matrix<T>(size, 1)
    {
    }

    // Conversion constructor from Matrix to ColumnVector
    explicit ColumnVector(const Matrix<T> &matrix)
        : Matrix<T>(matrix)
    {
        if (matrix.getColumnsNumber() != 1)
        {
            throw std::invalid_argument(
                "Error: the dimensional problem occurred. A ColumnVector can only be created from a Matrix with one column.");
        }
    }

    // Overloaded arithmetics operators
    //////////////////////////////////////////////////////////////////////////
    ColumnVector operator+(const ColumnVector &other) const
    {
        if (this->rowsNumber != other.rowsNumber)
            throw std::invalid_argument("Error: the dimensional problem occurred");
        return (ColumnVector)((Matrix<T>)*this + (Matrix<T>)other);
    }

    ColumnVector operator-(const ColumnVector &other) const
    {
        if (this->rowsNumber != other.rowsNumber)
            throw std::invalid_argument("Error: the dimensional problem occurred");
        return (ColumnVector)((Matrix<T>)*this - (Matrix<T>)other);
    }

    ColumnVector operator*(const T &other) const
    {
        ColumnVector output(this->rowsNumber);
        for (int i = 0; i < this->rowsNumber; i++)
            output.data[i][0] = this->data[i][0] * other;
        return output;
    }

    T operator*(const ColumnVector &other) const
    {
        if (this->rowsNumber != other.rowsNumber)
            throw std::invalid_argument("Error: the dimensional problem occurred");
        T output = 0;
        for (int i = 0; i < this->rowsNumber; i++)
            output += this->data[i][0] * other.data[i][0];
        return output;
    }
    //////////////////////////////////////////////////////////////////////////

    // Norm computations
    // Why do we need it..?
    T computeNorm()
    {
        T sum = 0;
        for (auto i : this->data)
            sum += i[0] * i[0];
        return sqrt(sum);
    }

    friend std::ostream &operator<<(std::ostream &output, const ColumnVector<T> &vector)
    {
        for (int i = 0; i < vector.rowsNumber; i++)
            output << vector.data[i][0] << "\n";
        // output << std::endl;
        return output;
    }

    friend std::istream &operator>>(std::istream &input, ColumnVector<T> &vector)
    {
        for (int i = 0; i < vector.rowsNumber; i++)
            input >> vector.data[i][0];
        return input;
    }
};

// Method to transform augmented matrix to diagonal
template <typename T>
AugmentedMatrix<T> transform2diagonal(const AugmentedMatrix<T> &inputMatrix, bool printMessages = false)
{
    // It uses only left matrix data and repeats all the actions on the right matrix

    AugmentedMatrix<T> matrix(inputMatrix);

    int stepNumber = 1;
    T det = 1;

    if (printMessages)
        std::cout << "Gaussian process:" << std::endl;

    for (int i = 0; i < matrix.getRowsNumber(); i++)
    {
        T max = 0;
        int maxIndex = i;
        for (int j = i; j < matrix.getRowsNumber(); j++)
        {
            if (std::abs(matrix.getData()[j][i]) > max)
            {
                max = std::abs(matrix.getData()[j][i]);
                maxIndex = j;
            }
        }

        if (maxIndex != i)
        {
            PermutationMatrix<T> P(matrix.getRowsNumber());
            P.swapRows(i, maxIndex);
            matrix = matrix * (Matrix<T>)P;
            det *= -1;

            if (printMessages)
            {
                std::cout << "step #" << stepNumber++ << ": permutation" << std::endl;
                std::cout << matrix.getLeftMatrix()
                          << matrix.getRightMatrix().transpose(); // transpose to output as row vector bellow matrix
            }
        }

        T pivot = matrix.getData()[i][i];
        for (int j = i + 1; j < matrix.getRowsNumber(); ++j)
        {
            EliminationMatrix<T> E(matrix.getRowsNumber());
            if (matrix.getData()[j][i] != 0)
            { // do not nulify zero elements
                E.nulifyElement(i, i, j, pivot, matrix.getLeftMatrix());

                matrix = matrix * (Matrix<T>)E;

                if (printMessages)
                {
                    std::cout << "step #" << stepNumber++ << ": elimination" << std::endl;
                    std::cout << matrix.getLeftMatrix() << matrix.getRightMatrix().transpose();
                }
            }
        }
    }

    // Nulify elements over main diagonal
    // We do not to swap any rows as we have REF form already to not mess anything up
    for (int i = matrix.getRowsNumber() - 1; i >= 0; i--)
    {
        T pivot = matrix.getData()[i][i];
        for (int j = i - 1; j >= 0; --j)
        {
            EliminationMatrix<T> E(matrix.getRowsNumber());
            E.nulifyElement(i, i, j, pivot, matrix.getLeftMatrix());
            matrix = matrix * (Matrix<T>)E;

            if (printMessages)
            {
                std::cout << "step #" << stepNumber++ << ": elimination" << std::endl;
                std::cout << matrix.getLeftMatrix() << matrix.getRightMatrix().transpose();
            }
        }
    }

    // Normalize diagonal
    if (printMessages)
        std::cout << "Diagonal normalization:" << std::endl;

    // Well... I thought it realised better...
    // I just created matrix to not initialize std::vector by hands..? Bruh... ok...
    SquareMatrix<T> sample(matrix.getColumnsNumber());
    auto data = sample.getData();
    for (int i = 0; i < matrix.getColumnsNumber(); ++i)
    {
        data[i][i] = 1 / matrix.getLeftMatrix().getData()[i][i];
    }
    SquareMatrix<T> normalizer(matrix.getColumnsNumber(), data);

    matrix = matrix * (Matrix<T>)normalizer;
    if (printMessages)
        std::cout << matrix.getLeftMatrix() << matrix.getRightMatrix().transpose();
    return matrix;
}

// Method to inverse matrix using Gaussian method
template <typename T>
Matrix<T> inverseMatrix(const Matrix<T> &matrixInput, bool printMessages = false)
{
    AugmentedMatrix<T> matrix(matrixInput, IdentityMatrix<T>(matrixInput.getRowsNumber()));
    if (printMessages)
        std::cout << "Augmented matrix:\n"
                  << matrix;

    // Just transform left matrix to identity matrix => right matrix is inverse for initial matrix
    AugmentedMatrix<T> inversedMatrix = transform2diagonal(matrix, printMessages);
    return inversedMatrix.getRightMatrix();
}

template <typename T>
ColumnVector<T> interior(ColumnVector<T> c, Matrix<T> A, ColumnVector<T> x, double eps)
{
    // Create ones vector with size n (number of variables)
    ColumnVector<T> ones(x.getRowsNumber());
    for (int i = 0; i < x.getRowsNumber(); i++)
    {
        ones.data[i][0] = 1;
    }

    uint i = 1;
    Matrix<T> A_Prime(A.getRowsNumber(), A.getColumnsNumber());
    ColumnVector<T> c_Prime(x.getRowsNumber());
    Matrix<T> P(x.getRowsNumber(), x.getRowsNumber());
    ColumnVector<T> c_p(x.getRowsNumber());
    ColumnVector<T> x_prev = x;
    ColumnVector<T> x_p(x.getRowsNumber());
    double nu;

    while (true)
    {
        Matrix<T> D(x.getRowsNumber(), x.getRowsNumber());
        for (int j = 0; j < x.getRowsNumber(); j++)
        {
            D.data[j][j] = x.data[j][0];
        }

        A_Prime = A * D;
        c_Prime = ColumnVector<T>(D * c);
        P = ((Matrix<T>)IdentityMatrix<T>(x.getRowsNumber())) - A_Prime.transpose() * inverseMatrix(A_Prime * A_Prime.transpose()) * A_Prime;
        c_p = ColumnVector<T>(P * c_Prime);
        x_prev = x;

        nu = -1;
        for (int i = 0; i < c_p.getRowsNumber(); i++)
        {
            if (c_p.data[i][0] < 0 && nu < -c_p.data[i][0])
            {
                nu = -c_p.data[i][0];
            }
        }

        for (int i = 0; i < c_p.getRowsNumber(); i++)
        {
            c_p.data[i][0] *= ALPHA / nu;
        }

        x_p = ones + c_p;
        x = x_p; // Update x for next iteration

        if ((x_p - x_prev).computeNorm() < eps)
        {
            break;
        }
        else
        {
            std::cout << "At iteration " << i << " we have x=\n"
                      << x_p << std::endl;
        }
        i++;
    }
    return x_p;
}

template <typename T>
ColumnVector<T> simplex_method(ColumnVector<T> c, Matrix<T> A, ColumnVector<T> b, double eps, int n, int m, bool maximize)
{
    init_python();
    PyObject *py_c = PyList_New(n);
    PyObject *py_A = PyList_New(m);
    PyObject *py_b = PyList_New(m);

    // Fill c vector
    for (int i = 0; i < n; i++)
    {
        PyList_SetItem(py_c, i, Py_BuildValue("d", c.data[i][0]));
    }

    // Fill A matrix
    for (int i = 0; i < m; i++)
    {
        PyObject *row = PyList_New(n);
        for (int j = 0; j < n; j++)
        {
            PyList_SetItem(row, j, Py_BuildValue("d", A.data[i][j]));
        }
        PyList_SetItem(py_A, i, row);
    }

    // Fill b vector
    for (int i = 0; i < m; i++)
    {
        PyList_SetItem(py_b, i, Py_BuildValue("d", b.data[i][0]));
    }

    PyObject *py_result = call_simplex_method(py_c, py_A, py_b, eps, true);

    std::cout << "Simplex result: \n"
              << py_result << std::endl;
    ColumnVector<double> result(n);
    for (int i = 0; i < n; i++)
    {
        result.data[i][0] = PyFloat_AsDouble(PyList_GetItem(py_result, i));
    }
    return result;
}

int main()
{
    std::cout << "Input sizes of system (n variables, m equations): ";
    int n, m;
    std::cin >> n >> m;
    while (n <= 0 || m <= 0)
    {
        std::cout << "Sizes must be positive. Try again: ";
        std::cin >> n >> m;
    }

    ColumnVector<double> c(n);
    Matrix<double> A(m, n);
    ColumnVector<double> x(n);
    ColumnVector<double> b(m);
    double eps;

    std::cout << "Input vector c: ";
    std::cin >> c;
    // std::cout << c << std::endl;
    std::cout << "Input matrix A: ";
    std::cin >> A;
    // std::cout << A << std::endl;
    std::cout << "Input vector b: ";
    std::cin >> b;
    // std::cout << b << std::endl;
    std::cout << "Input vector x: ";
    std::cin >> x;
    // std::cout << x << std::endl;
    std::cout << "Input accuracy eps: ";
    std::cin >> eps;

    // Running simplex method from python code (Assigment 1)
    ColumnVector<double> simplex_result = simplex_method(c, A, b, eps, n, m, true);
    std::cout << "Simplex result: \n"
              << simplex_result << std::endl;

    // Running interior point method (Assigment 2)
    ColumnVector<double> interior_result = interior(c, A, x, eps);
    std::cout << "Interior result: \n"
              << interior_result << std::endl;

    return 0;
}