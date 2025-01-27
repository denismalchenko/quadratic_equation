// Copyright 2024 <Denis Malchenko (scimgeo)>
// "https://github.com/denismalchenko"

#include "quadratic_equation.h"

#include <math.h>
#include <stdio.h>

#include "helpers/quadeq_helpers.h"

EquationRoots solve_equation(long double a, long double b, long double c) {
  //@ [solve_equation body]
  return solve_equation_accur(a, b, c, NULL);
  //@ [solve_equation body]
}

EquationRoots solve_equation_accur(long double a, long double b, long double c,
                                   const char *accuracy) {
  //@ [initializing variables in solve_equation_accur]
  EquationRoots roots = (EquationRoots){
      .rootsNumber = InvalidValue, .root1 = NAN + 0 * I, .root2 = NAN + 0 * I};
  long double sqrt_discriminant = 0, result = 0;
  Accuracy epsilon = (Accuracy){.type = TYPE_PRECISION, .value.precision = 'l'};
  //@ [initializing variables in solve_equation_accur]

  //@ [solve_equation_accur switch]
  switch (ascertain_equation_case(a, b, c)) {
      //@ [solve_equation_accur switch]
    case USUAL:
      //@ [USUAL start]
      if (check_accuracy(accuracy, &epsilon)) return roots;
      sqrt_discriminant = count_sqrt_discriminant(a, b, c, epsilon);
      //@ [USUAL start]
      //@ [zero discriminant]
      if (sqrt_discriminant == 0) {
        roots = (EquationRoots){.rootsNumber = OneDoubleRoot,
                                .root1 = -b / (2 * a) + 0 * I,
                                .root2 = -b / (2 * a) + 0 * I};
        //@ [zero discriminant]
      } else {
        //@ [check monster huge discriminant]
        if (!isfinite(sqrt_discriminant + ABSOLUTE_VALUE(b)) &&
            ABSOLUTE_VALUE(a) > 0.5) {
          b = b / 2 / a;
          c = c / 2 / a;
          a = 0.5;
          //@ [check monster huge discriminant]
          //@ [one more attempt]
          epsilon = (Accuracy){.type = TYPE_PRECISION, .value.precision = 'l'};
          sqrt_discriminant = count_sqrt_discriminant(a, b, c, epsilon);
        }
        //@ [one more attempt]
        //@ [standart solve equation]
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
      //@ [standart solve equation]
      break;
    //@ [solve_equation_accur other cases]
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
      //@ [solve_equation_accur other cases]
      //@ [solve_equation_accur default]
    default:
      break;
  }

  return roots;
  //@ [solve_equation_accur default]
}
