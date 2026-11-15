#pragma once
#include "AST.h"
#include "action.h"
#include "runtime_value.h"
#include "tokens.h"
#include <array>
class RuntimeValue;
// just some short helper functions here & check functions as well
namespace utils {

constexpr std::array<int32_t, static_cast<size_t>(StmtType::Amount)>
    AllowedStmt{
        0b0000, // Input
        0b0000, // Output
        0b0101, // Expression
        0b0000, // If statement
        0b0000, // While
        0b0010, // For
        0b0000, // Break
        0b0000, // Continue
        0b1111, // Function
        0b0000, // Return
        0b0000  // Block
    };

const int32_t AllowedDef = 0b101;

const int32_t AllowedParam = 0b100;
/**
 * helper function to identify if the data type is numerical (can be casted to
 * a number)
 */
bool isNumerical(const RuntimeValue &value);
/// helper function to identify logical operators
bool isLogical(const Operator &op);
/// helper function to identify -> and ->= operators
bool isArrow(const Operator &op);
/// helper function to identify is the tokentype is a part of an expression
bool isExpression(const TokenType &type);
/// helper function to identify if the keyword is an identifier
bool isModifier(const Token &token);
/**
 * checks if applied to the statement modifiers are legit. I use bitmasks
 * for each statement type defined above to do so
 */
bool CheckModifiers(const StmtType &type, const int32_t &modifiers);
/// It used to check modifiers for expressions. Not used in current version.
bool CheckModifiers(const Expression &expr, const int32_t &modifiers);

/// checks modifiers for a function parameter
bool CheckModifiers(const Parameter &param);

/// helper functions to identify specific modifier
bool isGlobal(const int32_t &modifiers);

bool isDynamic(const int32_t &modifiers);

bool isConst(const int32_t &modifiers);
/**
 * helper function to translate operator (used in lexer and AST) into bytecode
 * instruction (action)
 */
Action getOperatorAction(const Operator &op, const bool &binary = true);

/**
 * converts string into char32_t datatype, which can
 * be up to 4 times the value of basic char
 */
char32_t decodeUTF8(const std::string &s);
/**
 * gives the length of char, as char32_t can be up to 4 basic char
 * symbols(the length is stored in the first byte)
 */
size_t utf8CharLength(const unsigned char &c);
} // namespace utils
