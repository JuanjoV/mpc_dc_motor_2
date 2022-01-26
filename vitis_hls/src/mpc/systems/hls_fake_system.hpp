#pragma once

#include <mpc/Matrix.hpp>
#include <mpc/generic_dense_defaults.hpp>

constexpr int N = MPC_N;
constexpr int M = MPC_M;

extern Matrix<N,1> hls_main(Matrix<N,1> x, Matrix<M,1> u);
