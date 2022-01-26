#pragma once

#include "../mpc_dense.hpp"
#include "../generic_dense_defaults.hpp"

constexpr int N = MPC_N;
constexpr int M = MPC_M;
constexpr int P = MPC_P;
constexpr int L = MPC_L;
constexpr int V = MPC_V;

constexpr Solvers SOLVER = MPC_SOLVER;
constexpr MpcConstraints CONSTRAINTS = MPC_CONSTRAINTS;
constexpr bool TRACK_REF = MPC_TRACK_REF;
constexpr int QP_ITER = MPC_QP_ITER;
constexpr int TOL = MPC_TOL;

#if !MPC_TRACK_REF
extern Matrix<M,1> hls_main(Matrix<N,1> x);
#else
extern Matrix<M,1> hls_main(Matrix<N,1> x, Matrix<P,1> y_ref);
#endif
