#include "control_flow.h"


env::VariantType control::ReturnException::value() const {
    return returnValue;
}

const char* control::ReturnException::what() const noexcept {
    return "Return statement";
}

control::ReturnException::ReturnException(env::VariantType value) : returnValue(std::move(value)) {}

const char *control::BreakException::what() const noexcept {
    return "Break statement";
}

const char *control::ContinueException::what() const noexcept {
    return "Continue statement";
}
