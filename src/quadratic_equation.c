#include "quadratic_equation.h"

// #include <float.h>
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
  long double bb = 0, ac4 = 0, discriminant = 0;
  Accuracy epsilon = (Accuracy){.type = TYPE_PRECISION, .value.precision = 'l'};
  switch (ascertain_equation_case(a, b, c, &bb, &ac4)) {
    case USUAL:
      if (check_accuracy(accuracy, &epsilon)) return roots;
      discriminant = COUNT_DISCRIMINANT(bb, ac4);
      if (is_discriminant_near_zero(discriminant, epsilon, bb)) {
        roots = (EquationRoots){.rootsNumber = OneDoubleRoot,
                                .root1 = -b / (2 * a),
                                .root2 = -b / (2 * a) + 0 * I};
      } else {
        roots =
            (discriminant > 0)
                ? (EquationRoots){.rootsNumber = TwoRealRoots,
                                  .root1 = (-b - sqrt(discriminant)) / (2 * a) +
                                           0 * I,
                                  .root2 = (-b + sqrt(discriminant)) / (2 * a) +
                                           0 * I}
                : (EquationRoots){
                      .rootsNumber = TwoComplexRoots,
                      .root1 = -b / (2 * a) - sqrt(-discriminant) / (2 * a) * I,
                      .root2 =
                          -b / (2 * a) + sqrt(-discriminant) / (2 * a) * I};
      }
      break;

    default:
      break;
  }
  return roots;
}

// int main() {
//   EquationRoots roots = SOLVE_EQUATION(1, 2, 3, "f");
//   printf("%f %f\n", crealf(roots.root1), cimagf(roots.root1));
//   printf("%f %f\n", crealf(roots.root2), cimagf(roots.root2));
//   long double a = 1e2466L, b = 1e-2475L;
//   printf("a^2 = %Lg, b^2 = %Lg\n", a * a, b * b);
//   a = 1e2467L;
//   b = 1e-2476L;
//   printf("a^2 = %Lg, b^2 = %Lg\n", a * a, b * b);
//   // isfinite(roots.root1);
//   return 0;
// }