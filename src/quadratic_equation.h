// Copyright 2024 <Denis Malchenko (scimgeo)>
// "https://github.com/denismalchenko"

#ifndef SRC_QUADRATIC_EQUATION_H_
#define SRC_QUADRATIC_EQUATION_H_

#include <complex.h>

typedef enum {
  NoRoots = 0,
  OneDoubleRoot = 1,
  OnlyOneRoot = -1,
  TwoRealRoots = 2,
  TwoComplexRoots = -2,
  InfinityRoots = 3,
  InvalidValue = -3
} EquationResult;

typedef struct {
  EquationResult rootsNumber;
  long double complex root1;
  long double complex root2;
} EquationRoots;

EquationRoots solve_equation(long double a, long double b, long double c);

EquationRoots solve_equation_accur(long double a, long double b, long double c,
                                   const char *accuracy);

#define GET_FUNCTION(_1, _2, _3, _4, NAME, ...) NAME

#define SOLVE_EQUATION(...)                                             \
  GET_FUNCTION(__VA_ARGS__, solve_equation_accur, solve_equation, 0, 0) \
  (__VA_ARGS__)

#endif  // SRC_QUADRATIC_EQUATION_H_
