#ifndef DECODE_H
#define DECODE_H
#include <stdio.h>

#include "types.h" 

typedef struct DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Output File Info */
    char *output_fname;
    FILE *fptr_output;

    /* Decoded Secret Info */
    int extn_size;
    char extn_secret_file[10];
    int secret_file_size;

} DecodeInfo;


/* Encoding function prototype */

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *dencInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *dencInfo,char *argv[]);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *dencInfo);

/* Store Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *dencInfo);

/*Encode extension size*/
Status decode_secret_file_extn_size(DecodeInfo *dencInfo);

/* Encode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *dencInfo,char *argv[]);

/* Encode secret file size */
Status decode_secret_file_size(DecodeInfo *dencInfo);

/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *dencInfo);

/* Encode a byte into LSB of image data array */
char decode_byte_to_lsb(char *image_buffer);

// Encode a size to lsb
int decode_size_to_lsb(char *imageBuffer);

#endif
