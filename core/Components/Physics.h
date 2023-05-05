#pragma once
#include "BaseComponent.h"
#include "HexVector.h"
#include "box2d/box2d.h"
#include "HexAction.h"

class Physics : BaseComponent {
    private:
        static Physics* instance;
        b2World* world;
        struct ContactListener : public b2ContactListener {
            void BeginContact(b2Contact* contact) override;
            void EndContact(b2Contact* contact) override;
        } contactListener;
        struct RaycastListener : public b2RayCastCallback {
            std::vector<b2Fixture*> objs;
            float maxFraction;
            bool multiple;
            float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;
        } raycastListener;
    public:
        static void refl();
        static Physics& main();
        static GameObject& Raycast(Vector2f a, Vector2f b, float maxFraction);
        static void RaycastAll(Vector2f a, Vector2f b, float maxFraction, HexAction callback);
        bool useConfig = true;
        float deltaTime;
        Vector2f gravity = { 0, -10 };
        bool doSleep = true;
        int velocityIterations = 10;
        int positionIterations = 10;
        void Awake();
        void AfterDraw();
        void OnDestroy();
        void ApplyConfig();
        b2World& World();
};
