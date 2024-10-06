#ifndef SPL_CONTROL_FLOW_H
#define SPL_CONTROL_FLOW_H

#include <exception>
#include <utility>

#include "environment.h"

namespace control {
    class ReturnException : public std::exception {
    public:
        explicit ReturnException(env::VariantType value);

        [[nodiscard]] env::VariantType value() const;
        [[nodiscard]] const char* what() const noexcept override;

    private:
        env::VariantType returnValue;
    };

    // todo: add "continue" and "break" exceptions
}

#endif //SPL_CONTROL_FLOW_H
