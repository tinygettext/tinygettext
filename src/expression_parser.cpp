// tinygettext - A gettext replacement that works directly on .po files
//
// The expression parser module for plural forms
// Copyright (c) 2018 acme_pjz <acme_pjz@hotmail.com>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "tinygettext/expression_parser.hpp"
#include "tinygettext/plural_forms.hpp"
#include "tinygettext/log_stream.hpp"

#include <stdlib.h>
#include <map>

using tinygettext::Log;

// This is a simple expression parser which parses a subset of
// standard C++ expressions. See
// https://en.cppreference.com/w/cpp/language/operator_precedence
// for a reference of C++ operator precedence.
//
// The lexer and parser are partially copied from LLVM Kaleidoscope tutorial
// http://www.llvm.org/docs/tutorial/index.html
//
// According to official GNU Gettext implementation
// https://www.gnu.org/software/gettext/manual/html_node/Plural-forms.html
// http://git.savannah.gnu.org/cgit/gettext.git/tree/gettext-runtime/intl/plural-exp.h#n34
// http://git.savannah.gnu.org/cgit/gettext.git/tree/gettext-runtime/intl/plural.y#n132
// there are no bitwise operators and all the numbers should be regarded as UNSIGNED integers.
// However, the existing tinygettext implementation treats numbers as SIGNED integers.
// So our parser also uses SIGNED integers and preserves the unary minus operator.
//
// Supported operators and precedences are:
//
// Precedence | Operator  | Description                             | Associativity
// -----------|-----------|-----------------------------------------|---------------
// N/A        | ( )       | Parenthesis                             | -
// N/A        | + - !     | Unary plus and minus, Logical NOT       | Right-to-left
// 110        | * / %     | Multiplication, division, and remainder | Left-to-right
// 100        | + -       | Addition and subtraction                | Left-to-right
// 80         | < <= > >= | Comparisons                             | Left-to-right
// 70         | == !=     | Comparisons                             | Left-to-right
// 30         | &&        | Logical AND (short-circuited)           | Left-to-right
// 20         | ||        | Logical OR (short-circuited)            | Left-to-right
// 10         | ? :       | Ternary conditional                     | Right-to-left
//
// EBNF:
//
// expression ::= ternary_expression
// ternary_expression ::= binary_expression | binary_expression '?' ternary_expression ':' ternary_expression
// binary_expression ::= primary_expression ( BINARY_OP primary_expression ) *
// NOTE: the parsing of binary_expression uses operator-precedence parser, see
// https://en.wikipedia.org/wiki/Operator-precedence_parser
// primary_expression ::= '(' expression ')' | NUMBER | VARIABLE | UNARY_OP primary_expression
//
// Lexer:
//
// NUMBER ::= [0-9]+
// NOTE: we only support integers, and we don't support hex or oct
// VARIABLE ::= 'n'
// NOTE: we only support the variable n

// The list of tokens.
enum Token {
	tok_invalid,

	tok_eof,

	tok_number,
	tok_variable,

	tok_logical_not,

	// The following tokens are also used as types of binary operations,
	// where the first 2 tokens together with tok_logical_not are also used as types
	// of unary operations.
	// NOTE: in the function getOperatorPrecedence() it's assumed that tok_plus
	// is the first binary operator in this enum.

	// Precendence 100
	tok_plus, tok_minus,

	// Precendence 110
	tok_times, tok_divides, tok_modulo,

	// Precendence 80
	tok_lt, tok_le, tok_gt, tok_ge,

	// Precendence 70
	tok_eq, tok_ne,

	// Precendence 30
	tok_logical_and,

	// Precendence 20
	tok_logical_or,

	// End of binary operations.

	tok_question_mark,
	tok_colon,
	tok_left_parenthesis,
	tok_right_parenthesis,
};

// Get the precendence of the binary operator (should be >0).
// -1 means not a binary operator.
int getOperatorPrecedence(Token token) {
	const int precedence[] = {
		100, 100,
		110, 110, 110,
		80, 80, 80, 80,
		70, 70,
		30,
		20,
	};

	int i = (int)token - (int)tok_plus;
	if (i >= 0 && i < sizeof(precedence) / sizeof(precedence[0])) {
		return precedence[i];
	}

	//Not a binary operator.
	return -1;
}

struct AST;
struct Tokenizer;

AST* parseTernaryExpression(Tokenizer& tokenizer);
AST* parseBinaryExpression(Tokenizer& tokenizer);
AST* parsePrimaryExpression(Tokenizer& tokenizer);

struct Tokenizer {
	// The expression string.
	const char* expression;

	// The next token.
	Token token;

	// The position.
	int pos;

	// The start position of current token.
	int startPos;

	// Filled in if tok_number
	int numVal;

	Tokenizer(const char* expression) : expression(expression), token(tok_invalid), pos(0), numVal(0) {}

	// Return the next token.
	Token getNextToken() {
		token = getNextTokenInternal();
		return token;
	}

	Token getNextTokenInternal();

	// Print error message.
	void printError(const char* message, int position = -1, int length = -1, int highlight = -1);
};

