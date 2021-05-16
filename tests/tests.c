/*!
 * \file    tests.c
 * \author  Tomas Jurena
 * \date    6. 4. 2021
 * \brief   Simple unit test of RLE module.
 *
 * \attention
 * &copy; Copyright (c) 2021 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */
/* Private includes -------------------------------------------------------- */
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "minunit.h"
#include "rle.h"

////////////////////////////// IMPORTANT ///////////////////////////////////////
/////////// Source repository: https://github.com/siu/minunit //////////////////
/////////// For more info consult with this page ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

MU_TEST(test_decode_inputs) {
  RLE_Data data;
  uint8_t in[] = {0x81, 65};
  mu_assert(RLE_ERROR == RLE_decode(NULL, 0, NULL), "All parameters are wrong");
  mu_assert(RLE_ERROR == RLE_decode(in, 0, NULL),
            "Zero lenght and NULL result");
  mu_assert(RLE_ERROR == RLE_decode(in, 1, NULL), "NULL result");
  mu_assert(RLE_ERROR == RLE_decode(in, 1, &data), "Short input");
  mu_assert(RLE_OK == RLE_decode(in, 2, &data), "All OK");
}

MU_TEST(test_decode) {
  RLE_Data decoded;
  uint8_t data[] = {5, 65, 7, 66, 15, 67, 0x80 | 3, 65, 66, 67};
  uint8_t result[] = {65, 65, 65, 65, 65, 66, 66, 66, 66, 66,
                      66, 66, 67, 67, 67, 67, 67, 67, 67, 67,
                      67, 67, 67, 67, 67, 67, 67, 65, 66, 67};
  RLE_decode(data, sizeof(data), &decoded);
  mu_assert(sizeof(result) == decoded.size, "Wrong lenght of result array");
  mu_assert(memcmp(decoded.data, result, sizeof(result)) == 0,
            "Wrong result data");
  free(decoded.data);
}

MU_TEST(test_encode_inputs) {
  RLE_Data data;
  uint8_t in[] = {65};
  mu_assert(RLE_ERROR == RLE_encode(NULL, 0, NULL), "All parameters are wrong");
  mu_assert(RLE_ERROR == RLE_encode(in, 0, NULL),
            "Zero lenght and NULL result");
  mu_assert(RLE_ERROR == RLE_encode(in, 1, NULL), "NULL result");
  mu_assert(RLE_OK == RLE_encode(in, 1, &data), "All OK");
}

MU_TEST((test_encode_long_repetative_sequence)) {
  RLE_Data encoded;
  uint8_t data[] = {
      67, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
      65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
      65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
      65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
      65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
      65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
      65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
      65, 65, 65, 65, 65, 65, 65, 65, 65, 66, 67, 68, 69, 66, 66, 66, 66, 66};
  uint8_t result[] = {0x81, 67, 0x7f, 65, 0x7, 65, 0x84,
                      66,   67, 68,   69, 0x5, 66};
  RLE_encode(data, sizeof(data), &encoded);
  mu_assert(sizeof(result) == encoded.size, "Wrong lenght of result array");
  mu_assert(memcmp(encoded.data, result, sizeof(result)) == 0,
            "Wrong result data");
  free(encoded.data);
}

MU_TEST((test_encode_short_non_repetative_sequence)) {
  RLE_Data encoded;
  uint8_t data[] = {65, 66, 67};
  uint8_t result[] = {0x83, 65, 66, 67};
  RLE_encode(data, sizeof(data), &encoded);
  mu_assert(sizeof(result) == encoded.size, "Wrong lenght of result array");
  mu_assert(memcmp(encoded.data, result, sizeof(result)) == 0,
            "Wrong result data");
  free(encoded.data);
}

MU_TEST((test_encode_short_combine)) {
  RLE_Data encoded;
  uint8_t data[] = {65, 66, 67, 67, 67, 65, 65, 66};
  uint8_t result[] = {0x82, 65, 66, 0x3, 67, 0x2, 65, 0x81, 66};
  RLE_encode(data, sizeof(data), &encoded);
  mu_assert(sizeof(result) == encoded.size, "Wrong lenght of result array");
  mu_assert(memcmp(encoded.data, result, sizeof(result)) == 0,
            "Wrong result data");
  free(encoded.data);
}

