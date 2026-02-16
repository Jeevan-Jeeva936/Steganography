#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "common.h"
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr)
{
    // Find the size of secret file data
    uint size;
    fseek(fptr,0,SEEK_END);
    size = ftell(fptr);
    rewind(fptr);
    return size;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    char *ext = strrchr(argv[2],'.');

    if(ext == NULL || strcmp(ext,".bmp") != 0)
    {
        return e_failure;
    }

    encInfo->src_image_fname = argv[2];

    ext = strrchr(argv[3],'.');

    if(ext == NULL || strcmp(ext,".txt") != 0)
    {
        return e_failure;
    }

    encInfo->secret_fname = argv[3];
    
    if(argv[4] != NULL)
    {
        ext = strrchr(argv[4],'.');
        if(ext == NULL || strcmp(ext,".bmp") != 0)
        {
            return e_failure;
        }
        encInfo->stego_image_fname = argv[4];
    }
    else
    {
        encInfo->stego_image_fname = "stego.bmp";
    }
     
    return e_success;
}

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
   
    if((encInfo->image_capacity) > 16+32+32+32+54+(encInfo->size_secret_file*8))
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    //step -> rewind file pointer to 0th position
    rewind(fptr_src_image);
   //step2 -> read 54 bytes from source file(use fread);
   char buffer[54];
   fread(buffer,1,54,fptr_src_image);
   //step3 -> write the 54 bytes to stego image file(from buffer)
   fwrite(buffer,1,54,fptr_dest_image);
   //step4 -> return e_success
   return e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    //step1 -> read 8 bytes of buffer from source image
    char buffer[8];
    //step2 -> call encode_byte_to_lsb(magic_string[0],buffer)
    int i = 0;
    int size = strlen(magic_string);
    for(i = 0;i<size;i++)
    {
        fread(buffer,1,8,encInfo->fptr_src_image);
        encode_byte_to_lsb(magic_string[i],buffer);
        fwrite(buffer,1,8,encInfo->fptr_stego_image);
    }
    //step3 -> store the buffer to stego image files
        //do this upto magic_string size time (step1)
    return e_success;
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    //step1 -> read 32 bytes of buffer from source image
    char buffer[32];
    fread(buffer,1,32,encInfo->fptr_src_image);
    //step2 -> call encode_size_to_lsb(size,image_buffer)
    encode_size_to_lsb(size,buffer);
    //step3 -> store the buffer to stego image files
    fwrite(buffer,1,32,encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    //step1 -> read 8 bytes of buffer from source image file
    char buffer[8];
    int len = strlen(file_extn);
    for(int i = 0;i < len;i++)
    {
        fread(buffer,1,8,encInfo->fptr_src_image);
        encode_byte_to_lsb(file_extn[i],buffer);
        fwrite(buffer,1,8,encInfo->fptr_stego_image);
    }
    //step2 -> call encode_byte_to_lsb(file_extn[0],buffer)
    //step3 -> store the buffer to stego image files
        //repeat this upto size of extn size
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    //step1 -> read 32 bytes of buffer from source image
    char buffer[32];
    fread(buffer,1,32,encInfo->fptr_src_image);
    //step2 -> call encode_size_to_lsb(file_size,image_buffer)
    encode_size_to_lsb(file_size,buffer);
    //step3 -> store the buffer to stego image files
    fwrite(buffer,1,32,encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    //step1 -> read secret data into one data_buffer
    for(int i = 0;i < encInfo->size_secret_file; i++)
    {
        char data_buffer[1];
        char image_buffer[8];
        fread(data_buffer,1,1,encInfo->fptr_secret);
        fread(image_buffer,1,8,encInfo->fptr_src_image);
        encode_byte_to_lsb(data_buffer[0],image_buffer);
        fwrite(image_buffer,1,8,encInfo->fptr_stego_image);
    }
    //step2 -> read 8 bytes of buffer from source image file
    //step3 -> call encode_byte_to_lsb(data_buffer[0],buffer)
    //step4 -> store the buffer to stego image files
        //repeat this upto size of secret file size
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    // run a loop upto reaching EOF
    int ch;
    while((ch = fgetc(fptr_src)) != EOF)
    {
        fputc(ch,fptr_dest);
    }
    // read buffer from source image file                         
    // store into stego image file
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
   //step1 ->logic to ecode the data.
   int len = 8;
   for(int i=0; i<len; i++)
   {
        image_buffer[i] = (image_buffer[i] & ~1) | ((data >> (7 - i)) & 1);
   }
   return e_success;
}

Status encode_size_to_lsb(int size, char *imageBuffer)
{
    //step1 ->logic to ecode the data.
    for(int i=0;i<32;i++)
    {
        imageBuffer[i] = (imageBuffer[i] & ~1) | ((size >> (31 - i)) & 1);
    }
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    if (open_files(encInfo) == e_failure)
    {
        printf("Error: Unable to open files\n");
        return e_failure;
    }
    printf("Files opened successfully\n");

    if (check_capacity(encInfo) == e_failure)
    {
        printf("Error: Insufficient image capacity\n");
        return e_failure;
    }
    printf("Image has sufficient capacity\n");

    if (copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_failure)
    {
        printf("Error: Failed to copy BMP header\n");
        return e_failure;
    }
    printf("BMP header copied successfully\n");

    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        printf("Error: Failed to encode magic string\n");
        return e_failure;
    }
    printf("Magic string encoded successfully\n");

    if (encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_failure)
    {
        printf("Error: Failed to encode extension size\n");
        return e_failure;
    }
    printf("Secret file extension size encoded successfully\n");

    if (encode_secret_file_extn(encInfo->extn_secret_file,encInfo) == e_failure)
    {
        printf("Error: Failed to encode file extension\n");
        return e_failure;
    }
    printf("Secret file extension encoded successfully\n");

    if (encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_failure)
    {
        printf("Error: Failed to encode secret file size\n");
        return e_failure;
    }
    printf("Secret file size encoded successfully\n");

    if (encode_secret_file_data(encInfo) == e_failure)
    {
        printf("Error: Failed to encode secret file data\n");
        return e_failure;
    }
    printf("Secret file data encoded successfully\n");

    if (copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_failure)
    {
        printf("Error: Failed to copy remaining image data\n");
        return e_failure;
    }
    printf("Remaining image data copied successfully\n");

    return e_success;
}
