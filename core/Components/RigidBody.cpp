#include "RigidBody.h"
#include "Math.h"
#include "../GameObject.h"
#include "Collider.h"

void RigidBody::refl() {
    REG_COMP(RigidBody);
    REG_FIELD(type);
    REG_FIELD(velocity);
    REG_FIELD(allowSleep);
    REG_FIELD(fixedRotation);
    REG_FIELD(bullet);
    REG_FIELD(gravityScale);
    REG_FIELD(friction);
    REG_FIELD(elasticity);
    REG_FIELD(elasticityThreshold);
    REG_FIELD(density);
    REG_FIELD(isTrigger);
    REG_METHOD(SetEnabled);
    REG_METHOD(UpdateCollider);
    REG_METHOD(SetVelocity);
    REG_METHOD(ApplyForce);
    REG_METHOD(ApplyForceCenter);
    REG_METHOD(WorldCenter);
    REG_METHOD(Mass);
    REG_METHOD(SetPosition);
    REG_METHOD(SetRotation);
    mgr.RawAddField(TypeID::get<GameObject>(), TypeID::get<RigidBody&>(), "rigidBody", std::function(
        [](ObjectPtr instance) {
            return ObjectPtr{ TypeID::get<RigidBody&>(), instance.As<GameObject>().GetComponent("RigidBody").GetPtr() };
        }
    ));
}

void RigidBody::SetPosition(Vector2f pos) {
    auto p = gameObject->transform.worldPos();
    auto orig = gameObject->transform.position;
    gameObject->transform.position = {orig.x + pos.x - p.x, orig.y + pos.y - p.y};
}

void RigidBody::SetRotation(float angle) {
    gameObject->transform.angle = angle - gameObject->transform.angle;
}

void RigidBody::SetVelocity(Vector2f v) {
    velocity = v;
}

float RigidBody::Mass() {
    return body->GetMass();
}

void RigidBody::ApplyForceCenter(Vector2f f) {
    body->ApplyForceToCenter({f.x, f.y}, true);
}

void RigidBody::ApplyForce(Vector2f f, Vector2f pos) {
    body->ApplyForce({f.x, f.y}, {pos.x, pos.y}, true);
}

Vector2f RigidBody::WorldCenter() {
    auto& p = body->GetWorldCenter();
    return {p.x, p.y};
}

void RigidBody::Start() {
    physics = &Physics::main();
    b2BodyDef def;
    auto& transform = gameObject->transform;
    auto pos = transform.worldPos();
    def.position = b2Vec2(pos.x, pos.y);
    if (type == 0) {
        def.type = b2_staticBody;
    } else if (type == 1) {
        def.type = b2_kinematicBody;
    } else if (type == 2) {
        def.type = b2_dynamicBody;
    }
    def.angle = Math::DEG2RAD(transform.angle);
    def.allowSleep = allowSleep;
    def.linearVelocity = b2Vec2(velocity.x, velocity.y);
    def.fixedRotation = fixedRotation;
    def.bullet = bullet;
    def.gravityScale = gravityScale;
    def.userData.pointer = (uintptr_t)gameObject;
    body = physics->World().CreateBody(&def);
    if (!gameObject->HasComponent("Collider")) {
        return;
    }
    UpdateCollider();
    if (!enabled) {
        SetEnabled(false);
    }
}

void RigidBody::SetEnabled(bool enable) {
    body->SetEnabled(enable);
    enabled = enable;
}

void RigidBody::UpdateCollider() {
    if (fixture != nullptr) {
        body->DestroyFixture(fixture);
    }
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &gameObject->GetComponent("Collider").As<Collider>().GetShape();
    fixtureDef.friction = friction;
    fixtureDef.restitution = elasticity;
    fixtureDef.restitutionThreshold = elasticityThreshold;
    fixtureDef.density = density;
    fixtureDef.isSensor = isTrigger;
    fixture = body->CreateFixture(&fixtureDef);
}

void RigidBody::PhysicsUpdate() {
    Transform& transform = gameObject->transform;
    auto orig = transform.worldPos();
    auto& pos = body->GetPosition();
    transform.position = { pos.x - orig.x + transform.position.x, pos.y - orig.y + transform.position.y };
    transform.angle += Math::RAD2DEG(body->GetAngle()) - transform.worldRot();
    auto& v = body->GetLinearVelocity();
    velocity = { v.x, v.y };
}

void RigidBody::Draw() {
    Transform& transform = gameObject->transform;
    auto orig = transform.worldPos();
    body->SetTransform({orig.x, orig.y}, Math::DEG2RAD(transform.worldRot()));
    body->SetLinearVelocity({velocity.x, velocity.y});
}

void RigidBody::OnDestroy() {
    body->DestroyFixture(fixture);
    physics->World().DestroyBody(body);
}
