/*!
 * \file    tests.cpp
 * \author  Tomas Jurena
 * \date    6. 4. 2021
 * \brief   Simple unit test of RLE module using GoogleTest framework.
 *
 * \attention
 * &copy; Copyright (c) 2021 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */
/* Includes ------------------------------------------------------------------*/
#include "gtest/gtest.h"

extern "C" {
#include "rle.h"
}

/* Private types -------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function declarations ---------------------------------------------*/
/* Exported functions definitions --------------------------------------------*/
/* Private function definitions ----------------------------------------------*/

// todo: parametrize tests, most of them only differs in input/output (could be
// solved with static fuction - does this brake an idiom?)

TEST(rle, decodeAllInputsWrong) {
  ASSERT_EQ(RLE_ERROR, RLE_decode(NULL, 0, NULL));
}

TEST(rle, decodeWrongLengthAndResult) {
  uint8_t in[] = {0x81, 65};
  ASSERT_EQ(RLE_ERROR, RLE_decode(in, 0, NULL));
}

TEST(rle, decodeWrongResultPointer) {
  uint8_t in[] = {0x81, 65};
  ASSERT_EQ(RLE_ERROR, RLE_decode(in, 1, NULL));
}

TEST(rle, decodeWrongInputLength) {
  RLE_Data data;
  uint8_t in[] = {0x81, 65};
  ASSERT_EQ(RLE_ERROR, RLE_decode(in, 1, &data));
}

TEST(rle, decodeCorrectInputs) {
  RLE_Data data;
  uint8_t in[] = {0x81, 65};
  ASSERT_EQ(RLE_OK, RLE_decode(in, sizeof(in), &data));
}

TEST(rle, decodeSimpleRepeat) {
  RLE_Data decoded = {NULL, 0};
  uint8_t data[] = {5, 65};
  uint8_t result[] = {65, 65, 65, 65, 65};

  RLE_decode(data, sizeof(data), &decoded);

  ASSERT_EQ(sizeof(result), decoded.size);
  ASSERT_EQ(0, memcmp(decoded.data, result, sizeof(result)));

  free(decoded.data);
}

TEST(rle, decodeSimpleNonSequence) {
  RLE_Data decoded = {NULL, 0};
  uint8_t data[] = {1, 65, 1, 66, 1, 67};
  uint8_t result[] = {65, 66, 67};

  RLE_decode(data, sizeof(data), &decoded);

  ASSERT_EQ(sizeof(result), decoded.size);
  ASSERT_EQ(0, memcmp(decoded.data, result, sizeof(result)));

  free(decoded.data);
}

TEST(rle, decodeCombined) {
  RLE_Data decoded = {NULL, 0};
  uint8_t data[] = {5, 65, 7, 66, 15, 67, 1, 65, 1, 66, 1, 67};
  uint8_t result[] = {65, 65, 65, 65, 65, 66, 66, 66, 66, 66,
                      66, 66, 67, 67, 67, 67, 67, 67, 67, 67,
                      67, 67, 67, 67, 67, 67, 67, 65, 66, 67};

  RLE_decode(data, sizeof(data), &decoded);

  ASSERT_EQ(sizeof(result), decoded.size);
  ASSERT_EQ(0, memcmp(decoded.data, result, sizeof(result)));

  free(decoded.data);
}

TEST(rle, encodeAllInputsWrong) {
  ASSERT_EQ(RLE_ERROR, RLE_encode(NULL, 0, NULL));
}

TEST(rle, encodeWrongLengthAndResult) {
  uint8_t in[] = {65};
  ASSERT_EQ(RLE_ERROR, RLE_encode(in, 0, NULL));
}

TEST(rle, encodeWrongResultPointer) {
  uint8_t in[] = {1, 65};
  ASSERT_EQ(RLE_ERROR, RLE_encode(in, 1, NULL));
}

TEST(rle, encodeCorrectInputs) {
  RLE_Data data;
  uint8_t in[] = {1, 65};
  ASSERT_EQ(RLE_OK, RLE_encode(in, 1, &data));
}

TEST(rle, encodeLongRepetativeSequence) {
  RLE_Data encoded = {NULL, 0};

  uint8_t data[300] = {67};
  uint8_t *pData = data;
  ++pData;
  memset(pData, 65, 260);
  pData += 260;
  *pData++ = 66;
  *pData++ = 67;

  uint8_t result[] = {1, 67, 255, 65, 5, 65, 1, 66, 1, 67};
  RLE_encode(data, pData - data, &encoded);

  ASSERT_EQ(sizeof(result), encoded.size);
  ASSERT_EQ(0, memcmp(encoded.data, result, sizeof(result)));

  free(encoded.data);
}

TEST(rle, encodeShortNonRepetativeSequence) {
  RLE_Data encoded = {NULL, 0};
  uint8_t data[] = {65, 66, 67};
  uint8_t result[] = {1, 65, 1, 66, 1, 67};

  RLE_encode(data, sizeof(data), &encoded);

  ASSERT_EQ(sizeof(result), encoded.size);
  ASSERT_EQ(0, memcmp(encoded.data, result, sizeof(result)));

  free(encoded.data);
}

TEST(rle, encodeShortCombinedSequence) {
  RLE_Data encoded = {NULL, 0};
  uint8_t data[] = {65, 66, 67, 67, 67, 65, 65, 66};
  uint8_t result[] = {1, 65, 1, 66, 3, 67, 2, 65, 1, 66};

  RLE_encode(data, sizeof(data), &encoded);

  ASSERT_EQ(sizeof(result), encoded.size);
  ASSERT_EQ(0, memcmp(encoded.data, result, sizeof(result)));

  free(encoded.data);
}

TEST(rle, encodeShortCombineMultipleSmallChunks) {
  RLE_Data encoded = {NULL, 0};
  uint8_t data[] = {65, 66, 67, 67, 67, 65, 65, 66, 66};
  uint8_t result[] = {1, 65, 1, 66, 3, 67, 2, 65, 2, 66};

  RLE_encode(data, sizeof(data), &encoded);

  ASSERT_EQ(sizeof(result), encoded.size);
  ASSERT_EQ(0, memcmp(encoded.data, result, sizeof(result)));

  free(encoded.data);
}

TEST(rle, encodeUltraShortCombine) {
  RLE_Data encoded = {NULL, 0};
  uint8_t data[] = {65, 66, 66};
  uint8_t result[] = {1, 65, 2, 66};

  RLE_encode(data, sizeof(data), &encoded);

  ASSERT_EQ(sizeof(result), encoded.size);
  ASSERT_EQ(0, memcmp(encoded.data, result, sizeof(result)));
  free(encoded.data);
}

TEST(rle, encodeSingleByte) {
  RLE_Data encoded = {NULL, 0};
  uint8_t data[] = {65};
  uint8_t result[] = {1, 65};

  RLE_encode(data, sizeof(data), &encoded);

  ASSERT_EQ(sizeof(result), encoded.size);
  ASSERT_EQ(0, memcmp(encoded.data, result, sizeof(result)));

  free(encoded.data);
}

TEST(rle, testEncodeSingleSequence) {
  RLE_Data encoded = {NULL, 0};
  uint8_t data[] = {65, 65, 65};
  uint8_t result[] = {3, 65};

  RLE_encode(data, sizeof(data), &encoded);

  ASSERT_EQ(sizeof(result), encoded.size);
  ASSERT_EQ(0, memcmp(encoded.data, result, sizeof(result)));
  free(encoded.data);
}
