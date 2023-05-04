#include "Transform.h"

Transform& Transform::Parent() {
    return *parent;
}

int Transform::ChildCount() {
    return childs.size();
}

Transform& Transform::GetChild(int idx) {
    return *childs[idx];
}
