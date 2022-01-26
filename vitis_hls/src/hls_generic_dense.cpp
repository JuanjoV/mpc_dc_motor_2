#include "mpc/systems/hls_generic_dense.hpp"

#include "mpc/mpc_dense.hpp"
#include "mpc/generic_dense_init.hpp"

#if !MPC_TRACK_REF
Matrix<M,1> hls_main(Matrix<N,1> x)
#else
Matrix<M,1> hls_main(Matrix<N,1> x, Matrix<P,1> y_ref)
#endif
{
#pragma hls interface mode=s_axilite port=x
#pragma hls interface mode=s_axilite port=return
	static const auto A = Matrix<N,N>(__init_A);
	static const auto AL = A.pow(L);

	static const auto Acal = Matrix<N*L,N>(__init_Acal);
	static const auto Hcal = Matrix<M*L,M*L>(__init_Hcal);
	static const auto h_base = Matrix<M*L,N>(__init_h_base);

	static const auto Mx = Matrix<V,M*L>(__init_Mx);
	static const auto umin = Matrix<M,1>(__init_umin);
	static const auto umax = Matrix<M,1>(__init_umax);
	static const auto xmin = Matrix<N,1>(__init_xmin);
	static const auto xmax = Matrix<N,1>(__init_xmax);
	static const auto Nxmin = Matrix<N,1>(__init_Nxmin);
	static const auto Nxmax = Matrix<N,1>(__init_Nxmax);
	static auto cx = Matrix<V,1>(__init_cx);

#if MPC_TRACK_REF
	static const auto Lx = Matrix<N,P>(__init_Lx);
	static const auto Lu = Matrix<M,P>(__init_Lu);

	static auto xinfy = Lx * y_ref;
	static auto uinfy = Lu * y_ref;
#else
	static const auto xinfy = Matrix<N,1>(0.0);
	static const auto uinfy = Matrix<M,1>(0.0);
#endif

	Matrix<M,1> u;

	mpc_dense<SOLVER, CONSTRAINTS, L, TRACK_REF, QP_ITER, TOL>(
		AL,
		Acal, Hcal, Mx,
		umin, umax, uinfy,
		xmin, xmax, xinfy,
		Nxmin, Nxmax,
		h_base,
		cx, x, u
	);

	return u;
}
