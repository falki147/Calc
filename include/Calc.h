/**
 * \mainpage Calc
 *
 * \section intro_sec Introduction
 * This library provides an easy and effecient way to calculate the result of
 * an expression. E.g. this line prints 7
 *
 * \code{.c}
 * printf("%f\n", calc("1 + 2 * 3", NULL, NULL, NULL));
 * \endcode
 *
 * \section features_sec Features
 * - Operators: <tt>+ - * /</tt>
 * - Functions
 * - Variables
 * - Error Handling
 *
 * \section building_sec Building
 * The library can be built using CMake. It can only be built as a static
 * library. If \a CALC_BUILD_EXAMPLE is enabled the example is included.
 *
 * \section install_sec Installation
 * This library provides a CMake file which can be used to build it and install
 * it on your system. It can be included in any CMake project with
 * <tt>find_package(Calc)</tt> after installing it. You can however also copy
 * the files into your project.
 *
 * \section usage_sec Usage
 * See Calc.h
 */

/**
 * \file Calc.h
 * \brief Calculates the result of an expression
 */

#ifndef __CALC_H
#define __CALC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \struct CalcError
 * \brief Holds information about the error
 */

typedef struct {
	const char* message; ///< Null-terminated error message. It must not be freed.
	const char* position; ///< Position in the input string where the error occurd
} CalcError;

/**
 * \typedef double (*CalcVariableCallback)(const char* name, uint32_t length)
 * \brief Called when a variable is encountered
 * \param name Name of the variable (not null-terminated)
 * \param length Length of the name
 * \return Value of the variable
 */

typedef double (*CalcVariableCallback)(const char* name, uint32_t length);

/**
 * \typedef double (*CalcFunctionCallback)(const char* name, uint32_t length, double* args, uint8_t argsLength)
 * \brief Called when a function is encountered
 * \param name Name of the function (not null-terminated)
 * \param length Length of the name
 * \param args Arguments passed to the function
 * \param argsLength Number of arguments
 * \return Value of the function call
 */

typedef double (*CalcFunctionCallback)(const char* name, uint32_t length, double* args, uint8_t argsLength);

/**
 * \fn double calc(const char* string, CalcVariableCallback var, CalcFunctionCallback func, CalcError* error)
 * \brief Calculates the result of an expression.
 *
 * This function uses a recursive decent parser to determine the value. The
 * expressions are C-like. The following operators are built in: + - * /. Plus
 * and minus also act as sign for numbers. The multiply and divide operator
 * have a lower precedence than the addition and subtraction ones.
 *
 * There are no inbuilt variables or functions. Everything is forwarded to the
 * calback functions. If the corresponding callback function is <tt>NULL</tt>
 * the function fails with an error. If \a error is set to <tt>NULL</tt>, it is
 * ignored.
 *
 * \param string Null-terminated string with expression
 * \param var Variable callback. It can be NULL.
 * \param func Function callback. It can be NULL.
 * \param error Error struct. It can be NULL.
 * \return Result or 0.0 on error
 */

double calc(const char* string, CalcVariableCallback var, CalcFunctionCallback func, CalcError* error);

#ifdef __cplusplus
}
#endif

#endif
