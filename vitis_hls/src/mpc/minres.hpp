#pragma once

#include "Matrix.hpp"

/*!
@file   minres.hpp
*/

/*!
@brief  Minimal Residual method for solving linear systems in the form Ax=b.
@tparam N   Number of equations of the linear systems
@tparam T   Data size
@param  A   NxN matrix of coefficients of the system
@param  b   Nx1 vector of constants of the systems
@param  x0  Initial values for algorithm iterations
@param  iter_max    Maximum number of iterations
@param  tolerance   Maximum desirable error to stop iterations
@param  x   Nx1 vector for resulting values.
@return Number of iterations performed by the algorithm
*/
template<int iter_max, int N, typename T = float>
int minres(const Matrix<N,N,T> &A, const Matrix<N,1,T> &b, Matrix<N,1,T> &x0, T tolerance, Matrix<N,1,T> &x)
{
	Matrix<N,1,T> v(0, false), w(0, false), v_old, w_old, Av;
	Matrix<N,1,T> v_hat = b - A*x0;
	T beta = sqrt(v_hat.squaredSum());
	T beta_old = beta;
	T norm_rMR = beta;
	T norm_r0 = beta;
	T eta = beta;
	T c_old = 1;
	T s_old = 0;
	T c = 1;
	T s = 0;
	T alpha;
	int i = 0;

	x = x0;
	w_old = w;

	while(i < iter_max)
	{
		if((norm_rMR / norm_r0) <= tolerance) break;

		// Lanczos

		v_old = v;
		v = v_hat / beta;
		Av = A * v;
		alpha = v.dot(Av);
		v_hat = Av - v * alpha - v_old * beta;

		beta_old = beta;
		beta = sqrt(v_hat.squaredSum());

		// QR Factorization

		T c_oold = c_old;
		T s_oold = s_old;

		c_old = c;
		s_old = s;

		T r1_hat = c_old * alpha - c_oold * s_old * beta_old;
		T r1 = sqrt(r1_hat*r1_hat + beta*beta);
		T r2 = s_old * alpha + c_oold * c_old * beta_old;
		T r3 = s_oold * beta_old;

		// Givens rotation

		c = r1_hat / r1;
		s = beta / r1;

		// Update

		Matrix<N,1,T> w_oold = w_old;
		w_old = w;
		w = (v - w_oold * r3 - w_old * r2) / r1;
		x += w * (c * eta);

		norm_rMR *= fabs(s);
		eta *= -s;

		++i;
	}

	return i + 1;
}
