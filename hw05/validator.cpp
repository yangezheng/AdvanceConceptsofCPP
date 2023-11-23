#include "validator.h"

#include <variant>
#include <vector>

#include "token.h"

namespace sql {

bool SqlValidator::is_valid() const {
    // TODO: Implement this
    return std::holds_alternative<state::Valid>(state_);
}

// void SqlValidator::handle(const Token &token) {
//     state_ = transition(state_, token);
// }

void SqlValidator::handle(const Token &token) {
    auto currentState = state_;
    state_ = std::visit([&token](auto&& state) {
        return transition(state, token);
    }, currentState);
}




struct TransitionFromStartVisitor {
    // Only the `Select` token advances the FSM from the `Start` state
    // TODO: Correct the behaviour
    State operator()(token::Select) const { return state::SelectStmt{}; }
    // All the other tokens, put it in the invalid state
    State operator()(token::From) const { return state::Invalid{}; }
    State operator()(token::Comma) const { return state::Invalid{}; }
    State operator()(token::Asterisks) const { return state::Invalid{}; }
    State operator()(token::Semicolon) const { return state::Invalid{}; }
    State operator()(token::Identifier) const { return state::Invalid{}; }
};

State transition(state::Start, const Token &token) {
    return std::visit(TransitionFromStartVisitor{}, token.value());
}

// ------------------ more Struct --------------------- //

struct TransitionFromSelectStmtVisitor {
    State operator()(token::From) const { return state::FromClause{}; }
    State operator()(token::Comma) const { return state::Invalid{}; }
    State operator()(token::Asterisks) const { return state::AllColumns{}; }
    State operator()(token::Semicolon) const { return state::Invalid{}; }
    State operator()(token::Identifier) const { return state::NamedColumn{}; }
    State operator()(token::Select) const { return state::Invalid{}; }
};


struct TransitionFromAllColumnsVisitor {
    State operator()(token::From) const { return state::FromClause{}; }
    State operator()(token::Comma) const { return state::MoreColumns{}; }
    State operator()(token::Asterisks) const { return state::Invalid{}; }
    State operator()(token::Semicolon) const { return state::Invalid{}; }
    State operator()(token::Identifier) const { return state::Invalid{}; }
    State operator()(token::Select) const { return state::Invalid{}; }
};

struct TransitionFromNamedColumnVisitor {
    State operator()(token::Comma) const { return state::MoreColumns{}; }
    State operator()(token::From) const { return state::FromClause{}; }
    State operator()(token::Asterisks) const { return state::Invalid{}; }
    State operator()(token::Semicolon) const { return state::Invalid{}; }
    State operator()(token::Identifier) const { return state::Invalid{}; }
    State operator()(token::Select) const { return state::Invalid{}; }

};


struct TransitionFromMoreColumnsVisitor {
    State operator()(token::Comma) const { return state::Invalid{}; }
    State operator()(token::From) const { return state::FromClause{}; }
    State operator()(token::Asterisks) const { return state::Invalid{}; }
    State operator()(token::Semicolon) const { return state::Invalid{}; }
    State operator()(token::Identifier) const { return state::Invalid{}; }
    State operator()(token::Select) const { return state::Invalid{}; }

};


struct TransitionFromFromClauseVisitor {
    State operator()(token::Identifier) const { return state::TableName{}; }
    State operator()(token::Asterisks) const { return state::Invalid{}; }
    State operator()(token::Semicolon) const { return state::Invalid{}; }
    State operator()(token::Comma) const { return state::Invalid{}; }
    State operator()(token::From) const { return state::Invalid{}; }
    State operator()(token::Select) const { return state::Invalid{}; }
};


struct TransitionFromTableNameVisitor {
    State operator()(token::Semicolon) const { return state::Valid{}; }
    State operator()(token::Comma) const { return state::Invalid{}; }
    State operator()(token::Asterisks) const { return state::Invalid{}; }
    State operator()(token::Identifier) const { return state::Invalid{}; }
    State operator()(token::From) const { return state::Invalid{}; }
    State operator()(token::Select) const { return state::Invalid{}; }
};

// Create similar transition functions for other new states

State transition(state::SelectStmt, const Token &token) {
    return std::visit(TransitionFromSelectStmtVisitor{}, token.value());
}
State transition(state::AllColumns, const Token &token) {
    return std::visit(TransitionFromAllColumnsVisitor{}, token.value());
}
State transition(state::NamedColumn, const Token &token) {
    return std::visit(TransitionFromNamedColumnVisitor{}, token.value());
}
State transition(state::MoreColumns, const Token &token) {
    return std::visit(TransitionFromMoreColumnsVisitor{}, token.value());
}
State transition(state::FromClause, const Token &token) {
    return std::visit(TransitionFromFromClauseVisitor{}, token.value());
}
State transition(state::TableName, const Token &token) {
    return std::visit(TransitionFromTableNameVisitor{}, token.value());
}

State transition(state::Valid, const Token &token) {
    // TODO: Implement
    return std::visit(TransitionFromTableNameVisitor{}, token.value());

}

State transition(state::Invalid, const Token &token) {
    // TODO: Implement
    return state::Invalid{};
}

bool is_valid_sql_query(const std::vector<Token> &tokens){
    SqlValidator validator;

    for (const auto &token : tokens) {
        validator.handle(token);
    }

    return validator.is_valid();
}

} // namespace sql
