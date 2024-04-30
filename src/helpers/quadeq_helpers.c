// Copyright 2024 <Denis Malchenko (scimgeo)>
// "https://github.com/denismalchenko"

#include "quadeq_helpers.h"

#include <float.h>
#include <math.h>
#include <stdio.h>

int check_accuracy(const char *accuracy, Accuracy *epsilon) {
  if (accuracy == NULL || *accuracy == '\0' || *accuracy == 'l' ||
      *accuracy == 'L') {
    *epsilon = (Accuracy){.type = TYPE_PRECISION, .value.precision = 'l'};
    return 0;
  }
  if (*accuracy == 'd' || *accuracy == 'D') {
    *epsilon = (Accuracy){.type = TYPE_PRECISION, .value.precision = 'd'};
    return 0;
  }
  if (*accuracy == 'f' || *accuracy == 'F') {
    *epsilon = (Accuracy){.type = TYPE_PRECISION, .value.precision = 'f'};
    return 0;
  }
  if (*accuracy == 'i' || *accuracy == 'I') {
    *epsilon = (Accuracy){.type = NUMBER_PRECISION, .value.ten_power = 0};
    return 0;
  }
  if (*accuracy == 'z' || *accuracy == 'Z') {
    *epsilon = (Accuracy){.type = TYPE_PRECISION, .value.precision = 'z'};
    return 0;
  }

  int sign = (*accuracy == '-') ? -1 : 1;
  if (*accuracy == '-' || *accuracy == '+') {
    accuracy++;
  }

  if (*accuracy >= '0' && *accuracy <= '9') {
    int ten_power = 0;
    while (*accuracy >= '0' && *accuracy <= '9' &&
           ten_power < LDBL_MAX_10_EXP) {
      ten_power *= 10;
      ten_power += *accuracy - '0';
      accuracy++;
    }
    if (ten_power >= LDBL_MAX_10_EXP) {
      *epsilon =
          sign > 0 ? (Accuracy){.type = TYPE_PRECISION, .value.precision = 'l'}
                   : (Accuracy){.type = TYPE_PRECISION, .value.precision = 'z'};
    } else {
      *epsilon = (Accuracy){.type = NUMBER_PRECISION,
                            .value.ten_power = sign * ten_power};
    }
    return 0;
  }
  return 1;
}

EquationCases ascertain_equation_case(long double a, long double b,
                                      long double c) {
  if (!isfinite(a) || !isfinite(b) || !isfinite(c)) return ERROR_INPUT;
  if (a == 0 || b == 0 || c == 0) {
    if (a == 0) {
      return (b == 0) ? (c == 0 ? ZERO_ABC : ZERO_AB) : ZERO_A;
    } else {
      return (b == 0) ? (c == 0 ? ZERO_BC : ZERO_B) : ZERO_C;
    }
  }

  return USUAL;
}

long double count_sqrt_discriminant(long double a, long double b, long double c,
                                    Accuracy epsilon) {
  if (epsilon.type == TYPE_PRECISION && epsilon.value.precision == 'z')
    return 0;
  long double bb = b * b, ac4 = 4 * a * c, result = 0, discriminant = 0;
  long double abs_ac4 = ABSOLUTE_VALUE(ac4);

  if (!isinf(bb) && !isinf(ac4) && (bb >= LDBL_MIN || abs_ac4 >= LDBL_MIN)) {
    discriminant = bb - ac4;

    if (epsilon.type == TYPE_PRECISION) {
      result = sqrt_number_with_type_precision(
          discriminant, MINIMUM(bb, abs_ac4), epsilon.value.precision);
    } else {
      result = (ABSOLUTE_VALUE(discriminant) <
                DISCRIMINANT_EPSILON(epsilon.value.ten_power))
                   ? 0
               : (discriminant > 0) ? sqrtl(discriminant)
                                    : -sqrtl(-discriminant);
    }

  } else {
    int exp_ac4 = ilogbl(a) + ilogbl(c) + 2, exp_bb = 2 * ilogbl(b);

    if (exp_bb - exp_ac4 > LDBL_MANT_DIG) {
      result = ABSOLUTE_VALUE(b);
      if (epsilon.type == NUMBER_PRECISION &&
          result < SQRT_DISCRIMINANT_EPSILON(epsilon.value.ten_power))
        result = 0;
    } else if (exp_ac4 - exp_bb > LDBL_MANT_DIG) {
      result = (SAME_SIGN(a, c))
                   ? -2 * sqrtl(ABSOLUTE_VALUE(a)) * sqrtl(ABSOLUTE_VALUE(c))
                   : 2 * sqrtl(ABSOLUTE_VALUE(a)) * sqrtl(ABSOLUTE_VALUE(c));
      if (epsilon.type == NUMBER_PRECISION &&
          ABSOLUTE_VALUE(result) <
              SQRT_DISCRIMINANT_EPSILON(epsilon.value.ten_power))
        result = 0;

    } else {
      int exp_shift = MINIMUM(exp_ac4, exp_bb) / 2;
      a = ldexpl(a, -exp_shift);
      b = ldexpl(b, -exp_shift);
      c = ldexpl(c, -exp_shift);
      bb = b * b;
      ac4 = 4 * a * c;
      abs_ac4 = ABSOLUTE_VALUE(ac4);
      discriminant = bb - ac4;

      if (epsilon.type == TYPE_PRECISION) {
        result = sqrt_number_with_type_precision(
            discriminant, MINIMUM(bb, abs_ac4), epsilon.value.precision);
        result = ldexpl(result, exp_shift);
      } else {
        result = (discriminant > 0) ? sqrt(discriminant) : -sqrt(-discriminant);
        result = ldexpl(result, exp_shift);
        if (ABSOLUTE_VALUE(result) <
            SQRT_DISCRIMINANT_EPSILON(epsilon.value.ten_power))
          result = 0;
      }
    }
  }
  return result;
}

long double sqrt_number_with_type_precision(long double number,
                                            long double benchmark,
                                            char type_precision) {
  long double precision = ABSOLUTE_VALUE(number / benchmark);
  long double result = 0;
  switch (type_precision) {
    case 'f':
      result = (precision < FLT_EPSILON) ? 0
               : (number > 0)            ? sqrtl(number)
                                         : -sqrtl(-number);
      break;
    case 'd':
      result = (precision < DBL_EPSILON) ? 0
               : (number > 0)            ? sqrtl(number)
                                         : -sqrtl(-number);
      break;
    case 'l':
      result = (precision < LDBL_EPSILON) ? 0
               : (number > 0)             ? sqrtl(number)
                                          : -sqrtl(-number);
      break;
    default:
      result = (number > 0) ? sqrtl(number) : -sqrtl(-number);
  }
  return result;
}
