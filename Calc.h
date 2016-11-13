#ifndef __CALC_H
#define __CALC_H

#include <stdint.h>

typedef struct {
	const char* message;
	const char* position;
} CalcError;

typedef double (*CalcVariableCallback)(const char* name, uint32_t length);
typedef double (*CalcFunctionCallback)(const char* name, uint32_t length, double* args, uint8_t argsLength);

#ifdef __cplusplus
extern "C" double calc(const char* string, CalcVariableCallback var = nullptr, CalcFunctionCallback func = nullptr, CalcError* error = nullptr);
#else
double calc(const char* string, CalcVariableCallback var, CalcFunctionCallback func, CalcError* error);
#endif

#endif
