#include "NeighborSearch.hpp"
#include "Boid.hpp"
#include <cmath>

NeighborSearch::NeighborSearch(const Vec3& domainMin,
                               const Vec3& domainMax,
                               float radius)
    : domainMin_(domainMin), domainMax_(domainMax),
      cellSize_(radius), nx_(0), ny_(0), nz_(0) {
    // セル数計算 (最小 1 セル保証)
    nx_ = std::max(1, static_cast<int>((domainMax_.x - domainMin_.x) / cellSize_.x) + 1);
    ny_ = std::max(1, static_cast<int>((domainMax_.y - domainMin_.y) / cellSize_.y) + 1);
    nz_ = std::max(1, static_cast<int>((domainMax_.z - domainMin_.z) / cellSize_.z) + 1);
}

void NeighborSearch::build(const std::vector<Boid*>& boids) {
    grid_.clear();
    for (auto b : boids) {
        auto idx = posToIdx(b->getPosition());
        grid_[idx].push_back(b);
    }
}

void NeighborSearch::query(const Boid& b, std::vector<Boid*>& outNeighbors) const {
    outNeighbors.clear();
    auto baseIdx = posToIdx(b.getPosition());
    float r2 = cellSize_.x * cellSize_.x;
    for (int dx = -1; dx <= 1; ++dx) {
        int ix = std::get<0>(baseIdx) + dx;
        if (ix < 0 || ix >= nx_) continue;
        for (int dy = -1; dy <= 1; ++dy) {
            int iy = std::get<1>(baseIdx) + dy;
            if (iy < 0 || iy >= ny_) continue;
            for (int dz = -1; dz <= 1; ++dz) {
                int iz = std::get<2>(baseIdx) + dz;
                if (iz < 0 || iz >= nz_) continue;
                CellIdx cell = std::make_tuple(ix, iy, iz);
                auto it = grid_.find(cell);
                if (it == grid_.end()) continue;
                for (auto neighbor : it->second) {
                    if (neighbor == &b) continue;
                    Vec3 diff = neighbor->getPosition() - b.getPosition();
                    float dist2 = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
                    if (dist2 < r2) {
                        outNeighbors.push_back(neighbor);
                    }
                }
            }
        }
    }
}

NeighborSearch::CellIdx NeighborSearch::posToIdx(const Vec3& pos) const {
    int ix = static_cast<int>((pos.x - domainMin_.x) / cellSize_.x);
    int iy = static_cast<int>((pos.y - domainMin_.y) / cellSize_.y);
    int iz = static_cast<int>((pos.z - domainMin_.z) / cellSize_.z);
    ix = std::max(0, std::min(ix, nx_-1));
    iy = std::max(0, std::min(iy, ny_-1));
    iz = std::max(0, std::min(iz, nz_-1));
    return std::make_tuple(ix, iy, iz);
}
