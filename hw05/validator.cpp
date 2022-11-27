#include "validator.h"

#include <variant>
#include <vector>

#include "token.h"

namespace sql {

struct TransitionFromStartVisitor {
  /// Only the `Select` token advances the FSM from the `Start` state
  /// TODO: Correct the behaviour
  State operator()(token::Select) const { return state::SelectStmt{}; }
  /// All the other tokens, put it in the invalid state
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; } // problem here
  State operator()(token::Identifier) const { return state::Invalid{}; }
};

State transition(state::Start, Token token) {
  return std::visit(TransitionFromStartVisitor{}, token.value());
}

struct TransitionFromValidVistor {
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Valid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
};

State transition(state::Valid, Token token) {
  // TODO: Implement
  // return state::Invalid{};
  return std::visit(TransitionFromValidVistor{}, token.value());
}

struct TransitionFromInValidVistor {
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
};

State transition(state::Invalid, Token token) {
  // TODO: Implement
  return std::visit(TransitionFromInValidVistor{}, token.value());
}

struct TransitionFromSelectStmtVistor {
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::AllColumns{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::NamedColumn{}; }
};

State transition(state::SelectStmt, Token token) {
  return std::visit(TransitionFromSelectStmtVistor{}, token.value());
}

struct TransitionFromAllColumnsVistor {
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::FromClause{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
};

State transition(state::AllColumns, Token token) {
  return std::visit(TransitionFromAllColumnsVistor{}, token.value());
}

struct TransitionFromNamedColumnVistor {
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::FromClause{}; }
  State operator()(token::Comma) const { return state::MoreColumns{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
};

State transition(state::NamedColumn, Token token) {
  return std::visit(TransitionFromNamedColumnVistor{}, token.value());
}

struct TransitionFromMoreColumnsVistor {
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::NamedColumn{}; }
};

State transition(state::MoreColumns, Token token) {
  return std::visit(TransitionFromMoreColumnsVistor{}, token.value());
}

struct TransitionFromFromClauseVistor {
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::TableName{}; }
};

State transition(state::FromClause, Token token) {
  return std::visit(TransitionFromFromClauseVistor{}, token.value());
}

struct TransitionFromTableNameVistor {
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Valid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
};

State transition(state::TableName, Token token) {
  return std::visit(TransitionFromTableNameVistor{}, token.value());
}

bool SqlValidator::is_valid() const {
  /// TODO: Implement this
  return std::holds_alternative<state::Valid>(state_);
}

void SqlValidator::handle(Token token) { // transite from one state to the next
  /// TODO: Implement this
  std::visit(
      [this, token](auto state) { this->state_ = transition(state, token); },
      this->state_);
}

bool is_valid_sql_query(std::vector<Token> tokens) {
  SqlValidator sqlValidator{};
  for (const auto& token : tokens) {
    sqlValidator.handle(token);
  }
  return sqlValidator.is_valid();
}
} // namespace sql
