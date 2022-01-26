#pragma once

#include "Matrix.hpp"

/*!
@file   kron.hpp
*/

/*!
@brief  Computes the kronecker product between A and B matrices. Computes and concatenates A matrix by every single element of the matrix B, creating a AN*BNxAM*BM matrix. Created for software use.
@tparam AN  Number of rows of A matrix
@tparam AM  Number of columns of A matrix
@tparam BN  Number of rows of B matrix
@tparam BM  Number of columns of B matrix
@tparam T   Data type to use in computation
@param  A   Matrix to compute kronecker product
@param  B   Matrix to compute kronecker product
@return  New matrix with the kronecker product between A and B
*/
template<int AN, int AM, int BN, int BM, typename T = double>
Matrix<AN*BN, AM*BM, T> kron(const Matrix<AN, AM, T> &A, const Matrix<BN, BM, T> &B)
{
    Matrix<AN*BN, AM*BM, T> K;

	for (int i = 0; i < AN; i++)
	{
		for (int j = 0; j < AM; j++)
		{
			for (int ii = 0; ii < BN; ii++)
			{
				for (int jj = 0; jj < BM; jj++)
				{
					K(i * BN + ii, j * BM + jj) = A(i,j) * B(ii,jj);
				}
			}
		}
	}

    return K;
}
