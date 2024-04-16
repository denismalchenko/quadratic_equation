#include "quadeq_helpers.h"

#include <float.h>
#include <math.h>
#include <stdio.h>

#define MAX_STRING_LENGTH_FOR_POWER ceil(log10(LDBL_MAX_10_EXP))

int check_accuracy(const char *accuracy, Accuracy *epsilon) {
  if (accuracy == NULL || *accuracy == '\0' || *accuracy == 'l' ||
      *accuracy == 'L') {
    *epsilon = (Accuracy){.type = TYPE_PRECISION, .value.precision = 'l'};
    return 1;
  }
  if (*accuracy == 'd' || *accuracy == 'D') {
    *epsilon = (Accuracy){.type = TYPE_PRECISION, .value.precision = 'd'};
    return 1;
  }
  if (*accuracy == 'f' || *accuracy == 'F') {
    *epsilon = (Accuracy){.type = TYPE_PRECISION, .value.precision = 'f'};
    return 1;
  }
  if (*accuracy == 'i' || *accuracy == 'I') {
    *epsilon = (Accuracy){.type = NUMBER_PRECISION, .value.ten_power = 0};
    return 1;
  }
  if (*accuracy == 'z' || *accuracy == 'Z') {
    *epsilon = (Accuracy){.type = TYPE_PRECISION, .value.precision = 'z'};
    return 1;
  }
  int sign = (*accuracy == '-') ? -1 : 1;
  if (*accuracy == '-' || *accuracy == '+') {
    accuracy++;
  }

  if (*accuracy >= '0' && *accuracy <= '9') {
    int ten_power = 0, remaining_lenght = MAX_STRING_LENGTH_FOR_POWER;
    while (*accuracy >= '0' && *accuracy <= '9' && remaining_lenght > 0) {
      ten_power *= 10;
      ten_power += *accuracy - '0';
      accuracy++;
      remaining_lenght--;
    }
    if ((remaining_lenght == 0 && *accuracy >= '0' && *accuracy <= '9') ||
        ten_power > LDBL_MAX_10_EXP) {
      *epsilon =
          sign > 0 ? (Accuracy){.type = TYPE_PRECISION, .value.precision = 'l'}
                   : (Accuracy){.type = TYPE_PRECISION, .value.precision = 'z'};
    } else {
      *epsilon = (Accuracy){.type = NUMBER_PRECISION,
                            .value.ten_power = sign * ten_power};
    }
    return 1;
  }
  return 0;
}

EquationCases ascertain_equation_case(long double a, long double b,
                                      long double c, long double *bb,
                                      long double *ac4) {
  if (!isfinite(a) || !isfinite(b) || !isfinite(c)) return ERROR_INPUT;
  if (a == 0 || b == 0 || c == 0) {
    if (a == 0) {
      return (b == 0) ? (c == 0 ? ZERO_ABC : ZERO_AB) : ZERO_A;
    } else {
      return (b == 0) ? ZERO_B : ZERO_C;
    }
  }

  *bb = b * b;
  *ac4 = 4 * a * c;

  if (*bb < LDBL_MIN && ABSOLUTE_VALUE(*ac4) < LDBL_MIN)
    return SMALL_BB_AND_AC4;
  if (isinf(*bb) || isinf(*ac4))
    return isinf(*bb) ? (isinf(*ac4) ? BIG_BB_AND_AC4 : BIG_BB) : BIG_AC4;

  return USUAL;
}
