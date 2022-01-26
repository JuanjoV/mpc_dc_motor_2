#pragma once

#include "Matrix.hpp"

/*!
@file   cgrad.hpp
*/

/*!
@brief Gradient-descent algorithm. Solves linear system of N equations in the form Ax=b using the gradient descent method.
@tparam N   Number of equations of the linear system
@tparam T   Data type. float as default
@param  A   NxN matrix with system coefficients
@param  b   Nx1 vector with system constant terms
@param  x0  Nx1 vector with starting points for the algorithm
@param  iter_max    Maximum of iterations of the algorithm
@param  tolerance   Maximum error tolerated by the algorithm
@param  x   Nx1 vector with the aproximated solution
@return Number of iterations performed
*/
template<int iter_max, int N, typename T = float>
int cgrad(const Matrix<N,N,T> &A, const Matrix<N,1,T> &b, Matrix<N,1,T> &x0, T tolerance, Matrix<N,1,T> &x)
{
	Matrix<N,1,T> r = b - A*x0;
	Matrix<N,1,T> d = r;
	Matrix<N,1,T> q;
	T dw = r.squaredSum();
	T tce = sqrt(dw);
	T alpha = 0;
	T beta = 0;
	int i = 0;

	x = x0;

	while(i < iter_max)
	{
		if(tce <= tolerance) break;

		q = A*d;
		alpha = dw / d.dot(q);

		x += d*alpha;

		if((i % 50) == 0)
		{
			r = b - A*x;
		}
		else
		{
			r -= q*alpha;
		}

		T dl = dw;
		dw = r.squaredSum();

		beta = dw / dl;
		d = r + d*beta;

		++i;
		tce = sqrt((x - x0).squaredSum());
		x0 = x;
	}

	return i;
}
