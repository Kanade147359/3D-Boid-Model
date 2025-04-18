#pragma once

#include <vector>
#include "Vec3.hpp"

class Boid {
public:
    // コンストラクタ：初期位置・初速を設定
    Boid(const Vec3& position, const Vec3& velocity);

    // 外部からの力を加算
    void applyForce(const Vec3& force);

    // 近傍Boidリストを使って分離・整列・統合を計算
    void computeBehavior(const std::vector<Boid*>& neighbors);

    // 加速度を反映して速度・位置を更新
    void update(float dt);

    // 位置・速度の参照取得
    const Vec3& getPosition() const;
    const Vec3& getVelocity() const;

private:
    // 3つの振る舞いルール
    Vec3 separate(const std::vector<Boid*>& neighbors);
    Vec3 align   (const std::vector<Boid*>& neighbors);
    Vec3 cohesion(const std::vector<Boid*>& neighbors);

    Vec3 position_;
    Vec3 velocity_;
    Vec3 acceleration_;
    float maxSpeed_;
    float maxForce_;
};