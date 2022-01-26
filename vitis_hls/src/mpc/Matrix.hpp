#pragma once

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

/*!
@file   Matrix.hpp
*/

/*!
@brief A class support for matrices
@tparam N number of rows
@tparam M number of columns
@tparam T Data type. float as default
*/
template<int N, int M, typename T = float>
class Matrix
{
public:
    /*!
    @brief Empty constructor
     */
	Matrix() { }

    /*!
    @brief Create matrix with initial values. If it's a diagonal matrix, all elements outside are zeros
    @param init Initial value for matrix
    @param diagonal Check if is a diagonal matrix
    */
	Matrix(T init, bool diagonal = false)
	{
		for(int i = 0; i < N; i++)
		{
			for(int j = 0; j < M; ++j)
			{
				if(!diagonal || i == j)
				{
					m_values[i][j] = init;
				}
				else
				{
					m_values[i][j] = 0;
				}
			}
		}
	}
    /*!
    @brief Creates a matrix using 1-D array elements
    @param data Pointer to array with init values
    */
	Matrix(const T *data)
	{
		for(int i = 0; i < N; i++)
		{
			for(int j = 0; j < M; ++j)
			{
				m_values[i][j] = data[M*i + j];
			}
		}
	}
    /*!
    @brief Creates a matrix from a vector.
    @param init Vector used to create matrix
    @param diagonal If true, init will be the diagonal of the matrix. If false, init will be copied in every column of the matrix
    */
	Matrix(const Matrix<N,1,T> &init, bool diagonal = false)
	{
		for(int i = 0; i < N; i++)
		{
			for(int j = 0; j < M; ++j)
			{
				if(!diagonal || i == j)
				{
					m_values[i][j] = init(i,0);
				}
				else
				{
					m_values[i][j] = 0;
				}
			}
		}
	}
    /*!
    @brief Extracts a value from matrix
    @param row
    @param column
    @return Matrix value in given position
    */
	T &operator()(int row, int column)
	{
		assert(row >= 0 && row < N);
		assert(column >= 0 && column < M);

		return m_values[row][column];
	}
    /*!
    @brief This is an overloaded operator provided by convenience.
    @param row
    @param column
    @return Matrix value in given position
    */
	const T &operator()(int row, int column) const
	{
		assert(row >= 0 && row < N);
		assert(column >= 0 && column < M);

		return m_values[row][column];
	}
    /*!
    @brief Allow assignment as matrix copy
    @param rhs Right hand of the assignment. Assigned matrix
    @return Asigned matrix
    */
	Matrix<N,M,T> &operator=(const Matrix<N,M,T> &rhs)
	{
		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				m_values[i][j] = rhs(i,j);
			}
		}

		return *this;
	}
    /*!
    @brief Overloaded member function. Provided by convenience.
    @param rhs Value to assign to every matrix slot
    @return Asigned matrix
    */
	Matrix<N,M,T> &operator=(const T &rhs)
	{
		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				m_values[i][j] = rhs;
			}
		}

		return *this;
	}
    /*!
    @brief Matrix negative operator
    @return Matrix with inverted sign
    */
	Matrix<N,M,T> operator-() const
	{
		Matrix<N,M,T> res;

		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				res(i,j) = -m_values[i][j];
			}
		}

		return res;
	}

	// Matrix-Matrix operations
	/*!
    @brief Matrix add operator
    @param rhs Matrix to add
    @return Matrix sum
    */

	Matrix<N,M,T> operator+(const Matrix<N,M,T> &rhs) const
	{
		Matrix<N,M,T> res;

		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				res(i,j) = m_values[i][j] + rhs(i,j);
			}
		}

		return res;
	}

    /*!
    @brief Matrix difference operator
    @param rhs Matrix to subtract
    @return Matrix difference
    */
	Matrix<N,M,T> operator-(const Matrix<N,M,T> &rhs) const
	{
		Matrix<N,M,T> res;

		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				res(i,j) = m_values[i][j] - rhs(i,j);
			}
		}

		return res;
	}
    /*!
    @brief Multiplication operator
    @tparam P Number of columns of input/right-hand matrix
    @param rhs Right hand of matrix multiplication
    @return Result matrix
    */
	template<int P>
	Matrix<N,P,T> operator*(const Matrix<M,P,T> &rhs) const
	{
		Matrix<N,P,T> res;

		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < P; ++j)
			{
				res(i,j) = m_values[i][0] * rhs(0,j);

				for(int k = 1; k < M; ++k)
				{
					res(i,j) += m_values[i][k] * rhs(k,j);
				}
			}
		}

		return res;
	}

	// Self-Matrix operations

    /*!
    @brief Matrix add overloaded for convenience
    @param rhs Right side operator. Matrix to be added.
    */
	Matrix<N,M,T> &operator+=(const Matrix<N,M,T> &rhs)
	{
		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				m_values[i][j] += rhs(i,j);
			}
		}

		return *this;
	}
    /*!
    @brief Substract operation overload for convenience
    @param rhs Right-side matrix to substract
    */
	Matrix<N,M,T> &operator-=(const Matrix<N,M,T> &rhs)
	{
		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				m_values[i][j] -= rhs(i,j);
			}
		}

		return *this;
	}

	// Matrix-Value operations

	/*!
    @brief Matrix plus scalar overload for convenience. Allows to add a scalar value for every matrix position
    @param rhs Scalar value to add to matrix
    @return Resulting matrix
    */
	Matrix<N,M,T> operator+(const T &rhs)
	{
		Matrix<N,M> res;

		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				res(i,j) = m_values[i][j] + rhs;
			}
		}

		return res;
	}
    /*!
    @brief  Matrix minus scalar overload for convenience. Allows to substract a scalar value for every matrix position
    @param  rhs Scalar value to substract.
    @return Resulting matrix
    */

	Matrix<N,M,T> operator-(const T &rhs)
	{
		Matrix<N,M,T> res;

		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				res(i,j) = m_values[i][j] - rhs;
			}
		}

		return res;
	}

    /*!
    @brief  Scalar-matrix product overload for convenience
    @param T Scalar value for matrix multiplication
    @return Resulting matrix
    */
	Matrix<N,M,T> operator*(const T &rhs)
	{
		Matrix<N,M,T> res;

		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				res(i,j) = m_values[i][j] * rhs;
			}
		}

		return res;
	}

	/*!
    @brief  Matrix division by scalar for convenience
    @param  T   Scalar value used to perform matrix division
    @return Resulting matrix
    */
	Matrix<N,M,T> operator/(const T &rhs)
	{
		Matrix<N,M,T> res;

		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				res(i,j) = m_values[i][j] / rhs;
			}
		}

		return res;
	}

	/*!
    @brief  Power operator for convenience. Computes the power operation for every matrix position
    @param  T   Exponent to power the matrix values
    @result Resulting matrix
    */
	Matrix<N,M,T> operator^(const T &rhs)
	{
		Matrix<N,M,T> res;

		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				res(i,j) = pow(m_values[i][j], rhs);
			}
		}

		return res;
	}

	// Self-Value operations

	/*!
    @brief  Matrix plus scalar operation overloaded for convenience
    @param  T   Scalar value to add
    */
	Matrix<N,M,T> operator+=(const T &rhs)
	{
		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				m_values[i][j] += rhs;
			}
		}

		return *this;
	}

    /*!
    @brief  Matrix minus scalar overload, provided for convenience.
    @param  rhs Scalar value to substract
    */
	Matrix<N,M,T> operator-=(const T &rhs)
	{
		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				m_values[i][j] -= rhs;
			}
		}

		return *this;
	}

	/*!
    @brief  Matrix by scalar operation overloaded for convenience.
    @param  T   Scalar value to multiply
    */
	Matrix<N,M,T> operator*=(const T &rhs)
	{
		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				m_values[i][j] *= rhs;
			}
		}

		return *this;
	}

	/*!
    @brief  Matrix divided by scalar operation overloaded for convenience.
    @param  T   Scalar value for divition
    */
	Matrix<N,M,T> operator/=(const T &rhs)
	{
		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				m_values[i][j] /= rhs;
			}
		}

		return *this;
	}

	/*!
    @brief  Scalar power for matrix overloaded.
    @param  T   Scalar value to power to.
    */
	Matrix<N,M,T> operator^=(const T &rhs)
	{
		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				m_values[i][j] = ::pow(m_values[i][j], rhs);
			}
		}

		return *this;
	}

	// Others

	/*!
    @brief  Squared sum method. Computes squared sum for every element in current matrix.
    @return Squared sum.
    */
	T squaredSum() const
	{
		T res = 0;

		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				res += ::pow(m_values[i][j], 2);
			}
		}

		return res;
	}

	/*!
    @brief  Dot product method. Computes the dot product between the current matrix and the given matrix.
    @param  rhs Matrix to compute the dot product
    @return Scalar result of the inner product.
    */
	T dot(const Matrix<N,M,T> &rhs) const
	{
		T res = 0;

		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				res += m_values[i][j] * rhs(i,j);
			}
		}

		return res;
	}

    /*!
    @brief  Element-wise multiplication method. The current matrix is updated with the result
    @param  rhs Matrix to perform the element-wise multiplication
    */
	void emul(const Matrix<N,M,T> &rhs)
	{
		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				m_values[i][j] *= rhs(i,j);
			}
		}
	}

    /*!
    @brief  Element-wise divition method. The current matrix is updated with the result.
    @param  rhs Matrix to perform the element-wise divition
    */
	void ediv(const Matrix<N,M,T> &rhs)
	{
		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				m_values[i][j] /= rhs(i,j);
			}
		}
	}

    /*!
    @brief  Element-wise matrix multiplication method.
    @param  rhs Matrix to perform the element-wise multiplication
    @return Resulting matrix
    */
	Matrix<N,M,T> emulCopy(const Matrix<N,M,T> &rhs) const
	{
		Matrix<N,M,T> res;

		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				res(i,j) = m_values[i][j] * rhs(i,j);
			}
		}

		return res;
	}

	/*!
    @brief  Element-wise matrix divition method.
    @param  rhs Matrix to perform the element-wise divition
    @return Resulting matrix
    */
	Matrix<N,M,T> edivCopy(const Matrix<N,M,T> &rhs) const
	{
		Matrix<N,M,T> res;

		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				res(i,j) = m_values[i][j] / rhs(i,j);
			}
		}

		return res;
	}

	/*!
    @brief  Multiply current matrix with the transpose input matrix. This method allows to avoid single tranpose operations by computing multiplication using convenient indexes.
    @tparam P   Second dimension of the input matrix. Column size of rhs
    @param  rhs Matrix to be transposed and multiplied
    @return Result of current matrix by the transpose of the input matrix, rhs.
    */
	template<int P>
	Matrix<M,P,T> multTr(const Matrix<N,P,T> &rhs) const
	{
		Matrix<M,P,T> res;

		for(int i = 0; i < M; ++i)
		{
			for(int j = 0; j < P; ++j)
			{
				res(i,j) = m_values[0][i] * rhs(0,j);

				for(int k = 1; k < N; ++k)
				{
					res(i,j) += m_values[k][i] * rhs(k,j);
				}
			}
		}

		return res;
	}

	template<int begin, int size, int times>
	void repeat()
	{
		static_assert(M == 1, "Must be used on a vector");
		static_assert(begin >= 0, "Begin offset must be at least 0");
		static_assert(size > 0, "Number of elements must be at least 1");
		static_assert(times > 0, "Number of copies must be at least 1");
		static_assert(begin + size*times <= N, "Repetition overflows the vector");

		for(int i = begin+size, j = begin; i < begin + size*times; ++i, ++j)
		{
			if(j >= begin+size)
			{
				j = begin;
			}

			m_values[i][0] = m_values[j][0];
		}
	}

    /*!
    @brief  Power matrix method. Computes the matrix by itself product
    @param  exponent    Times to compute the matrix by itself product.
    @return Resulting matrix
    */
	Matrix<N,M,T> pow(int exponent) const
	{
		static_assert(N == M, "Matrix must be square");

		Matrix<N,M,T> res = *this;

		for(int i = 1; i < exponent; ++i)
		{
			res = res * *this;
		}

		return res;
	}

    /*!
    @brief Transpose matrix operation method.
    @result Transposed matrix
    */
	Matrix<M,N,T> transpose() const
	{
		Matrix<M,N,T> res;

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < M; j++)
			{
				res(j,i) = m_values[i][j];
			}
		}

		return res;
	}

	T mse(const std::vector<T> &ref) const
	{
		T res = 0.0;
		int k = 0;

		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				res += ::pow(ref[k] - m_values[j][i], 2) / (N * M);
				++k;
			}
		}

		return res;
	}

	// IO

	/*!
    @brief  Load values from Standard Input method. Allows to complete matrix values by reading loadFromStdin
    @return Matrix with the read values.
    */
	static Matrix<N,M,T> loadFromStdin()
	{
		Matrix<N,M,T> res;

		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < M; ++j)
			{
				std::cin >> res(i, j);
			}
		}

		return res;
	}

    /*!
    @brief  Prints in a single line matrix values separated with spaces.
    */
	void printLine() const
	{
		for(int i = 0; i < N; i++)
		{
			for(int j = 0; j < M; j++)
			{
				if(i != 0 || j != 0)
				{
					std::cout << " ";
				}

				std::cout << m_values[i][j];
			}
		}

		std::cout << std::endl;
	}

	void printLine(std::ostream &output) const
	{
		for(int i = 0; i < N; i++)
		{
			for(int j = 0; j < M; j++)
			{
				if(i != 0 || j != 0)
				{
					output << " ";
				}

				output << m_values[i][j];
			}
		}

		output << std::endl;
	}

	/*!
    @brief  Prints formatted matrix
    */
	void print() const
	{
		for(int i = 0; i < N; i++)
		{
			std::cout << "[";

			for(int j = 0; j < M; j++)
			{
				if(j != 0)
				{
					std::cout << ", ";
				}

				std::cout << m_values[i][j];
			}

			std::cout << "]" << std::endl;
		}
	}

private:
    //! Array container for matrix values
	T m_values[N][M];
};
