#pragma once

#include "generic_dense_defaults.hpp"

#define MPC_NAME_STR __str_name(MPC_NAME)
#define __str_name(x) #x

extern const float __init_A[MPC_N*MPC_N];
extern const float __init_B[MPC_N*MPC_M];
