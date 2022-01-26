#include "mpc/systems/hls_generic_dense.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>

#include "mpc/mpc_dense.hpp"
#include "mpc/generic_dense_init.hpp"
#include "mpc/generic_dense_cosim.hpp"

int main()
{
	auto A = Matrix<N,N>(__init_A);
	auto B = Matrix<N,M>(__init_B);
	auto x = Matrix<N,1>(__cosim_x0[0].data());
	auto u = Matrix<M,1>(0.0);

	// Set up IO

	std::ofstream outFile("../../sim_" MPC_NAME_STR ".dat", std::ios::out | std::ios::binary);

	if(!outFile)
	{
	      std::cout << "Cannot open file!" << std::endl;
	      return EXIT_FAILURE;
	}

	std::cout << std::setprecision(2) << std::fixed;
	std::cerr << std::setprecision(2) << std::fixed;

	// Run iterations

	float total_mse_x = 0;
	float total_mse_u = 0;

	for(int i = 0; i < __cosim_iters; ++i)
	{
		// Simulate control and system

#if MPC_TRACK_REF
		auto y_ref = Matrix<P,1>(__cosim_yref[i].data());
		u = hls_main(x, y_ref, true);
#else
		u = hls_main(x);
#endif

		x = A * x + B * u;

		// Log values

		outFile << "X ";
		x.printLine(outFile);
		outFile << "U ";
		u.printLine(outFile);

		// Compare against reference values

		float iter_mse_x = x.mse(__cosim_x[i]);
		float iter_mse_u = u.mse(__cosim_u[i]);

		if(iter_mse_x >= 0.01 || iter_mse_u >= 0.01)
		{
			std::cerr << "[!] WARNING: In iteration " << (i + 1) << ": ";

			if(iter_mse_x >= 0.01)
			{
				std::cerr << "MSE_x = " << (iter_mse_x*100) << "%";

				if(iter_mse_u >= 0.01)
				{
					std::cerr << ", MSE_u = " << (iter_mse_u*100) << "%";
				}
			}
			else
			{
				std::cerr << "MSE_u = " << (iter_mse_u*100) << "%";
			}

			std::cerr << std::endl;
		}

		total_mse_x += iter_mse_x / __cosim_iters;
		total_mse_u += iter_mse_u / __cosim_iters;
	}

	// Check global MSE

	std::cout << std::endl;
	std::cout << "- MSE_x: " << (total_mse_x*100) << "%" << std::endl;
	std::cout << "- MSE_u: " << (total_mse_u*100) << "%" << std::endl;
	std::cout << std::endl;

	return (total_mse_x < 0.01 && total_mse_u < 0.01) ? EXIT_SUCCESS : EXIT_FAILURE;
}