void Tokenizer::printError(const char* message, int position, int length, int highlight) {
	if (position < 0) {
		position = startPos;
		if (length <= 0) length = pos - startPos;
	}
	if (length <= 0) length = 1;

	std::string indicator = std::string(position, ' ') + std::string(length, '~');

	if (highlight >= 0 && highlight < (int)indicator.size()) indicator[highlight] = '^';

	log_error << "Error: " << message << "\n" << expression << "\n" << indicator << std::endl;
}

Token Tokenizer::getNextTokenInternal() {
	char c;

	// Skip any whitespace.
	while (isspace(c = expression[pos++])) {
	}

	// Record the start position.
	startPos = pos - 1;

	// Check number.
	if (isdigit(c)) {
		numVal = c - '0';
		while (isdigit(c = expression[pos++])) {
			numVal = numVal * 10 + (c - '0');
		}
		pos--;
		return tok_number;
	}

	// Check variable.
	if (c == 'n') {
		return tok_variable;
	}

	// Check operators.
	switch (c) {
	case '(': return tok_left_parenthesis;
	case ')': return tok_right_parenthesis;
	case '+': return tok_plus;
	case '-': return tok_minus;
	case '*': return tok_times;
	case '/': return tok_divides;
	case '%': return tok_modulo;
	case '?': return tok_question_mark;
	case ':': return tok_colon;
	case '<':
		switch (expression[pos++]) {
		case '=': return tok_le;
		default: pos--; return tok_lt;
		}
	case '>':
		switch (expression[pos++]) {
		case '=': return tok_ge;
		default: pos--; return tok_gt;
		}
	case '!':
		switch (expression[pos++]) {
		case '=': return tok_ne;
		default: pos--; return tok_logical_not;
		}
	case '=':
		switch (expression[pos++]) {
		case '=': return tok_eq;
		default: printError("'=' expected", -1, -1, pos - 1); pos--; return tok_invalid;
		}
	case '&':
		switch (expression[pos++]) {
		case '&': return tok_logical_and;
		default: printError("'&' expected", -1, -1, pos - 1); pos--; return tok_invalid;
		}
	case '|':
		switch (expression[pos++]) {
		case '|': return tok_logical_or;
		default: printError("'|' expected", -1, -1, pos - 1); pos--; return tok_invalid;
		}
	case ';': case '\0':
		pos--; return tok_eof;
	case '^': case '~':
		printError("This operator is unsupported in plural forms expression"); pos--; return tok_invalid;
	default:
		printError("Invalid character"); pos--; return tok_invalid;
	}
}

struct AST {
	virtual int evaluate(int n) = 0;
	virtual ~AST() {}
};

struct ConstExprAST : public AST {
	int value;

	ConstExprAST(int value) : value(value) {}
	int evaluate(int n) {
		return value;
	}
};

struct VariableExprAST : public AST {
	VariableExprAST() {}
	int evaluate(int n) {
		return n;
	}
};

struct UnaryExprAST : public AST {
	Token type;
	AST *child;

	UnaryExprAST(Token type, AST *child) : type(type), child(child) {}
	~UnaryExprAST() {
		delete child;
	}
	int evaluate(int n) {
		int ret = child->evaluate(n);
		switch (type) {
		case tok_plus: return ret;
		case tok_minus: return -ret;
		case tok_logical_not: return ret ? 0 : 1;
		default:
			log_error << "Unknown unary operator: " << (int)type << std::endl;
			return 0;
		}
	}
};

struct BinaryExprAST : public AST {
	Token type;
	AST *child1, *child2;

	BinaryExprAST(Token type, AST *child1, AST *child2) : type(type), child1(child1), child2(child2) {}
	~BinaryExprAST() {
		delete child1;
		delete child2;
	}
	int evaluate(int n) {
		switch (type) {
		case tok_plus: return child1->evaluate(n) + child2->evaluate(n);
		case tok_minus: return child1->evaluate(n) - child2->evaluate(n);
		case tok_times: return child1->evaluate(n) * child2->evaluate(n);
		case tok_divides: {
			int ret1 = child1->evaluate(n);
			int ret2 = child2->evaluate(n);
			if (ret2 == 0) {
				log_error << "Division by zero" << std::endl;
				return 0;
			} else if (ret2 == 1){
				return ret1;
			} else if (ret2 == -1) {
				return -ret1;
			} else {
				return ret1 / ret2;
			}
		}
		case tok_modulo: {
			int ret1 = child1->evaluate(n);
			int ret2 = child2->evaluate(n);
			if (ret2 == 0) {
				log_error << "Division by zero" << std::endl;
				return 0;
			} else if (ret2 == 1 || ret2 == -1){
				return 0;
			} else {
				return ret1 % ret2;
			}
		}
		case tok_logical_and: return (child1->evaluate(n) && child2->evaluate(n)) ? 1 : 0;
		case tok_logical_or: return (child1->evaluate(n) || child2->evaluate(n)) ? 1 : 0;
		case tok_lt: return (child1->evaluate(n) < child2->evaluate(n)) ? 1 : 0;
		case tok_gt: return (child1->evaluate(n) > child2->evaluate(n)) ? 1 : 0;
		case tok_le: return (child1->evaluate(n) <= child2->evaluate(n)) ? 1 : 0;
		case tok_ge: return (child1->evaluate(n) >= child2->evaluate(n)) ? 1 : 0;
		case tok_eq: return (child1->evaluate(n) == child2->evaluate(n)) ? 1 : 0;
		case tok_ne: return (child1->evaluate(n) != child2->evaluate(n)) ? 1 : 0;
		default:
			log_error << "Unknown binary operator: " << (int)type << std::endl;
			return 0;
		}
	}
};

