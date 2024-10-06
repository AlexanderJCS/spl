#include "control_flow.h"


env::VariantType control::ReturnException::value() const {
    return returnValue;
}

const char* control::ReturnException::what() const noexcept {
    return "Return statement";
}

control::ReturnException::ReturnException(env::VariantType value) : returnValue(std::move(value)) {}