MU_TEST((test_encode_short_combine_multiple_small_chunks)) {
  RLE_Data encoded;
  uint8_t data[] = {65, 66, 67, 67, 67, 65, 65, 66, 66};
  uint8_t result[] = {0x82, 65, 66, 0x3, 67, 0x2, 65, 0x2, 66};
  RLE_encode(data, sizeof(data), &encoded);
  mu_assert(sizeof(result) == encoded.size, "Wrong lenght of result array");
  mu_assert(memcmp(encoded.data, result, sizeof(result)) == 0,
            "Wrong result data");
  free(encoded.data);
}

MU_TEST((test_encode_ultra_short_combine)) {
  RLE_Data encoded;
  uint8_t data[] = {65, 66, 66};
  uint8_t result[] = {0x81, 65, 0x2, 66};
  RLE_encode(data, sizeof(data), &encoded);
  mu_assert(sizeof(result) == encoded.size, "Wrong lenght of result array");
  mu_assert(memcmp(encoded.data, result, sizeof(result)) == 0,
            "Wrong result data");
  free(encoded.data);
}

MU_TEST((test_encode_single_byte)) {
  RLE_Data encoded;
  uint8_t data[] = {65};
  uint8_t result[] = {0x81, 65};
  RLE_encode(data, sizeof(data), &encoded);
  mu_assert(sizeof(result) == encoded.size, "Wrong lenght of result array");
  mu_assert(memcmp(encoded.data, result, sizeof(result)) == 0,
            "Wrong result data");
  free(encoded.data);
}

MU_TEST((test_encode_single_sequence)) {
  RLE_Data encoded;
  uint8_t data[] = {65, 65, 65};
  uint8_t result[] = {0x3, 65};
  RLE_encode(data, sizeof(data), &encoded);
  mu_assert(sizeof(result) == encoded.size, "Wrong lenght of result array");
  mu_assert(memcmp(encoded.data, result, sizeof(result)) == 0,
            "Wrong result data");
  free(encoded.data);
}

MU_TEST((test_encode_non_repetative)) {
  RLE_Data encoded;
  uint8_t data[130];
  for (uint8_t i = 0; i < 130; ++i) {
    data[i] = i;
  }
  uint8_t result[] = {
      0xff, 0,   1,    2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,
      13,   14,  15,   16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,
      27,   28,  29,   30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
      41,   42,  43,   44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,
      55,   56,  57,   58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,
      69,   70,  71,   72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,
      83,   84,  85,   86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,
      97,   98,  99,   100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
      111,  112, 113,  114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124,
      125,  126, 0x83, 127, 128, 129};
  RLE_encode(data, sizeof(data), &encoded);
  mu_assert(sizeof(result) == encoded.size, "Wrong lenght of result array");
  mu_assert(memcmp(encoded.data, result, sizeof(result)) == 0,
            "Wrong result data");
  free(encoded.data);
}

MU_TEST_SUITE(test_suite) {
  MU_RUN_TEST(test_decode_inputs);
  MU_RUN_TEST(test_decode);
  MU_RUN_TEST(test_encode_inputs);
  MU_RUN_TEST(test_encode_long_repetative_sequence);
  MU_RUN_TEST(test_encode_short_non_repetative_sequence);
  MU_RUN_TEST(test_encode_short_combine);
  MU_RUN_TEST(test_encode_short_combine_multiple_small_chunks);
  MU_RUN_TEST(test_encode_ultra_short_combine);
  MU_RUN_TEST(test_encode_single_byte);
  MU_RUN_TEST(test_encode_single_sequence);
  MU_RUN_TEST(test_encode_non_repetative);
}

int main(void) {
  MU_RUN_SUITE(test_suite);
  MU_REPORT();
  return 0;
}