struct TernaryExprAST : public AST {
	AST *child1, *child2, *child3;

	TernaryExprAST(AST *child1, AST *child2, AST *child3) : child1(child1), child2(child2), child3(child3) {}
	~TernaryExprAST() {
		delete child1;
		delete child2;
		delete child3;
	}
	int evaluate(int n) {
		return child1->evaluate(n) ? child2->evaluate(n) : child3->evaluate(n);
	}
};

inline AST* parseExpression(Tokenizer& tokenizer) {
	return parseTernaryExpression(tokenizer);
}

AST* parseTernaryExpression(Tokenizer& tokenizer) {
	AST *child1 = parseBinaryExpression(tokenizer);
	if (child1 == NULL) return NULL;

	if (tokenizer.token != tok_question_mark) return child1;

	tokenizer.getNextToken();

	AST* child2 = parseTernaryExpression(tokenizer);
	if (child2 == NULL) {
		delete child1;
		return NULL;
	}

	if (tokenizer.token != tok_colon) {
		tokenizer.printError("':' expected");
		delete child1;
		delete child2;
		return NULL;
	}

	tokenizer.getNextToken();

	AST* child3 = parseTernaryExpression(tokenizer);
	if (child3 == NULL) {
		delete child1;
		delete child2;
		return NULL;
	}

	return new TernaryExprAST(child1, child2, child3);
}

AST* parseBinaryExpressionInternal(Tokenizer& tokenizer, AST* lhs, int min_precedence) {
	// These codes are adapted from the pseudo code in
	// https://en.wikipedia.org/wiki/Operator-precedence_parser

	for (;;) {
		const int current_precendence = getOperatorPrecedence(tokenizer.token);
		if (current_precendence < min_precedence) return lhs;

		Token type = tokenizer.token;

		tokenizer.getNextToken();

		AST *rhs = parsePrimaryExpression(tokenizer);
		if (rhs == NULL) {
			delete lhs;
			return NULL;
		}

		for (;;) {
			int precendence = getOperatorPrecedence(tokenizer.token);
			if (precendence <= current_precendence) break;

			rhs = parseBinaryExpressionInternal(tokenizer, rhs, precendence);
			if (rhs == NULL) {
				delete lhs;
				return NULL;
			}
		}

		lhs = new BinaryExprAST(type, lhs, rhs);
	}
}

AST* parseBinaryExpression(Tokenizer& tokenizer) {
	AST *lhs = parsePrimaryExpression(tokenizer);
	if (lhs == NULL) return NULL;

	return parseBinaryExpressionInternal(tokenizer, lhs, 0);
}

AST* parsePrimaryExpression(Tokenizer& tokenizer) {
	switch (tokenizer.token) {
	case tok_left_parenthesis: {
		tokenizer.getNextToken();

		AST *ret = parseExpression(tokenizer);
		if (ret == NULL) return NULL;

		if (tokenizer.token != tok_right_parenthesis) {
			tokenizer.printError("')' expected");
			delete ret;
			return NULL;
		}

		tokenizer.getNextToken();

		return ret;
	}
	case tok_number: {
		AST *ret = new ConstExprAST(tokenizer.numVal);
		tokenizer.getNextToken();
		return ret;
	}
	case tok_variable:
		tokenizer.getNextToken();
		return new VariableExprAST();
	case tok_plus: case tok_minus: case tok_logical_not: {
		Token type = tokenizer.token;
		tokenizer.getNextToken();

		AST *ret = parsePrimaryExpression(tokenizer);
		if (ret == NULL) return NULL;

		if (type == tok_plus) return ret;
		return new UnaryExprAST(type, ret);
	}
	case tok_invalid:
		return NULL;
	default:
		tokenizer.printError("'(' or a number or 'n' or an unary operator expected");
		return NULL;
	}
}

class PluralExpressionFunctor : public tinygettext::PluralFunctor {
private:
	AST *root;
public:
	PluralExpressionFunctor(AST *root) : root(root) {}
	~PluralExpressionFunctor() {
		delete root;
	}
	unsigned int operator()(int n) const {
		if (root) return root->evaluate(n); else return 0;
	}
};

tinygettext::PluralFunctor* tinygettext::plural_functor_from_expression(const char* expression) {
	Tokenizer tokenizer(expression);
	tokenizer.getNextToken();

	AST *root = parseExpression(tokenizer);
	if (root == NULL) return NULL;

	return new PluralExpressionFunctor(root);
}
