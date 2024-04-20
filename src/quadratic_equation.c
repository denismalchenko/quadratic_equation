// Copyright 2024 <Denis Malchenko (scimgeo)>
// "https://github.com/denismalchenko"
#include "quadratic_equation.h"

#include <math.h>
#include <stdio.h>

#include "helpers/quadeq_helpers.h"

EquationRoots solve_equation(long double a, long double b, long double c) {
  return solve_equation_accur(a, b, c, NULL);
}

EquationRoots solve_equation_accur(long double a, long double b, long double c,
                                   const char *accuracy) {
  EquationRoots roots = (EquationRoots){
      .rootsNumber = InvalidValue, .root1 = NAN + 0 * I, .root2 = NAN + 0 * I};
  long double sqrt_discriminant = 0, result = 0;
  Accuracy epsilon = (Accuracy){.type = TYPE_PRECISION, .value.precision = 'l'};

  switch (ascertain_equation_case(a, b, c)) {
    case USUAL:
      if (check_accuracy(accuracy, &epsilon)) return roots;
      sqrt_discriminant = count_sqrt_discriminant(a, b, c, epsilon);
      if (!isfinite(sqrt_discriminant + ABSOLUTE_VALUE(b)) &&
          ABSOLUTE_VALUE(a) >= 1) {
        b = b / 2 / a;
        c = c / 2 / a;
        a = 0.5;
        sqrt_discriminant = count_sqrt_discriminant(a, b, c, epsilon);
      }
      if (sqrt_discriminant == 0) {
        roots = (EquationRoots){.rootsNumber = OneDoubleRoot,
                                .root1 = -b / (2 * a) + 0 * I,
                                .root2 = -b / (2 * a) + 0 * I};
      } else {
        int sign_a = (a > 0) ? 1 : -1;
        roots =
            (sqrt_discriminant > 0)
                ? (EquationRoots){.rootsNumber = TwoRealRoots,
                                  .root1 = (-b - sign_a * sqrt_discriminant) /
                                               (2 * a) +
                                           0 * I,
                                  .root2 = (-b + sign_a * sqrt_discriminant) /
                                               (2 * a) +
                                           0 * I}
                : (EquationRoots){
                      .rootsNumber = TwoComplexRoots,
                      .root1 = -b / (2 * a) +
                               sign_a * sqrt_discriminant / (2 * a) * I,
                      .root2 = -b / (2 * a) -
                               sign_a * sqrt_discriminant / (2 * a) * I};
      }
      break;
    case ZERO_C:
      result = -b / a;
      roots = (result > 0) ? (EquationRoots){.rootsNumber = TwoRealRoots,
                                             .root1 = 0 + 0 * I,
                                             .root2 = result + 0 * I}
                           : (EquationRoots){.rootsNumber = TwoRealRoots,
                                             .root1 = result + 0 * I,
                                             .root2 = 0 + 0 * I};
      break;
    case ZERO_B:
      result = -c / a;
      roots = (result > 0) ? (EquationRoots){.rootsNumber = TwoRealRoots,
                                             .root1 = -sqrtl(result) + 0 * I,
                                             .root2 = sqrtl(result) + 0 * I}
                           : (EquationRoots){.rootsNumber = TwoComplexRoots,
                                             .root1 = 0 - sqrtl(-result) * I,
                                             .root2 = 0 + sqrtl(-result) * I};
      break;
    case ZERO_A:
      roots = (EquationRoots){.rootsNumber = OnlyOneRoot,
                              .root1 = -c / b + 0 * I,
                              .root2 = NAN + 0 * I};
      break;
    case ZERO_BC:
      roots = (EquationRoots){
          .rootsNumber = OneDoubleRoot, .root1 = 0 + 0 * I, .root2 = 0 + 0 * I};
      break;
    case ZERO_AB:
      roots.rootsNumber = NoRoots;
      break;
    case ZERO_ABC:
      roots.rootsNumber = InfinityRoots;
      break;
    default:
      break;
  }
  return roots;
}
