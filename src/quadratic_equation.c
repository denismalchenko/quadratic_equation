#include "quadratic_equation.h"

// #include <float.h>
#include <math.h>
#include <stdio.h>

#include "helpers/quadeq_helpers.h"

EquationRoots solve_equation(long double a, long double b, long double c) {
  // printf("no accuracy\n");
  return solve_equation_accur(a, b, c, NULL);
}

EquationRoots solve_equation_accur(long double a, long double b, long double c,
                                   const char *accuracy) {
  EquationRoots roots;
  roots.rootsNumber = NoRoots;
  roots.root1 = 0 + 0 * I;
  roots.root2 = 0 + 0 * I;
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