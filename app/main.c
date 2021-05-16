/*!
 * \file    main.c
 * \author  Tomas Jurena
 * \date    6. 4. 2021
 * \brief   Main file of the project implements application logic.
 *
 * \attention
 * &copy; Copyright (c) 2021 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */
/* Includes ------------------------------------------------------------------*/
#include <assert.h>  /* assert */
#include <stdbool.h> /* true, false */
#include <stdint.h>  /* uint8_t, uint32_t */
#include <stdio.h>   /* FILE, ftell, fopen, rewind, fseek, fprintf,  ... */
#include <stdlib.h>  /* malloc, EXIT_SUCCESS */

#include "rle.h"

/* Private typedef -----------------------------------------------------------*/
/*! The Config struct contains information about current run configuration based
 * on commandline arguments.
 */
struct Config {
  /*! Path to input file. */
  char* inputFileName;

  /*! Path to output file. */
  char* outputFileName;

  /*! Structure for desired action. */
  struct Action {
    /*! Pointer to effective function that do the RLE action
     * (encoding/decoding). */
    RLE_State (*fnc)(const uint8_t*, uint32_t, RLE_Data*);

    /*! Message for error state. */
    const char* errMsg;

    /*! Message for success. */
    const char* okMSg;
  } rleAction;
};

/* Private macro -------------------------------------------------------------*/
/*! \defgroup EXIT_CODES Exit codes
 *  List of program non-standard exit codes.
 *  \{
 */

/*! The program arrguments were not parsed correctly. */
#define EXIT_FAILURE_ARGUMENTS (2)

/*! Some file (input/output) was not opened. */
#define EXIT_FAILURE_FILE (3)

/*! The program was unable to allocate necessary memory. */
#define EXIT_FAILURE_MEMORY (4)

/*! RLE was not successful. */
#define EXIT_RLE_ERROR (5)

/*! \}*/

/*! Allocation step for file loading. */
#define ALLOC_STEP (0x400)

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/
static bool parseArgs(int argc, char** argv, struct Config* cfg);
static void printHelp(char* bin);
static int loadFileData(char* fileName, RLE_Data* fileData);

/* Exported functions definitions --------------------------------------------*/
int main(int argc, char** argv) {
  int programResult = EXIT_SUCCESS;
  struct Config cfg;
  struct Action* action = &cfg.rleAction;
  if (!parseArgs(argc, argv, &cfg)) {
    programResult = EXIT_FAILURE_ARGUMENTS;
    goto exit;
  }

  // load input data
  RLE_Data in = {NULL, 0};
  if ((programResult = loadFileData(cfg.inputFileName, &in)) != 0) {
    goto exit;
  }

  // do RLE action
  RLE_Data result = {NULL, 0};
  bool rleSuccessful = action->fnc(in.data, in.size, &result) == RLE_OK;
  free(in.data);  // free data as soon as possible

  // exit when RLE was not successful
  if (rleSuccessful == false) {
    fprintf(stderr, "Error while %s\n", action->errMsg);
    programResult = EXIT_RLE_ERROR;
    goto exit;
  }

  fprintf(stdout, "%s done\nCompression ratio: %f %%\n", action->okMSg,
          (double)in.size / result.size);
  FILE* of = fopen(cfg.outputFileName, "wb");

  if (of == NULL) {
    programResult = EXIT_FAILURE_FILE;
  } else if (fwrite(result.data, 1, result.size, of) != result.size) {
    fprintf(stderr, "Error writing output. Error '%d'", ferror(of));
  }

  free(result.data);
  fclose(of);

exit:
  return programResult;
}

/* Private function definitions ----------------------------------------------*/
/*! Parse program arguments and returns false if incorect number of arguments is
 * passed or unknown action is required. Function prints help if error is
 * detected.
 *
 * \param[in] argc  Size of arguments array.
 * \param[in] argv  Values in arguments array.
 * \param[in] cfg   Pointer to struncture with configuration.
 *
 * \return Returns true if arguments correctly parsed, false otherwise.
 */
static bool parseArgs(int argc, char** argv, struct Config* cfg) {
  assert(cfg != NULL);
  if (argc != 4) {
    printHelp(argv[0]);
    return false;
  }

  cfg->inputFileName = argv[1];
  cfg->outputFileName = argv[3];

  if (argv[2][0] == 'd') {
    cfg->rleAction.fnc = RLE_decode;
    cfg->rleAction.errMsg = "decoding";
    cfg->rleAction.okMSg = "Decode";
  } else if (argv[2][0] == 'e') {
    cfg->rleAction.fnc = RLE_encode;
    cfg->rleAction.errMsg = "encoding";
    cfg->rleAction.okMSg = "Encode";
  } else {
    fprintf(stderr, "Unknown value '%s'\n", argv[2]);
    printHelp(argv[0]);
    return false;
  }

  return true;
}

/*! Writes help message to stderr.
 *
 * \param[in] bin Path to current binary. Used for effective call generation.
 */
static void printHelp(char* bin) {
  assert(bin != NULL);
  fprintf(stderr,
          "Wrong call\n"
          "Usage %s <input file> <type> <output file>\n"
          "\ttype:\n"
          "\t\td - decode file\n"
          "\t\te - encode file\n",
          bin);
}

/*! Load content of the file to the dynamically allocated memory.
 *
 * \param[in] fileName    Name of file to load.
 * \param[out] fileData   Pointer to the loaded data structure.
 *
 * \return Returns non-zero value on error.
 */
static int loadFileData(char* fileName, RLE_Data* fileData) {
  assert(fileName != NULL);
  assert(fileData != NULL);

  int ret = EXIT_SUCCESS;
  size_t allocated = ALLOC_STEP;
  size_t readBytes;
  uint8_t* pFileContent;
  FILE* file;

  fileData->data = malloc(ALLOC_STEP);
  if (fileData->data == NULL) {
    ret = EXIT_FAILURE_MEMORY;
    goto exit;
  }

  pFileContent = fileData->data;

  file = fopen(fileName, "rb");
  if (!file) {
    ret = EXIT_FAILURE_FILE;
    free(fileData->data);
    goto exit;
  }

  while ((readBytes = fread(pFileContent, 1, ALLOC_STEP, file)) == ALLOC_STEP) {
    uint8_t* p = realloc(fileData->data, allocated + ALLOC_STEP);

    if (p == NULL) {
      fputs("Realloc error! The file may contain more bytes\n", stderr);
      break;
    }

    fileData->data = p;
    pFileContent = p + allocated;
    allocated += ALLOC_STEP;
  }

  allocated -= ALLOC_STEP - readBytes;
  fileData->size = allocated;
  fileData->data = realloc(fileData->data, allocated);

  if (ferror(file)) {
    free(fileData->data);
    fileData->size = 0;
    ret = EXIT_FAILURE_FILE;
  }

  fclose(file);

exit:
  return ret;
}
