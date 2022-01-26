#pragma once

#include "Matrix.hpp"
#include "pdip.hpp"
#include "mpc_constraints.hpp"

/*!
@file   mpc_dense.hpp
*/

/*!
@brief  MPC dense implementation. Written considering a future HLS implementation
@tparam solver  Solve method to use for quadratic problem
@tparam constraints Type of constraints of the system
@tparam L   TODO
@tparam use_yref    true if yref wil be used. false as default
@tparam qpiter  Number of iterations for QP algorithm. By default, is 20
@tparam tol     Tolerance magnitude order. 1e-9 is used by default
@tparam N
@tparam M
@tparam P
@tparam Q
@tparam T       Data type
@param  A
@param  B
@param  C
@param  Acal
@param  Bcal
@param  Hcal
@param  Mx
@param  Omg
@param  nxmin
@param  nxmax
@param  xmin
@param  xmax
@param  umin
@param  umax
@param  yref
@param  x       States of the system
@param  u       Input values for system
*/
template<
	Solvers solver,
	MpcConstraints constraints,
	int L,
	bool track_ref = false,
	int qpiter = 20,
	int tol = -9,
	int N, int M, int V, typename T = float // automatically deduced from input arguments
>
void mpc_dense
(
	const Matrix<N,N,T> &AL,
	const Matrix<N*L,N,T> &Acal, const Matrix<M*L,M*L,T> &Hcal, const Matrix<V,M*L,T> &Mx,
	const Matrix<M,1,T> &umin, const Matrix<M,1,T> &umax, const Matrix<M,1,T> &uinfy,
	const Matrix<N,1,T> &xmin, const Matrix<N,1,T> &xmax, const Matrix<N,1,T> &xinfy,
	const Matrix<N,1,T> &Nxmin, const Matrix<N,1,T> &Nxmax,
	const Matrix<M*L,N,T> &h_base,
	Matrix<V,1,T> &cx, Matrix<N,1,T> &x, Matrix<M,1,T> &u
)
{
	// Read input vector

	Matrix<N,1,T> x0nau(x - xinfy);

	// Vector of cost

	Matrix<M*L,1,T> h = h_base * x0nau;

	// Set up the constraints vector

	updateConstraintsVector<constraints, track_ref, N, M, L>(
		AL, Acal, x0nau,
		umin, umax, uinfy,
		xmin, xmax, xinfy,
		Nxmin, Nxmax,
		cx
	);

	// Solve QP problem

	static const T tol_f = pow(10.0, tol);
	Matrix<M*L,1,T> unau = pdip<solver, qpiter, 20>(Hcal, h, Mx, cx, tol_f);

	// Write output vector

	for(int i = 0; i < M; ++i)
	{
		u(i,0) = unau(i,0) + uinfy(i,0);
	}
}
