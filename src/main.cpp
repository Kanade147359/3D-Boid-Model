#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <cstdlib>
#include "Simulator.hpp"

int main(int argc, char* argv[]) {
    // MPI 初期化
    MPI_Init(&argc, &argv);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // OpenMP スレッド数設定 (環境変数優先)
    #pragma omp parallel
    {
        #pragma omp master
        {
            int nthreads = omp_get_num_threads();
            if (world_rank == 0) {
                std::cout << "MPI world size: " << world_size << std::endl;
                std::cout << "OpenMP threads: " << nthreads << std::endl;
            }
        }
    }

    // シミュレータ設定読み込み
    std::string config_file = "configs/default.yaml";
    if (argc > 1) {
        config_file = argv[1];
    }

    try {
        Simulator sim(config_file, world_rank, world_size);
        sim.run();
    } catch (const std::exception& e) {
        std::cerr << "[Rank " << world_rank << "] Error: " << e.what() << std::endl;
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    // MPI 終了
    MPI_Finalize();
    return EXIT_SUCCESS;
}