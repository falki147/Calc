#include "calc.h"

#include <stdlib.h>
#include <ctype.h>
#include <setjmp.h>

#ifndef ARGUMENT_LIMIT
#define ARGUMENT_LIMIT 16
#endif

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

typedef struct {
	const char* cur;
	const char* next;
	uint8_t last;

	CalcVariableCallback var;
	CalcFunctionCallback func;

	jmp_buf buf;

	union {
		double dbl;

		struct {
			const char* beg;
			const char* end;
		} id;
	} value;
} Context;

enum {
	TokenInvalid,
	TokenEOF,
	TokenPlus,
	TokenMinus,
	TokenMul,
	TokenDiv,
	TokenOpen,
	TokenClose,
	TokenNumber,
	TokenIdentifier,
	TokenComma
};

enum {
	ErrorToken = 1,
	ErrorArguments,
	ErrorVarFunc,
	ErrorFuncFunc
};

static uint8_t peek(Context* con);
static void next(Context* con);
static double parse(Context* con);
static double parseAddSub(Context* con);
static double parseMulDiv(Context* con);
static double parseValue(Context* con);

double calc(const char* string, CalcVariableCallback var, CalcFunctionCallback func, CalcError* error) {
	if (!string)
		return 0;

	Context con = { string, string, 0, var, func };

	int ret = setjmp(con.buf);

	if (ret) {
		if (error) {
			error->position = con.cur;
			error->message = "unexpected character";

			const char* messages[] = {
				"unexpected character",
				"unexpected end",
				"unexpected plus",
				"unexpected minus",
				"unexpected star",
				"unexpected slash",
				"unexpected opened parentheses",
				"unexpected closed parentheses",
				"unexpected number",
				"unexpected identifier",
				"unexpected comma"
			};

			switch (ret) {
			case ErrorToken:
				if (con.last < sizeof(messages) / sizeof(*messages))
					error->message = messages[con.last];
				break;
			case ErrorArguments:
				error->message = "too many arguments (argument limit: " STRINGIFY(ARGUMENT_LIMIT) ")";
				break;
			case ErrorVarFunc:
				error->message = "no variable callback set";
				break;
			case ErrorFuncFunc:
				error->message = "no function callback set";
				break;
			}
		}
		
		return 0;
	}

	return parse(&con);
}

uint8_t peek(Context* con) {
	if (con->cur != con->next)
		return con->last;

	while (isspace(*con->cur))
		++con->cur;

	con->next = con->cur;

	if (*con->next == 0)
		return con->last = TokenEOF;

	const uint8_t table[] = {
		TokenOpen, TokenClose, TokenMul, TokenPlus, TokenComma, TokenMinus, TokenInvalid, TokenDiv
	};

	switch (*con->next) {
	case '+':
	case '-':
	case '*':
	case '/':
	case '(':
	case ')':
	case ',':
		return con->last = table[*con->next++ - '('];
	}

	if (isdigit(*con->next)) {
		con->value.dbl = strtod(con->next, (char**) &con->next);

		if (con->next != con->cur)
			return con->last = TokenNumber;
	}

	if (isalpha(*con->next) || *con->next == '_') {
		con->value.id.beg = con->next++;

		while (isalnum(*con->next) || *con->next == '_')
			++con->next;

		con->value.id.end = con->next;
		return con->last = TokenIdentifier;
	}

	con->last = TokenInvalid;
	longjmp(con->buf, ErrorToken);
}

void next(Context* con) {
	con->cur = con->next;
}

double parseValue(Context* con) {
	switch (peek(con)) {
	case TokenNumber:
		next(con);
		return con->value.dbl;
	case TokenOpen:
		next(con);
		double value = parseAddSub(con);

		if (peek(con) != TokenClose)
			longjmp(con->buf, ErrorToken);

		next(con);
		return value;
	case TokenPlus:
		next(con);
		return parseValue(con);
	case TokenMinus:
		next(con);
		return -parseValue(con);
	case TokenIdentifier:
		next(con);

		const char* beg = con->value.id.beg;
		const char* end = con->value.id.end;

		if (peek(con) != TokenOpen) {
			if (!con->var)
				longjmp(con->buf, ErrorVarFunc);

			return con->var(beg, end - beg);
		}
		else {
			next(con);

			double args[ARGUMENT_LIMIT];
			uint8_t numArgs = 0;

			uint8_t token = peek(con);

			while (token != TokenClose) {
				if (numArgs >= ARGUMENT_LIMIT)
					longjmp(con->buf, ErrorArguments);

				args[numArgs++] = parseAddSub(con);

				token = peek(con);

				if (token != TokenComma && token != TokenClose)
					longjmp(con->buf, ErrorToken);

				next(con);
			}

			next(con);

			if (!con->func)
				longjmp(con->buf, ErrorFuncFunc);

			return con->func(beg, end - beg, args, numArgs);
		}
		break;
	}

	longjmp(con->buf, ErrorToken);
}

double parseMulDiv(Context* con) {
	double value = parseValue(con);

	for (;;) {
		switch (peek(con)) {
		case TokenMul:
			next(con);
			value *= parseValue(con);
			continue;
		case TokenDiv:
			next(con);
			value /= parseValue(con);
			continue;
		}

		break;
	}

	return value;
}

double parseAddSub(Context* con) {
	double value = parseMulDiv(con);

	for (;;) {
		switch (peek(con)) {
		case TokenPlus:
			next(con);
			value += parseMulDiv(con);
			continue;
		case TokenMinus:
			next(con);
			value -= parseMulDiv(con);
			continue;
		}

		break;
	}

	return value;
}

double parse(Context* con) {
	double ret = parseAddSub(con);

	if (peek(con) != TokenEOF)
		longjmp(con->buf, ErrorToken);

	return ret;
}
