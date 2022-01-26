#pragma once

#include "Matrix.hpp"
#include "cgrad.hpp"
#include "lschol.hpp"
#include "minres.hpp"

/*!
@file   pdip.hpp
*/

enum Solvers
{
	MINRES,        /*! Minimal Residual method */
	CGRAD,         /*! Gradient Descent method */
	CHOLESKY       /*! Cholesky factorization based method */
};

template<Solvers solver, int N, int iter_max, typename T = float>
struct SolverDispatch
{ };

template<int N, int iter_max, typename T>
struct SolverDispatch<MINRES, N, iter_max, T>
{
	static void call(const Matrix<N,N,T> &A, const Matrix<N,1,T> &b, Matrix<N,1,T> &x0, T tolerance, Matrix<N,1,T> &x)
	{
		#pragma HLS INLINE
		minres<iter_max>(A, b, x0, tolerance, x);
	}
};

template<int N, int iter_max, typename T>
struct SolverDispatch<CGRAD, N, iter_max, T>
{
	static void call(const Matrix<N,N,T> &A, const Matrix<N,1,T> &b, Matrix<N,1,T> &x0, T tolerance, Matrix<N,1,T> &x)
	{
		#pragma HLS INLINE
		cgrad<iter_max>(A, b, x0, tolerance, x);
	}
};

template<int N, int iter_max, typename T>
struct SolverDispatch<CHOLESKY, N, iter_max, T>
{
	static void call(const Matrix<N,N,T> &A, Matrix<N,1,T> &b, Matrix<N,1,T>&, T, Matrix<N,1,T> &x)
	{
		#pragma HLS INLINE
		lschol(A, b, x);
	}
};
