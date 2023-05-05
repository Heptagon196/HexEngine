#pragma once
#include "BaseComponent.h"
#include "HexVector.h"
#include "Physics.h"
#include "box2d/box2d.h"

class RigidBody : BaseComponent {
    private:
        b2Body* body;
        Physics* physics;
        b2Fixture* fixture = nullptr;
    public:
        static void refl();
        void UpdateCollider();
        void SetVelocity(Vector2f v);
        void ApplyForceCenter(Vector2f f);
        void ApplyForce(Vector2f f, Vector2f pos);
        void SetPosition(Vector2f pos);
        void SetRotation(float angle);
        float Mass();
        Vector2f WorldCenter();
        void SetEnabled(bool enable);
        int type = 2; // 0: static 1: kinematic 2: dynamic
        Vector2f velocity;
        bool allowSleep = true;
        bool fixedRotation = true;
        bool bullet = false;
        float gravityScale = 1;
        float friction = 0.3;
        float elasticity = 0.3;
        float elasticityThreshold = 0.1;
        float density = 1;
        bool isTrigger = false;
        void Start();
        void PhysicsUpdate();
        void OnDestroy();
};
