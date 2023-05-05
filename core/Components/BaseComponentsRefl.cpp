#include "HexLib.h"

void HexEngine::ReflectBaseComponents() {
    Vector2f::refl();
    Vector2i::refl();
    Vector3f::refl();
    Vector3i::refl();
    Vector4f::refl();
    Vector4i::refl();
    BaseComponent::refl();
    Transform::refl();
    Window::refl();
    Events::refl();
    Renderer::refl();
    Image::refl();
    Enums::refl();
    Engine::refl();
    Button::refl();
    Math::refl();
    HexAction::refl();
    Audio::refl();
    Text::refl();
    Physics::refl();
    Collider::refl();
    BoxCollider::refl();
    CircleCollider::refl();
    Sprite::refl();
    Camera::refl();
    RigidBody::refl();
}

void Vector2f::refl() {
    REG_COMP(Vector2f);
    mgr.AddMethod(std::function([](Vector2f* self, float x, float y) { self->x = x; self->y = y; }), "__ctor");
    mgr.AddMethod(std::function([](Vector2f* self) -> std::string {
        std::stringstream ss;
        ss << "[" << self->x << ", " << self->y << "]";
        return ss.str();
    }), "__tostring");
    REG_FIELD(x);
    REG_FIELD(y);
}

void Vector2i::refl() {
    REG_COMP(Vector2i);
    mgr.AddMethod(std::function([](Vector2i* self, int x, int y) { self->x = x; self->y = y; }), "__ctor");
    mgr.AddMethod(std::function([](Vector2i* self) -> std::string {
        std::stringstream ss;
        ss << "[" << self->x << ", " << self->y << "]";
        return ss.str();
    }), "__tostring");
    REG_FIELD(x);
    REG_FIELD(y);
}

void Vector3f::refl() {
    REG_COMP(Vector3f);
    mgr.AddMethod(std::function([](Vector3f* self, float x, float y, float z) { self->x = x; self->y = y; self->z = z; }), "__ctor");
    mgr.AddMethod(std::function([](Vector3f* self) -> std::string {
        std::stringstream ss;
        ss << "[" << self->x << ", " << self->y << ", " << self->z << "]";
        return ss.str();
    }), "__tostring");
    REG_FIELD(x);
    REG_FIELD(y);
    REG_FIELD(z);
}

void Vector3i::refl() {
    REG_COMP(Vector3i);
    mgr.AddMethod(std::function([](Vector3i* self, int x, int y, int z) { self->x = x; self->y = y; self->z = z; }), "__ctor");
    mgr.AddMethod(std::function([](Vector3i* self) -> std::string {
        std::stringstream ss;
        ss << "[" << self->x << ", " << self->y << ", " << self->z << "]";
        return ss.str();
    }), "__tostring");
    REG_FIELD(x);
    REG_FIELD(y);
    REG_FIELD(z);
}

void Vector4f::refl() {
    REG_COMP(Vector4f);
    mgr.AddMethod(std::function([](Vector4f* self, float x, float y, float z, float w) { self->x = x; self->y = y; self->z = z; self->w = w; }), "__ctor");
    mgr.AddMethod(std::function([](Vector4f* self) -> std::string {
        std::stringstream ss;
        ss << "[" << self->x << ", " << self->y << ", " << self->z << ", " << self->w << "]";
        return ss.str();
    }), "__tostring");
    REG_FIELD(x);
    REG_FIELD(y);
    REG_FIELD(z);
    REG_FIELD(w);
}

void Vector4i::refl() {
    REG_COMP(Vector4i);
    mgr.AddMethod(std::function([](Vector4i* self, int x, int y, int z, int w) { self->x = x; self->y = y; self->z = z; self->w = w; }), "__ctor");
    mgr.AddMethod(std::function([](Vector4i* self) -> std::string {
        std::stringstream ss;
        ss << "[" << self->x << ", " << self->y << ", " << self->z << ", " << self->w << "]";
        return ss.str();
    }), "__tostring");
    REG_FIELD(x);
    REG_FIELD(y);
    REG_FIELD(z);
    REG_FIELD(w);
    REG_METHOD(color);
}

void BaseComponent::refl() {
    using REG_CLASS = BaseComponent;
    auto& mgr = ReflMgr::Instance();
    mgr.AddClass<REG_CLASS>();
    REG_FIELD(name);
    REG_FIELD(enabled);
    mgr.AddAliasClass("Component", "BaseComponent");
    mgr.RawAddField(TypeID::get<BaseComponent>(), TypeID::get<GameObject&>(), "gameObject", std::function([](ObjectPtr instance) -> decltype(auto) {
        return ObjectPtr(TypeID::get<GameObject>(), (void*)instance.As<BaseComponent>().gameObject);
    }));
}

void Transform::refl() {
    REG_COMP(Transform);
    REG_FIELD(position);
    REG_FIELD(angle);
    REG_FIELD(size);
    REG_FIELD(pivot);
    REG_METHOD(worldPos);
    REG_METHOD(worldRot);
    REG_METHOD(Parent);
    REG_METHOD(ChildCount);
    REG_METHOD(GetChild);
}
