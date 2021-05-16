/*!
 * \file    rle.c
 * \author  Tomas Jurena
 * \date    6. 4. 2021
 * \brief   Implementation of RLE module.
 *
 * \attention
 * &copy; Copyright (c) 2021 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

/* Includes ------------------------------------------------------------------*/
#include "rle.h"

#include <stdio.h>

/* Private types -------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function declarations ---------------------------------------------*/
/* Exported functions definitions --------------------------------------------*/
RLE_State RLE_decode(const uint8_t *in, uint32_t len, RLE_Data *result) {
  int i = 0;
  int count = 0;

  while (in[i] < len) {
    if(in[i] == in[i-1]) {
      while (count > 0) {
        count--;
      }
    } else {
      result->size = count;
      result->data = (uint8_t *)&in[i];;
    }
    i++;
  }

  return RLE_ERROR;
}

RLE_State RLE_encode(const uint8_t *in, uint32_t len, RLE_Data *result) {
  int count;
  int size = 0;

  for (uint32_t i = 0; i < len; i++) {
    count = 1;
    size++;
    while (i + 1 < len && in[i] == in[i + 1]) {
      if (count >= 255) {
        printf("daco");
        break;
      }
      count++;
      i++;
    }
    printf("% d, %c,", count, in[i]);
    result->data = (uint8_t *)&in[i];
    result->size = size;
  }
  printf("\n%d", size);

  return RLE_ERROR;
}

/* Private function definitions ----------------------------------------------*/
