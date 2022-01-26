#pragma once

#include "Matrix.hpp"
#include "cgrad.hpp"
#include "lschol.hpp"
#include "minres.hpp"
#include "solver_dispatch.hpp"

/*!
@file   pdip.hpp
*/

template <int N, typename T = float>
T computeAlp(Matrix<N, 1, T> delta, Matrix<N, 1, T> k)
{
	T bt = 0.99999;
	T alp = 1;

	for(int elem = 0; elem < N; elem++)
	{
		if(delta(elem,0) < 0 && -delta(elem,0)/k(elem,0) > alp)
		{
			alp = -delta(elem,0)/k(elem,0);
		}
	}

	return bt/alp;
}

/*!
@brief  Primal-Dual Interior-Point method for solving quadratic convex optimization.
        Quadratic programing (QP) problem solution:
                    min 0.5 tk'*H*tk + h'*tk
                    s.t     F*tk  = f
                            G*tk <= g
        Arrange the equality and inequality constraints into a inequality, being finally:
                    min 0.5 tk'*H*tk + h'*tk
                    s.t     Mx*tk <= cx
                            Mx=[G;F;-F]
                            cx=[g;f;-f]

@tparam S   Solver for linear systems
@tparam N   Number of optimization values
@tparam M   Number of systems constraints
@tparam P
@tparam T   Data type
@param  H   NxN Cost matrix
@param  h   NxP Cost vector
@param  Mx  MxN Matrix with constraints coefficients
@param  cx  Mx1 vector with constraints constants
@param  IT  Maximum of iterations for the main algorithm
@param  tol Error maximum tolerance considered for algorithms
@param  mrmax   Maximum of iterations for inner linear system solving. As default, is 20.
@return A Nx1 optimal solutions vector
*/
template<Solvers S = MINRES, int IT, int mrmax, int N, int M, int P, typename T = float>
Matrix<N,1,T> pdip(const Matrix<N,N,T> &H, const Matrix<N,P,T> &h, const Matrix<M,N,T> &Mx, const Matrix<M,1,T> &cx, T tol)
{
	Matrix<N, 1, T> tk(1.0);
	Matrix<M, 1, T> lk(0.5);
	Matrix<M, 1, T> sk(0.5);
	Matrix<M, 1, T> em(1.0);
	Matrix<N, 1, T> zko(0.0);
	T sgk = 0.5;

	for (int k = 0; k < IT; k++)
	{
		// Build Ak

		Matrix<M, M, T> RK(lk.edivCopy(sk),true);
		Matrix<M, M, T> RKI(sk.edivCopy(lk),true);
		Matrix<N, N, T> Ak = H + Mx.multTr(RK) * Mx;

		// Build bk

		T muk = lk.dot(sk)/M;
		Matrix<N, 1, T> HK =  (H * tk * - 1) - h - Mx.multTr(lk);
		Matrix<M, 1, T> GK =  cx - sk - Mx * tk ;
		Matrix<M, 1, T> TK =  em * sgk * muk - lk.emulCopy(sk);
		Matrix<N, 1, T> bk = HK + Mx.multTr(RK) * (GK - TK.edivCopy(lk));
		Matrix<N, 1, T> zk;

		SolverDispatch<S, N, mrmax, T>::call(Ak, bk, zko, tol, zk);

		Matrix<M, 1, T> Dlk = RK *(GK - TK.edivCopy(lk) - Mx * zk) * -1;
		Matrix<M, 1, T> Dsk = TK.edivCopy(lk) - RKI * Dlk;

		// Find max ak in (0,1]

		T alp_lk = computeAlp(Dlk, lk);
		T alp_sk = computeAlp(Dsk, sk);
		T alp = alp_lk > alp_sk ? alp_sk : alp_lk;

		tk += zk * alp;
		lk += Dlk * alp;
		sk += Dsk * alp;
		zko = zk;
	}

	return tk;
}
