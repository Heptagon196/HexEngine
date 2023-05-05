#include "ExampleModule.h"

void ExampleModule::refl() {
    REG_COMP(ExampleModule);
}

void ExampleModule::Start() {
    std::cout << "Example module added" << std::endl;
}
