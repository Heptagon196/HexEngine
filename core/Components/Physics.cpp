#include "../HexEngine.h"
#include "../Utils/StringUtils.h"
#include "Physics.h"
#include "Engine.h"

void Physics::ContactListener::BeginContact(b2Contact* contact) {
    GameObject* objA = (GameObject*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    GameObject* objB = (GameObject*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
    objA->SendMessage("OnBeginContact", { ObjectPtr{ TypeID::get<GameObject&>(), objB } });
    objB->SendMessage("OnBeginContact", { ObjectPtr{ TypeID::get<GameObject&>(), objA } });
}

void Physics::ContactListener::EndContact(b2Contact* contact) {
    GameObject* objA = (GameObject*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    GameObject* objB = (GameObject*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
    objA->SendMessage("OnEndContact", { ObjectPtr{ TypeID::get<GameObject&>(), objB } });
    objB->SendMessage("OnEndContact", { ObjectPtr{ TypeID::get<GameObject&>(), objA } });
}

float Physics::RaycastListener::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) {
    if (maxFraction > 0 && fraction > maxFraction) {
        return 0;
    }
    objs.push_back(fixture);
    if (multiple) {
        return 1;
    }
    return 0;
}

Physics* Physics::instance = nullptr;

void Physics::refl() {
    REG_COMP(Physics);
    REG_STATIC_METHOD(main);
    REG_STATIC_METHOD(Raycast);
    REG_STATIC_METHOD(RaycastAll);
    REG_FIELD(useConfig);
    REG_FIELD(gravity);
    REG_FIELD(deltaTime);
    REG_FIELD(doSleep);
    REG_FIELD(velocityIterations);
    REG_FIELD(positionIterations);
    REG_METHOD(ApplyConfig);
    REG_METHOD(World);
}

Physics& Physics::main() {
    return *instance;
}

void Physics::Awake() {
    Engine::DontDestroyOnLoad(*gameObject);
    if (instance == nullptr) {
        instance = this;
    }
    if (useConfig) {
        HexEngine::GameConfig& cfg = HexEngine::HexEngine::Instance().config();
        gravity.x = cfg.Get<float>("gravityX", 0.0);
        gravity.y = cfg.Get<float>("gravityY", -10.0);
        doSleep = cfg.GetBool("doSleep", true);
        velocityIterations = cfg.Get<int>("velocityIterations", 10);
        positionIterations = cfg.Get<int>("positionIterations", 8);
        deltaTime = 1.0 / (float)HexEngine::HexEngine::Instance().Frame();
    }
    world = new b2World(b2Vec2(gravity.x, gravity.y));
    world->SetContactListener(&contactListener);
    ApplyConfig();
}

void Physics::AfterDraw() {
    world->Step(deltaTime, velocityIterations, positionIterations);
    world->ClearForces();
}

void Physics::OnDestroy() {
    world->SetContactListener(nullptr);
    delete world;
}

void Physics::ApplyConfig() {
    world->SetGravity(b2Vec2(gravity.x, gravity.y));
    world->SetAllowSleeping(doSleep);
}

b2World& Physics::World() {
    return *world;
}

GameObject& Physics::Raycast(Vector2f a, Vector2f b, float maxFraction) {
    auto& instance = Physics::main();
    instance.raycastListener.multiple = false;
    instance.raycastListener.maxFraction = maxFraction;
    instance.world->RayCast(&instance.raycastListener, {a.x, a.y}, {b.x, b.y});
    if (instance.raycastListener.objs.size() == 0) {
        static GameObject nullObj;
        nullObj.null = true;
        nullObj.name = "NULL";
        return nullObj;
    }
    GameObject* ret = (GameObject*)instance.raycastListener.objs[0]->GetBody()->GetUserData().pointer;
    instance.raycastListener.objs.clear();
    return *ret;
}

void Physics::RaycastAll(Vector2f a, Vector2f b, float maxFraction, HexAction callback) {
    auto& instance = Physics::main();
    instance.raycastListener.multiple = true;
    instance.raycastListener.maxFraction = maxFraction;
    instance.world->RayCast(&instance.raycastListener, {a.x, a.y}, {b.x, b.y});
    for (auto* fixture : instance.raycastListener.objs) {
        callback(ObjectPtr{ TypeID::get<GameObject&>(), (void*)fixture->GetBody()->GetUserData().pointer });
    }
    instance.raycastListener.objs.clear();
}
