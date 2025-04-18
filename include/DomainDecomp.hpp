#pragma once

#include <vector>
#include <array>
#include "Vec3.hpp"

class Boid;  // 前方宣言

class DomainDecomp {
public:
    // worldRank: MPIランク番号, worldSize: ランク数
    // globalMin/Max: 全体空間の境界
    DomainDecomp(int worldRank, int worldSize,
                 const Vec3& globalMin, const Vec3& globalMax);

    // posがこのランクのローカル領域内にあるか判定
    bool inLocal(const Vec3& pos) const;

    // 境界セルにいるBoidを隣接ランクと交換
    void exchangeBoundary(std::vector<Boid*>& boids);

private:
    // 隣接ランクを±X,±Y,±Zの順で保持
    void computeNeighbors();

    int rank_;
    int size_;
    Vec3 globalMin_, globalMax_;
    Vec3 localMin_, localMax_;
    std::array<int,6> neighbors_;
};

