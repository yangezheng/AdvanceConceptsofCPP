#include "token.h"

namespace sql {
Token::Token(token_type value) : value_(value) {}

auto Token::value() const -> token_type {
    return this->value_;
}

} // namespaces sql
