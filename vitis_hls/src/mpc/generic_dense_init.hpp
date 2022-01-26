#pragma once

#include "generic_dense_defaults.hpp"

#define MPC_NAME_STR __str_name(MPC_NAME)
#define __str_name(x) #x

extern const float __init_A[MPC_N*MPC_N];
extern const float __init_B[MPC_N*MPC_M];

extern const float __init_Acal[MPC_N*MPC_L*MPC_N];
extern const float __init_Hcal[MPC_M*MPC_L*MPC_M*MPC_L];
extern const float __init_h_base[MPC_M*MPC_L*MPC_N];

extern const float __init_Mx[MPC_V*MPC_M*MPC_L];
extern const float __init_umin[MPC_M];
extern const float __init_umax[MPC_M];
extern const float __init_xmin[MPC_N];
extern const float __init_xmax[MPC_N];
extern const float __init_Nxmin[MPC_N];
extern const float __init_Nxmax[MPC_N];
extern const float __init_cx[MPC_V];

extern const float __init_Lu[MPC_M*MPC_P];
extern const float __init_Lx[MPC_N*MPC_P];
