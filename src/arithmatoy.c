#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int VERBOSE = 0;

const char *get_all_digits() { return "0123456789abcdefghijklmnopqrstuvwxyz"; }
const size_t ALL_DIGIT_COUNT = 36;

void arithmatoy_free(char *number) { free(number); }

char *arithmatoy_add(unsigned int base, const char *lhs, const char *rhs) {
  if (VERBOSE) {
    fprintf(stderr, "add %u %s %s --verbose\n", base, lhs, rhs);
  }

  char *lhslog = strdup(lhs);
  char *rhslog = strdup(rhs);

  if (lhs == NULL && rhs == NULL) {
    return NULL;
  }

  if (rhs == NULL) {
    return strdup(lhs);
  }

  if (lhs == NULL) {
    return strdup(rhs);
  }

  lhs = drop_leading_zeros(lhs);
  rhs = drop_leading_zeros(rhs);

  lhs = reverse(strdup(lhs));
  rhs = reverse(strdup(rhs));

  const size_t lhs_length = strlen(lhs);
  const size_t rhs_length = strlen(rhs);
  const size_t max_length = lhs_length > rhs_length ? lhs_length : rhs_length;
  const size_t buffer_length = max_length + 2; // +1 for carry, +1 for \0
  char *buffer = (char *)malloc(buffer_length * sizeof(char));

  unsigned int retenue = 0;
  unsigned int i = 0;

  while (i < max_length) {
    unsigned int lhs_digit = i < lhs_length ? get_digit_value(lhs[i]) : 0;
    unsigned int rhs_digit = i < rhs_length ? get_digit_value(rhs[i]) : 0;
    unsigned int sum = lhs_digit + rhs_digit + retenue;
    retenue = sum / base;
    buffer[i] = to_digit(sum % base);
    ++i;
  }

  if (retenue > 0) {
    buffer[i] = to_digit(retenue);
    ++i;
  }

  buffer[i] = '\0';
  buffer = reverse(buffer);
  buffer = (char *)drop_leading_zeros(buffer);
  if (VERBOSE) {
    fprintf(stderr, "%s + %s = %s\n", lhslog, rhslog, buffer);
    fprintf(stderr, "add: entering function\n");
  }
  return buffer;

  // Fill the function, the goal is to compute lhs + rhs
  // You should allocate a new char* large enough to store the result as a
  // string Implement the algorithm Return the result
}

char *arithmatoy_sub(unsigned int base, const char *lhs, const char *rhs) {
  if (VERBOSE) {
    fprintf(stderr, "sub %u %s %s --verbose\n", base, lhs, rhs);
  }

  char *lhslog = strdup(lhs);
  char *rhslog = strdup(rhs);

  if (lhs == NULL && rhs == NULL) {
    return NULL;
  }

  if (rhs == NULL) {
    return strdup(lhs);
  }

  if (lhs == NULL) {
    return strdup(rhs);
  }

  lhs = drop_leading_zeros(lhs);
  rhs = drop_leading_zeros(rhs);

  if (strlen(lhs) < strlen(rhs) ||
      strlen(lhs) == strlen(rhs) &&
          (get_digit_value(lhs[0]) < get_digit_value(rhs[0]))) {
    return NULL;
  }

  lhs = reverse(strdup(lhs));
  rhs = reverse(strdup(rhs));

  const size_t lhs_length = strlen(lhs);
  const size_t rhs_length = strlen(rhs);
  const size_t max_length = lhs_length > rhs_length ? lhs_length : rhs_length;
  const size_t buffer_length = max_length + 1; // +1 for \0 (no carry)
  char *buffer = (char *)malloc(buffer_length * sizeof(char));

  unsigned int retenue = 0;
  unsigned int i = 0;

  while (i < max_length) {
    unsigned int lhs_digit = i < lhs_length ? get_digit_value(lhs[i]) : 0;
    unsigned int rhs_digit = i < rhs_length ? get_digit_value(rhs[i]) : 0;
    int diff = lhs_digit - rhs_digit - retenue;
    if (diff < 0) {
      retenue = 1;
      diff += base;
    } else {
      retenue = 0;
    }
    buffer[i] = to_digit(diff);
    ++i;
  }

  buffer[i] = '\0';
  buffer = reverse(buffer);
  buffer = (char *)drop_leading_zeros(buffer);
  if (VERBOSE) {
    fprintf(stderr, "%s - %s = %s\n", lhslog, rhslog, buffer);
    fprintf(stderr, "sub: entering function\n");
  }
  return buffer;

  // Fill the function, the goal is to compute lhs - rhs (assuming lhs > rhs)
  // You should allocate a new char* large enough to store the result as a
  // string Implement the algorithm Return the result
}

