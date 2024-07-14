#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

const size_t ALL_DIGIT_COUNT = 36;

#define VERBOSE 0

const char *get_all_digits() { return "0123456789abcdefghijklmnopqrstuvwxyz"; }

void arithmatoy_free(char *number) { free(number); }

char *init_buffer(size_t size) {
  char *buffer = (char *)malloc(size + 1);
  if (buffer) {
    memset(buffer, '0', size);
    buffer[size] = '\0';
  }
  return buffer;
}

char to_digit(unsigned int val) {
  if (val < 10) {
    return '0' + val;
  } else if (val < 36) {
    return 'a' + (val - 10);
  }
  return '0';
}

unsigned int get_digit_value(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  } else if (c >= 'a' && c <= 'z') {
    return 10 + c - 'a';
  } else if (c >= 'A' && c <= 'Z') {
    return 10 + c - 'A';
  }
  return 0;
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

char *drop_leading_zeros(const char *number) {
  // If the number has leading zeros, return a pointer past these zeros
  // Might be helpful to avoid computing a result with leading zeros
  if (*number == '\0') {
    return (char *)number;
  }
  while (*number == '0') {
    ++number;
  }
  if (*number == '\0') {
    --number;
  }
  return (char *)number;
}

char *arithmatoy_add(unsigned int base, const char *lhs, const char *rhs) {
  if (VERBOSE) {
    fprintf(stderr, "add: entering function\n");
  }

  // Check for invalid inputs
  if (lhs == NULL || rhs == NULL || base >= ALL_DIGIT_COUNT) {
    return NULL;
  }

  // Drop leading zeros
  lhs = drop_leading_zeros(lhs);
  rhs = drop_leading_zeros(rhs);

  const size_t lhs_length = strlen(lhs);
  const size_t rhs_length = strlen(rhs);
  const size_t max_length = lhs_length > rhs_length ? lhs_length : rhs_length;
  char *result = init_buffer(max_length);

  unsigned int carry = 0;

  // Add the digits from right to left
  for (size_t i = 0; i < max_length; ++i) {
    const unsigned int lhs_digit =
        i < lhs_length ? get_digit_value(lhs[lhs_length - i - 1]) : 0;

    const unsigned int rhs_digit =
        i < rhs_length ? get_digit_value(rhs[rhs_length - i - 1]) : 0;

    const unsigned int sum = lhs_digit + rhs_digit + carry;

    // Compute the carry and the sum
    carry = sum / base;

    // Store the result
    result[max_length - i] = to_digit(sum % base);
  }

  // Store the last carry
  result[0] = to_digit(carry);

  return drop_leading_zeros(result);
}

char *arithmatoy_sub(unsigned int base, const char *lhs, const char *rhs) {
  if (VERBOSE) {
    fprintf(stderr, "sub: entering function\n");
  }

  // Check for invalid inputs
  if (lhs == NULL || rhs == NULL || base >= ALL_DIGIT_COUNT) {
    return NULL;
  }

  // Drop leading zeros
  lhs = drop_leading_zeros(lhs);
  rhs = drop_leading_zeros(rhs);

  const size_t lhs_length = strlen(lhs);
  const size_t rhs_length = strlen(rhs);
  const size_t max_length = lhs_length > rhs_length ? lhs_length : rhs_length;

  // Check if the result will be negative
  if (lhs_length < rhs_length ||
      (lhs_length == rhs_length && strcmp(lhs, rhs) < 0)) {
    return NULL;
  }

  char *result = init_buffer(max_length);

  int borrow = 0;

  // Subtract the digits from right to left
  for (size_t i = 0; i < max_length; ++i) {
    const int lhs_digit =
        (i < lhs_length) ? get_digit_value(lhs[lhs_length - i - 1]) : 0;
    const int rhs_digit =
        (i < rhs_length) ? get_digit_value(rhs[rhs_length - i - 1]) : 0;

    int diff = lhs_digit - rhs_digit - borrow;

    if (diff < 0) {
      diff += base;
      borrow = 1;
    } else {
      borrow = 0;
    }

    result[max_length - i - 1] = to_digit(diff);
  }

  // Drop leading zeros from the result
  char *final_result = drop_leading_zeros(result);

  if (final_result != result) {
    // If drop_leading_zeros returned a pointer inside result, we need to
    // allocate new memory for the final result
    final_result = strdup(final_result);
    free(result);
  }

  return final_result;
}

char *arithmatoy_mul(unsigned int base, const char *lhs, const char *rhs) {
  if (VERBOSE) {
    fprintf(stderr, "mul: entering function\n");
  }

  // Check for invalid inputs
  if (lhs == NULL || rhs == NULL || base >= ALL_DIGIT_COUNT) {
    return NULL;
  }

  // Drop leading zeros
  lhs = drop_leading_zeros(lhs);
  rhs = drop_leading_zeros(rhs);

  const size_t lhs_length = strlen(lhs);
  const size_t rhs_length = strlen(rhs);
  const size_t max_length = lhs_length + rhs_length;

  char *result = init_buffer(max_length);

  // Initialize the result with zeros
  memset(result, '0', max_length);
  result[max_length] = '\0';

  // Multiply each digit of lhs with each digit of rhs
  for (size_t i = 0; i < lhs_length; ++i) {
    unsigned int carry = 0;
    for (size_t j = 0; j < rhs_length; ++j) {
      // Multiply the digits
      const unsigned int lhs_digit = get_digit_value(lhs[lhs_length - i - 1]);
      const unsigned int rhs_digit = get_digit_value(rhs[rhs_length - j - 1]);
      const unsigned int mul = lhs_digit * rhs_digit;

      // Position in the result array
      const size_t pos = max_length - 1 - (i + j);

      // Add the product to the current position
      unsigned int sum = get_digit_value(result[pos]) + mul + carry;
      result[pos] = to_digit(sum % base);
      carry = sum / base;
    }

    // If there is a carry left, add it to the next position in result
    size_t k = max_length - 1 - (i + rhs_length);
    while (carry > 0) {
      unsigned int sum = get_digit_value(result[k]) + carry;
      result[k] = to_digit(sum % base);
      carry = sum / base;
      k--;
    }
  }

  // Drop leading zeros from the result
  char *final_result = drop_leading_zeros(result);

  if (final_result != result) {
    // If drop_leading_zeros returned a pointer inside result, we need to
    // allocate new memory for the final result
    final_result = strdup(final_result);
    free(result);
  }

  return final_result;
}

void debug_abort(const char *debug_msg) {
  // Print a message and exit
  fprintf(stderr, debug_msg);
  exit(EXIT_FAILURE);
}
