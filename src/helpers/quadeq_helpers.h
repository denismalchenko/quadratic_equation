#ifndef Helpers_QuadraticEquation_H
#define Helpers_QuadraticEquation_H

#include <math.h>

#define NUMBER_PRECISION 0
#define TYPE_PRECISION 1
#define ABSOLUTE_VALUE(x) (x < 0 ? -x : x)
#define COUNT_DISCRIMINANT(bb, ac4) (bb - ac4)
#define DISCRIMINANT_EPSILON(ten_power) (powl(10, -2 * ten_power))
#define SQRT_DISCRIMINANT_EPSILON(ten_power) (powl(10, -ten_power))

typedef enum {
  ERROR_INPUT = -1,
  USUAL = 0,
  ZERO_ABC = 1,
  ZERO_AB = 2,
  ZERO_A = 3,
  ZERO_B = 4,
  ZERO_C = 5,
  SMALL_BB_AND_AC4 = 6,
  BIG_BB_AND_AC4 = 7,
  BIG_BB = 8,
  BIG_AC4 = 9
} EquationCases;

typedef struct {
  int type;
  union {
    int ten_power;
    char precision;
  } value;
} Accuracy;

int check_accuracy(const char *accuracy, Accuracy *epsilon);

EquationCases ascertain_equation_case(long double a, long double b,
                                      long double c, long double *bb,
                                      long double *ac4);

int is_discriminant_near_zero(long double discriminant, Accuracy epsilon,
                              long double bb);

#endif  // Helpers_QuadraticEquation_H