char *arithmatoy_mul(unsigned int base, const char *lhs, const char *rhs) {
  if (VERBOSE) {
    fprintf(stderr, "mul %u %s %s --verbose\n", base, lhs, rhs);
  }

  char *lhslog = strdup(lhs);
  char *rhslog = strdup(rhs);

  if (lhs == NULL && rhs == NULL) {
    return NULL;
  }

  if (rhs == NULL) {
    return strdup(lhs);
  }

  if (lhs == NULL) {
    return strdup(rhs);
  }

  lhs = drop_leading_zeros(lhs);
  rhs = drop_leading_zeros(rhs);

  lhs = reverse(strdup(lhs));
  rhs = reverse(strdup(rhs));

  const size_t lhs_length = strlen(lhs);
  const size_t rhs_length = strlen(rhs);
  const size_t buffer_length = lhs_length + rhs_length + 1; // +1 for \0
  char *buffer = (char *)malloc(buffer_length * sizeof(char));

  for (size_t i = 0; i < buffer_length; ++i) {
    buffer[i] = '0';
  }

  for (size_t i = 0; i < lhs_length; ++i) {
    unsigned int lhs_digit = get_digit_value(lhs[i]);
    unsigned int retenue = 0;
    for (size_t j = 0; j < rhs_length; ++j) {
      unsigned int rhs_digit = get_digit_value(rhs[j]);
      unsigned int product = lhs_digit * rhs_digit + retenue + get_digit_value(buffer[i + j]);
      retenue = product / base;
      buffer[i + j] = to_digit(product % base);
    }
    buffer[i + rhs_length] = to_digit(retenue);
  }

  buffer[buffer_length - 1] = '\0';
  buffer = reverse(buffer);
  buffer = (char *)drop_leading_zeros(buffer);
  if (VERBOSE) {
    fprintf(stderr, "%s * %s = %s\n", lhslog, rhslog, buffer);
    fprintf(stderr, "mul: entering function\n");
  }
  return buffer;

  // Fill the function, the goal is to compute lhs * rhs
  // You should allocate a new char* large enough to store the result as a
  // string Implement the algorithm Return the result
}

// Here are some utility functions that might be helpful to implement add, sub
// and mul:

unsigned int get_digit_value(char digit) {
  // Convert a digit from get_all_digits() to its integer value
  if (digit >= '0' && digit <= '9') {
    return digit - '0';
  }
  if (digit >= 'a' && digit <= 'z') {
    return 10 + (digit - 'a');
  }
  return -1;
}

char to_digit(unsigned int value) {
  // Convert an integer value to a digit from get_all_digits()
  if (value >= ALL_DIGIT_COUNT) {
    debug_abort("Invalid value for to_digit()");
    return 0;
  }
  return get_all_digits()[value];
}

char *reverse(char *str) {
  // Reverse a string in place, return the pointer for convenience
  // Might be helpful if you fill your char* buffer from left to right
  const size_t length = strlen(str);
  const size_t bound = length / 2;
  for (size_t i = 0; i < bound; ++i) {
    char tmp = str[i];
    const size_t mirror = length - i - 1;
    str[i] = str[mirror];
    str[mirror] = tmp;
  }
  return str;
}

const char *drop_leading_zeros(const char *number) {
  // If the number has leading zeros, return a pointer past these zeros
  // Might be helpful to avoid computing a result with leading zeros
  if (*number == '\0') {
    return number;
  }
  while (*number == '0') {
    ++number;
  }
  if (*number == '\0') {
    --number;
  }
  return number;
}

void debug_abort(const char *debug_msg) {
  // Print a message and exit
  fprintf(stderr, debug_msg);
  exit(EXIT_FAILURE);
}
