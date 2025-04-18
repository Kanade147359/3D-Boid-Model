#pragma once

#include <vector>
#include <tuple>
#include <unordered_map>
#include "Vec3.hpp"

class Boid;  // 前方宣言

class NeighborSearch {
public:
    // domainMin/Max: 検索対象全体の境界, radius: 作用半径
    NeighborSearch(const Vec3& domainMin,
                   const Vec3& domainMax,
                   float radius);

    // 全Boidリストからセルリストを構築
    void build(const std::vector<Boid*>& boids);

    // ボイドbの近傍をoutNeighborsに返す
    void query(const Boid& b, std::vector<Boid*>& outNeighbors) const;

private:
    using CellIdx = std::tuple<int,int,int>;

    // 位置→セルインデックス変換
    CellIdx posToIdx(const Vec3& pos) const;

    Vec3 cellSize_;
    std::unordered_map<CellIdx, std::vector<Boid*>> grid_;
};
