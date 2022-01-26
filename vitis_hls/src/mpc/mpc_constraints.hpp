#pragma once

#include "Matrix.hpp"

/*!
	@file mpc_constraints.hpp
*/

/*! Possible MPC constraints combinations */
enum MpcConstraints
{
	INPUT            = 1,                         /*!< Only input constraints */
	STATE            = 2,                         /*!< Only state related constraints */
	FINALSTATE       = 4,                         /*!< Final state constraints */
	INPUT_STATE      = INPUT | STATE,             /*!< Input and state constraints */
	INPUT_FINALSTATE = INPUT | FINALSTATE,        /*!< Input and final state constraints */

	STATE_INPUT      = STATE | INPUT,             /*!< State and input state constraints */
	STATE_FINALSTATE = STATE | FINALSTATE,        /*!< State and final state constraints */

	FINALSTATE_INPUT = FINALSTATE | INPUT,        /*!< Final state and input constraints */
	FINALSTATE_STATE = FINALSTATE | STATE,        /*!< Final state and state constraints */

	CONSTRAINT_ALL   = FINALSTATE | STATE | INPUT /*!< Final state, state and input constraints */
};

template<bool enable, bool track_ref, int N, int M, int L, int V, typename T = float>
struct MpcConstraintsImpl
{ };

/*!
	@brief Updates the vector with constraint information for the MPC problem. Updates only the elements that change
	depending on the current value of x.

	@tparam constraints Values to constraint
	@tparam track_ref   Toggle reference tracking
	@tparam N           Size of the state vector, x
	@tparam M           Size of the input vector, u
	@tparam L           Prediction horizon
	@tparam V           Length of the constraints vector, cx
	@tparam T           Matrix elements type

	@param  AL          Matrix A^L, derived from system definition
	@param  Acal        Matrix Acal, derived from system definition
	@param  x0nau       Difference between the current x value and stationary state target
	@param  umin        Minimum input constraint
	@param  umax        Maximum input constraint
	@param  uinfy       Stationary state target input
	@param  xmin        Minimum state constraint
	@param  xmax        Maximum state constraint
	@param  xinfy       Stationary state target
	@param  Nxmin       Minimum final state constraint
	@param  Nxmax       Maximum final state constraint
	@param  cx          Constraints vector to be updated
*/

template<MpcConstraints constraints = INPUT, bool track_ref, int N, int M, int L, int V, typename T = float>
void updateConstraintsVector
(
	const Matrix<N,N,T> &AL, const Matrix<N*L,N,T> &Acal, const Matrix<N,1,T> &x0nau,
	const Matrix<M,1,T> &umin, const Matrix<M,1,T> &umax, const Matrix<M,1,T> &uinfy,
	const Matrix<N,1,T> &xmin, const Matrix<N,1,T> &xmax, const Matrix<N,1,T> &xinfy,
	const Matrix<N,1,T> &Nxmin, const Matrix<N,1,T> &Nxmax,
	Matrix<V,1,T> &cx
)
{
	#pragma HLS INLINE

	using InputImpl = MpcConstraintsImpl<!!(constraints & INPUT), track_ref, N, M, L, V, T>;
	using StateImpl = MpcConstraintsImpl<!!(constraints & STATE), track_ref, N, M, L, V, T>;
	using FinalStateImpl = MpcConstraintsImpl<!!(constraints & FINALSTATE), track_ref, N, M, L, V, T>;

	// Offsets for cx

	constexpr int StateOffset = FinalStateImpl::FINALSTATE_SIZE;
	constexpr int InputOffset = StateOffset + StateImpl::STATE_SIZE;
	static_assert(InputOffset + InputImpl::INPUT_SIZE == V, "Constraints vector length mismatch");

	// Update the vector elements

	FinalStateImpl::template constraintFinalState<0>(AL, x0nau, Nxmin, Nxmax, xinfy, cx);
	StateImpl::template constraintState<StateOffset>(Acal, x0nau, xmin, xmax, xinfy, cx);
	InputImpl::template constraintInput<InputOffset>(umin, umax, uinfy, cx);
}

template<bool track_ref, int N, int M, int L, int V, typename T>
struct MpcConstraintsImpl<false, track_ref, N, M, L, V, T>
{
	static constexpr int INPUT_SIZE = 0;
	static constexpr int STATE_SIZE = 0;
	static constexpr int FINALSTATE_SIZE = 0;

	template<int begin>
	static void constraintInput
	(
		const Matrix<M,1,T>&, const Matrix<M,1,T>&, const Matrix<M,1,T>&,
		Matrix<V,1,T>&
	)
	{
		// No-op, constraint disabled
	}

	template<int begin>
	static void constraintState
	(
		const Matrix<N*L,N,T>&, const Matrix<N,1,T>&,
		const Matrix<N,1,T>&, const Matrix<N,1,T>&, const Matrix<N,1,T>&,
		Matrix<V,1,T>&
	)
	{
		// No-op, constraint disabled
	}

	template<int begin>
	static void constraintFinalState
	(
		const Matrix<N,N,T>&, const Matrix<N,1,T>&,
		const Matrix<N,1,T>&, const Matrix<N,1,T>&, const Matrix<N,1,T>&,
		Matrix<V,1,T>&
	)
	{
		// No-op, constraint disabled
	}
};

