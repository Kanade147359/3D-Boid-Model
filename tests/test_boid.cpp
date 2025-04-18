#include <gtest/gtest.h>
#include "Boid.hpp"
#include "Vec3.hpp"

// 初期化テスト
TEST(BoidInitialization, DefaultValues) {
    Vec3 initPos(0.0f, 0.0f, 0.0f);
    Vec3 initVel(1.0f, 2.0f, 3.0f);
    Boid b(initPos, initVel);
    EXPECT_EQ(b.getPosition(), initPos);
    EXPECT_EQ(b.getVelocity(), initVel);
}

// 隣接なしでのアップデート（等速直線運動）
TEST(BoidUpdate, NoNeighbors) {
    Vec3 initPos(0.0f, 0.0f, 0.0f);
    Vec3 initVel(2.0f, 0.0f, 0.0f);
    Boid b(initPos, initVel);
    std::vector<Boid*> empty;
    b.computeBehavior(empty);
    b.update(0.5f);
    Vec3 pos = b.getPosition();
    EXPECT_FLOAT_EQ(pos.x, 1.0f);
    EXPECT_FLOAT_EQ(pos.y, 0.0f);
    EXPECT_FLOAT_EQ(pos.z, 0.0f);
    Vec3 vel = b.getVelocity();
    EXPECT_FLOAT_EQ(vel.x, 2.0f);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
