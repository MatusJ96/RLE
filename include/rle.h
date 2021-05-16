/*!
 * \file    rle.h
 * \author  Tomas Jurena
 * \date    6. 4. 2021
 * \brief   Declaration of RLE module.
 *
 * \defgroup RLE  RLE module
 *
 * This module defines two functions that encode and decode the given input
 * using the RLE compression algorithm. The RLE implementation uses minor
 * optimization to generate smaller results when encoding. The optimization lies
 * in detecting the non-repetitive bytes and their encoding as repetitive. This
 * although may increase the size for inputs that contains a lot of bytes in
 * large series because to distinguish these two states apart, it uses the MSB
 * bit. When while encoding the series of non-repetitive bytes are detected, the
 * MSB bit is set, and the 0:6 bits have stored the length of the non-repetitive
 * series. It means that a maximum of 127 bytes can be encoded in one byte. If
 * this optimization is not used, the limit is 255 bytes, but the non-repetitive
 * bytes will take one extra byte for each non-repetitive byte. This approach
 * could be optimized even more if before processing the input data are analyzed
 * and is decided which version is more beneficial.
 *
 * \attention
 * &copy; Copyright (c) 2021 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * \{
 */
#ifndef RLE_H
#define RLE_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/*! Structure that holds the dynamically allocated array and its size. */
typedef struct {
  uint8_t *data; /*!< Pointer to heap allocated RLE data. */
  uint32_t size; /*!< Length of encoded/decoded data allocated on heap. */
} RLE_Data;

/*! Definition of RLE exit codes. */
typedef enum {
  RLE_OK,   /*!< RLE process finnished without error. */
  RLE_ERROR /*!< Error detected while RLE. */
} RLE_State;

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions declarations -------------------------------------------*/

/*! Decodes RLE \a in byte array to original data. It returns \ref RLE_Data
 * structure with decoded data as pointer to heap. It means that this function
 * allocate certain space on heap where the data are stored. If any error is
 * detected, no memory is allocated.
 *
 * See the RLE_encode for information about encoding process.
 *
 * \param[in]   in      Encoded input array.
 * \param[in]   len     Length of input array.
 * \param[out]  result  Pointer to RLE_Data structure, where the decoded result
 * will be stored.
 *
 * \return If error occure the \ref RLE_ERROR is returned, \ref RLE_OK
 * otherwise.
 */
RLE_State RLE_decode(const uint8_t *in, uint32_t len, RLE_Data *result);

/*! Encodes \a in byte array to RLE coded byte array. It
 * returns \ref RLE_Data structure with encoded data as pointer to heap. It
 * means that this function allocate certain space on heap where the data are
 * stored. If any error is detected, no memory is allocated.
 *
 * The encode function simply reads the input buffer and counts how many
 * following bytes have the same value, and this value is recorded to the output
 * array with the encoded value after the number of bytes. This implies that the
 * result stream is a sequence of byte pairs where the first position is the
 * number of repetitions and the second number is the repeated value. The
 * encoding into two bytes means that the maximum number of repetitions is 255,
 * when the counter reaches this value, the byte is yielded and the counter is
 * reset.
 *
 * For example:
 * input                                    | output
 * -----------------------------------------|---------------------
 * 65, 65, 65, 65                           | 4, 65
 * 65, 65, 66, 67                           | 2, 65, 1, 66, 1, 67
 * 65, 66, 67, 68                           | 1, 65, 1, 66, 1, 67, 1, 68
 * 65, 65, ...(65 repeated 263 times).., 65 | 255, 65, 10, 65
 *
 * \param[in]   in      Input array
 * \param[in]   len     Length of input array
 * \param[out]  result  Pointer to RLE_Data structure, where result will be
 * stored.
 *
 * \return If error occure the \ref RLE_ERROR is returned, \ref RLE_OK
 * otherwise.
 */
RLE_State RLE_encode(const uint8_t *in, uint32_t len, RLE_Data *result);

/*! \} */
#endif  // RLE_H
