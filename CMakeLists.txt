cmake_minimum_required(VERSION 3.15)
project(Boids3D_HPC LANGUAGES CXX)

# C++ 標準と最適化フラグ
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
# HPC 向け最適化例
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native")

# MPI, OpenMP, GoogleTest の検出
find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)
find_package(GTest REQUIRED)

# インクルードディレクトリ
include_directories(
  ${MPI_INCLUDE_PATH}
  include
)

# コアライブラリ: src/, parallel/, sim/ をまとめてビルド
file(GLOB_RECURSE CORE_SRC
  src/*.cpp
  parallel/*.cpp
  sim/*.cpp
)
add_library(boids3d_lib ${CORE_SRC})
# MPI と OpenMP をリンク
target_link_libraries(boids3d_lib PRIVATE MPI::MPI_CXX OpenMP::OpenMP_CXX)

target_include_directories(boids3d_lib PUBLIC include)

# メイン実行バイナリ
add_executable(boids3d_main src/main.cpp)
target_link_libraries(boids3d_main PRIVATE boids3d_lib)

# テスト設定
enable_testing()
add_executable(test_boid tests/test_boid.cpp)
target_link_libraries(test_boid PRIVATE boids3d_lib GTest::gtest_main)
add_test(NAME BoidTest COMMAND test_boid)

# インストール設定
install(TARGETS boids3d_main boids3d_lib
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
