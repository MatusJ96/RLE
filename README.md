# Algoritmus RLE

Implementujte jednoduchý RLE algoritmus pro komprimování/dekomprimování dat. Teorii si nastudujte např. [zde](http://www.cs.vsb.cz/benes/vyuka/pte/texty/komprese/ch02s01.html) a [zde](https://en.wikipedia.org/wiki/Run-length_encoding).

| input                                    | output                     |
| ---------------------------------------- | -------------------------- |
| 65, 65, 65, 65                           | 4, 65                      |
| 65, 65, 66, 67                           | 2, 65, 1, 66, 1, 67        |
| 65, 66, 67, 68                           | 1, 65, 1, 66, 1, 67, 1, 68 |
| 65, 65, ...(65 repeated 263 times).., 65 | 255, 65, 10, 65            |


Program musí umožňovat dekompresi již komprimovaných dat, dle předchozího popisu.

## Spuštění programu

Program vyžaduje pro svoji činnost několik parametrů příkazové řádky a volání programu je následovné:

`./rle <input> <operation type> <output>`

Vyžadovány jsou tedy tři argumenty jejich význam je:

 - input - Cesta ke vstupnímu souboru s daty.
 - operation type - Typ operace, která bude se vstupními daty prováděna. Jsou povoleny dvě hodnoty a to **d** a **e**, kde:
   - **d** (decode) znamená dekódování zakódovaného vstupu,
   - **e** (encode) znamená zakódování vstupu.
 - output - Cesta k výstupnímu souboru, kam bude uložen výstup po spuštění RLE algoritmu.
 
## Výstup programu

1. `jurenat@jurenat-HP-Z4-G4-Workstation:/tmp/rle/build/bin$ ./app test_files/input.bin e test_files/encoded.bin`

```
Encode done
Compression ratio: 1.095238 %
```

Obsah komprimovaného souboru:

```bash
jurenat@jurenat-HP-Z4-G4-Workstation:/tmp/rle/build/bin$ hexdump test_files/encoded.bin
0000000 6505 6486 6766 6572 0671 0264 0265 8177
0000010 0271 8567 6466 6673 0364 8176 0366 0264
0000020 0272 8665 7277 6567 6772               
000002a
```

2. `jurenat@jurenat-HP-Z4-G4-Workstation:/tmp/rle/build/bin$ ./app test_files/encoded.bin d test_files/decoded.bin`

```
Decode done
Compression ratio: 0.913043 %
```

Obsah dekomprimovaného souboru:

```bash
jurenat@jurenat-HP-Z4-G4-Workstation:/tmp/rle/build/bin$ hexdump test_files/decoded.bin
0000000 6565 6565 6465 6766 6572 6471 6464 6464
0000010 6564 7765 7177 6767 6466 6673 7664 7676
0000020 6466 6464 7272 6565 7277 6567 6772     
000002e
```

# RLE algorithm

Implement simple RLE algorithm for data compression/decompression. Theory can be found for example [here](http://www.cs.vsb.cz/benes/vyuka/pte/texty/komprese/ch02s01.html) or [here](https://en.wikipedia.org/wiki/Run-length_encoding).

| input                                    | output                     |
| ---------------------------------------- | -------------------------- |
| 65, 65, 65, 65                           | 4, 65                      |
| 65, 65, 66, 67                           | 2, 65, 1, 66, 1, 67        |
| 65, 66, 67, 68                           | 1, 65, 1, 66, 1, 67, 1, 68 |
| 65, 65, ...(65 repeated 263 times).., 65 | 255, 65, 10, 65            |

## Executing program

The program requires few command-line arguments to a function and a generic call looks like this:

`./rle <input> <operation type> <output>`

Three arguments are required and their meaning is following:

 - input - Path to file with input data.
 - operation type - Type of operation that will be done with input data. Two values are allowed and they are **d** and **e** where:
   - **d** (decode) means decompression of encoded input,
   - **e** (encode) encoding the plain input with RLE algorithm,
 - output - Path to the output file, where the result will be saved.
 
## Program output

1. `jurenat@jurenat-HP-Z4-G4-Workstation:/tmp/rle/build/bin$ ./app test_files/input.bin e test_files/encoded.bin`

```
Encode done
Compression ratio: 1.095238 %
```

Content of compressed file:

```bash
jurenat@jurenat-HP-Z4-G4-Workstation:/tmp/rle/build/bin$ hexdump test_files/encoded.bin
0000000 6505 6486 6766 6572 0671 0264 0265 8177
0000010 0271 8567 6466 6673 0364 8176 0366 0264
0000020 0272 8665 7277 6567 6772               
000002a
```

2. `jurenat@jurenat-HP-Z4-G4-Workstation:/tmp/rle/build/bin$ ./app test_files/encoded.bin d test_files/decoded.bin`

```
Decode done
Compression ratio: 0.913043 %
```

Content of decompressed file:

```bash
jurenat@jurenat-HP-Z4-G4-Workstation:/tmp/rle/build/bin$ hexdump test_files/decoded.bin
0000000 6565 6565 6465 6766 6572 6471 6464 6464
0000010 6564 7765 7177 6767 6466 6673 7664 7676
0000020 6466 6464 7272 6565 7277 6567 6772     
000002e
```
