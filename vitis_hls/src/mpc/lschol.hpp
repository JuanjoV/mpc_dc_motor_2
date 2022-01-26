#pragma once

#include "Matrix.hpp"

/*!
@file   lschol.hpp
*/

/*!
@brief  Solves a linear system using Cholesky factorization. Considers the system in the form Ax=v
@tparam N   Number of equations of the linear system
@tparam T   Data type
@param  A   NxN matrix with system coefficients
@param  v   Nx1 vector with constant coefficients
@param  x   Nx1 resulting vector with system solution
*/
template<int N, typename T = float>
void lschol(const Matrix<N,N,T> &A, Matrix<N,1,T> &v, Matrix<N,1,T> &x)
{
	Matrix<N,N,T> L;
	Matrix<N,1,T> D, sumCache(0, false);

	// Get diagonal and lower-triangular matrices

	for(int j = 0; j < N; ++j)
	{
		// Get value for the diagonal

		D(j,0) = A(j,j) - sumCache(j,0);

		// Iterate across rows

		for(int i = j+1; i < N; ++i)
		{
			T sum = A(i,j);

			for(int k = 0; k < j; ++k)
			{
				sum -= L(i,k) * L(j,k) * D(k,0);
			}

			T sum2 = sum / D(j,0);
			sumCache(i,0) += sum * sum2;

			L(i,j) = sum2;
		}
	}

	// Solve first system

	Matrix<N,1,T> y;

	for(int j = 0; j < N; ++j)
	{
#pragma HLS PIPELINE II=10
		y(j,0) = v(j,0);

		for(int i = j+1; i < N; ++i)
		{
			v(i,0) -= L(i,j) * v(j,0);
		}

		y(j,0) /= D(j,0);
	}

	// Solve final system

	for(int j = N-1; j >= 0; --j)
	{
#pragma HLS PIPELINE II=10
		x(j,0) = y(j,0);

		for(int i = j-1; i >= 0; --i)
		{
			y(i,0) -= L(j,i) * y(j,0);
		}
	}
}
