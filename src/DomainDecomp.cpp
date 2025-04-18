#include "DomainDecomp.hpp"
#include "Boid.hpp"
#include <mpi.h>

DomainDecomp::DomainDecomp(int worldRank, int worldSize,
                           const Vec3& globalMin, const Vec3& globalMax)
    : rank_(worldRank), size_(worldSize),
      globalMin_(globalMin), globalMax_(globalMax) {
    // 1D分割: X軸方向に等分
    float dx = (globalMax_.x - globalMin_.x) / static_cast<float>(size_);
    localMin_ = Vec3(globalMin_.x + rank_ * dx,
                     globalMin_.y,
                     globalMin_.z);
    localMax_ = Vec3(localMin_.x + dx,
                     globalMax_.y,
                     globalMax_.z);
    // 隣接ランク: ±Xのみ
    neighbors_[0] = (rank_ > 0 ? rank_ - 1 : MPI_PROC_NULL);
    neighbors_[1] = (rank_ < size_ - 1 ? rank_ + 1 : MPI_PROC_NULL);
    neighbors_[2] = neighbors_[3] = neighbors_[4] = neighbors_[5] = MPI_PROC_NULL;
}

bool DomainDecomp::inLocal(const Vec3& pos) const {
    return (pos.x >= localMin_.x && pos.x < localMax_.x &&
            pos.y >= localMin_.y && pos.y < localMax_.y &&
            pos.z >= localMin_.z && pos.z < localMax_.z);
}

void DomainDecomp::exchangeBoundary(std::vector<Boid*>& boids) {
    // 1) 送信リストを作成
    std::vector<Boid*> sendLeft, sendRight;
    for (auto it = boids.begin(); it != boids.end();) {
        Boid* b = *it;
        const Vec3& p = b->getPosition();
        if (p.x < localMin_.x && neighbors_[0] != MPI_PROC_NULL) {
            sendLeft.push_back(b);
            it = boids.erase(it);
        } else if (p.x >= localMax_.x && neighbors_[1] != MPI_PROC_NULL) {
            sendRight.push_back(b);
            it = boids.erase(it);
        } else {
            ++it;
        }
    }
    // 2) シリアライズ
    auto pack = [](const std::vector<Boid*>& vec) {
        std::vector<float> buf;
        buf.reserve(vec.size() * 6);
        for (auto b : vec) {
            const Vec3& p = b->getPosition();
            const Vec3& v = b->getVelocity();
            buf.push_back(p.x); buf.push_back(p.y); buf.push_back(p.z);
            buf.push_back(v.x); buf.push_back(v.y); buf.push_back(v.z);
        }
        return buf;
    };
    std::vector<float> bufLeft  = pack(sendLeft);
    std::vector<float> bufRight = pack(sendRight);

    int left  = neighbors_[0];
    int right = neighbors_[1];
    int sendCountLeft  = static_cast<int>(sendLeft.size());
    int sendCountRight = static_cast<int>(sendRight.size());
    int recvCountRight = 0, recvCountLeft = 0;

    // 3) カウント交換
    MPI_Sendrecv(&sendCountLeft,  1, MPI_INT, left,  0,
                 &recvCountRight, 1, MPI_INT, right, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&sendCountRight, 1, MPI_INT, right, 1,
                 &recvCountLeft,  1, MPI_INT, left,  1,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // 4) データ交換
    std::vector<float> recvRight(recvCountRight * 6);
    std::vector<float> recvLeft (recvCountLeft  * 6);
    MPI_Sendrecv(bufLeft.data(),  sendCountLeft * 6,  MPI_FLOAT, left,  2,
                 recvRight.data(), recvCountRight * 6, MPI_FLOAT, right, 2,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Sendrecv(bufRight.data(), sendCountRight * 6, MPI_FLOAT, right, 3,
                 recvLeft.data(),  recvCountLeft  * 6, MPI_FLOAT, left,  3,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // 5) 受信データからBoidを再構築
    auto unpack = [&](const std::vector<float>& buf, int count) {
        for (int i = 0; i < count; ++i) {
            int idx = i * 6;
            Vec3 p(buf[idx], buf[idx+1], buf[idx+2]);
            Vec3 v(buf[idx+3], buf[idx+4], buf[idx+5]);
            boids.push_back(new Boid(p, v));
        }
    };
    unpack(recvRight, recvCountRight);
    unpack(recvLeft,  recvCountLeft);
}