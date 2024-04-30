// Copyright 2024 <Denis Malchenko (scimgeo)>
// "https://github.com/denismalchenko"

#ifndef SRC_HELPERS_QUADEQ_HELPERS_H_
#define SRC_HELPERS_QUADEQ_HELPERS_H_

#include <math.h>

#define ABSOLUTE_VALUE(x) (x < 0 ? -x : x)
#define MINIMUM(x, y) (x < y ? x : y)
#define SAME_SIGN(x, y) (x < 0 ? y < 0 : y > 0)
#define DISCRIMINANT_EPSILON(ten_power) (powl(10, -2 * ten_power))
#define SQRT_DISCRIMINANT_EPSILON(ten_power) (powl(10, -ten_power))

typedef enum {
  ERROR_INPUT = -1,
  USUAL = 0,
  ZERO_ABC = 1,
  ZERO_AB = 2,
  ZERO_BC = 3,
  ZERO_A = 4,
  ZERO_B = 5,
  ZERO_C = 6
} EquationCases;

typedef enum { NUMBER_PRECISION = 0, TYPE_PRECISION = 1 } PrecisionType;

typedef struct {
  PrecisionType type;
  union {
    int ten_power;
    /**<
    Максимальное число цифр после запятой у аргументов a, b, c, если задана
    абсолютная точность. Если число отрицательное, то задает минимальное
    количество последних цифр равных нулю в десятичной записи аргументов a, b,
    c. Подробнее см. @ref accur_absolute_sec.
     */
    char precision;
    /**<
    Задает тип передаваемый аргументов a, b, c, если задана относительная
    точность или равен "z". Подробнее см. @ref accur_relative_sec.
     */

  } value;
} Accuracy;

EquationCases ascertain_equation_case(long double a, long double b,
                                      long double c);
int check_accuracy(const char *accuracy, Accuracy *epsilon);
long double count_sqrt_discriminant(long double a, long double b, long double c,
                                    Accuracy epsilon);
long double sqrt_number_with_type_precision(long double number,
                                            long double benchmark,
                                            char type_precision);

#endif  // SRC_HELPERS_QUADEQ_HELPERS_H_
