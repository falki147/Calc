# Calc
Calculate value from a string. Example: `1 + 2 * sin(3 * pi / 2)`

## Features
- Operators: `+`, `-`, `*`, `/`
- Functions
- Variables
- Error Handling

## Calc Function
```
double calc(const char* string, CalcVariableCallback var, CalcFunctionCallback func, CalcError* error);
```

| Parameter | Description |
|-----------|-----|
| string    | Expression as a string |
| var       | Callback when variable is encountered, can be null |
| func      | Callback when function is encountered, can be null |
| error     | Pointer to CalcError struct, only modified on error, can be null |