template<int N, int M, int L, int V, typename T>
struct MpcConstraintsImpl<true, false, N, M, L, V, T>
{
	static constexpr int INPUT_SIZE = 2*L*M;
	static constexpr int STATE_SIZE = 2*L*N;
	static constexpr int FINALSTATE_SIZE = 2*N;

	template<int begin>
	static void constraintInput
	(
		const Matrix<M,1,T>&, const Matrix<M,1,T>&, const Matrix<M,1,T>&,
		Matrix<V,1,T>&
	)
	{
		// No-op, doesn't depend on current state
	}

	template<int begin>
	static void constraintState
	(
		const Matrix<N*L,N,T> &Acal, const Matrix<N,1,T> &x0nau,
		const Matrix<N,1,T> &xmin, const Matrix<N,1,T> &xmax, const Matrix<N,1,T>&,
		Matrix<V,1,T> &cx
	)
	{
		auto Acal_mul = Acal * x0nau;

		for(int i = begin, j = 0, k = 0; i < begin+N*L; ++i, ++j, ++k)
		{
			if(k >= N)
			{
				k = 0;
			}

			cx(i,0) = xmax(k,0) - Acal_mul(j,0);
		}

		for(int i = begin+N*L, j = 0, k = 0; i < begin+2*N*L; ++i, ++j, ++k)
		{
			if(k >= N)
			{
				k = 0;
			}

			cx(i,0) = Acal_mul(j,0) - xmin(k,0);
		}
	}

	template<int begin>
	static void constraintFinalState
	(
		const Matrix<N,N,T> &AL, const Matrix<N,1,T> &x0nau,
		const Matrix<N,1,T> &Nxmin, const Matrix<N,1,T> &Nxmax, const Matrix<N,1,T>&,
		Matrix<V,1,T> &cx
	)
	{
		auto AL_mul = AL * x0nau;

		for(int i = begin, j = 0; i < begin+N; ++i, ++j)
		{
			cx(i,0) = Nxmax(j,0) - AL_mul(j,0);
		}

		for(int i = begin+N, j = 0; i < begin+2*N; ++i, ++j)
		{
			cx(i,0) = AL_mul(j,0) - Nxmin(j,0);
		}
	}
};

template<int N, int M, int L, int V, typename T>
struct MpcConstraintsImpl<true, true, N, M, L, V, T>
{
	static constexpr int INPUT_SIZE = 2*L*M;
	static constexpr int STATE_SIZE = 2*L*N;
	static constexpr int FINALSTATE_SIZE = 2*N;

	template<int begin>
	static void constraintInput
	(
		const Matrix<M,1,T> &umin, const Matrix<M,1,T> &umax, const Matrix<M,1,T> &uinfy,
		Matrix<V,1,T> &cx
	)
	{
		for(int i = begin, j = 0; i < begin+M; ++i, ++j)
		{
			cx(i,0) = umax(j,0) - uinfy(j,0);
		}

		for(int i = begin+L*M, j = 0; i < begin+M*(L+1); ++i, ++j)
		{
			cx(i,0) = uinfy(j,0) - umin(j,0);
		}

		cx.template repeat<begin, M, L>();
		cx.template repeat<begin + L*M, M, L>();
	}

	template<int begin>
	static void constraintState
	(
		const Matrix<N*L,N,T> &Acal, const Matrix<N,1,T> &x0nau,
		const Matrix<N,1,T> &xmin, const Matrix<N,1,T> &xmax, const Matrix<N,1,T> &xinfy,
		Matrix<V,1,T> &cx
	)
	{
		auto Acal_mul = Acal * x0nau;

		for(int i = begin, j = 0, k = 0; i < begin+N*L; ++i, ++j, ++k)
		{
			if(k >= N)
			{
				k = 0;
			}

			cx(i,0) = xmax(k,0) - xinfy(k,0) - Acal_mul(j,0);
		}

		for(int i = begin+N*L, j = 0, k = 0; i < begin+2*N*L; ++i, ++j, ++k)
		{
			if(k >= N)
			{
				k = 0;
			}

			cx(i,0) = xinfy(k,0) - xmin(k,0) + Acal_mul(j,0);
		}
	}

	template<int begin>
	static void constraintFinalState
	(
		const Matrix<N,N,T> &AL, const Matrix<N,1,T> &x0nau,
		const Matrix<N,1,T> &Nxmin, const Matrix<N,1,T> &Nxmax, const Matrix<N,1,T> &xinfy,
		Matrix<V,1,T> &cx
	)
	{
		auto AL_mul = AL * x0nau;

		for(int i = begin, j = 0; i < begin+N; ++i, ++j)
		{
			cx(i,0) = Nxmax(j,0) - xinfy(j,0) - AL_mul(j,0);
		}

		for(int i = begin+N, j = 0; i < begin+2*N; ++i, ++j)
		{
			cx(i,0) = xinfy(j,0) - Nxmin(j,0) + AL_mul(j,0);
		}
	}
};
