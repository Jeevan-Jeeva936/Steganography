#include <stdio.h>
#include "types.h"
#include <string.h>
#include "common.h"
#include "decode.h"
#include<stdlib.h>

Status read_and_validate_decode_args(char *argv[], DecodeInfo *dencInfo)
{
    char *ext = strrchr(argv[2],'.');

    if(ext == NULL || strcmp(ext,".bmp") != 0)
    {
        return e_failure;
    }
    dencInfo->stego_image_fname = argv[2];

    if(argv[3] != NULL)
    {
        strcmp(dencInfo->output_fname,argv[3]);
    }
    else
    {
        dencInfo->output_fname[0] = '\0';
    }
    return e_success;
}

Status open_decode_files(DecodeInfo *dencInfo)
{
    dencInfo->fptr_stego_image = fopen(dencInfo->stego_image_fname,"rb");

    if(dencInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr,"Error: unable ro open file %s\n",dencInfo->stego_image_fname);

        return e_failure;
    }
    return e_success;
}

Status decode_magic_string(const char *magic_string, DecodeInfo *dencInfo)
{
    fseek(dencInfo->fptr_stego_image,54,SEEK_SET);
    char buffer[8];
    int len = strlen(magic_string);
    char decode_magic[len+1];
    
    for(int i = 0;i<len;i++)
    {
        if(fread(buffer,1,8,dencInfo->fptr_stego_image)!=8)
            return e_failure;
        decode_magic[i] = decode_byte_to_lsb(buffer);
        if(decode_magic[i] != magic_string[i])
        {
            return e_failure;
        }
    }
    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *dencInfo)
{
    char buffer[32];
    if(fread(buffer,1,32,dencInfo->fptr_stego_image)!=32)
        return e_failure;
    dencInfo->extn_size = decode_size_to_lsb(buffer);
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *dencInfo,char *argv[])
{
    char buffer[8];
    int len = dencInfo->extn_size;
    char extension[len+1];
    //dencInfo->output_fname = malloc(100);
    for(int i = 0; i < len; i++)
    {
        if(fread(buffer,1,8,dencInfo->fptr_stego_image)!=8)
            return e_failure;
        extension[i] = decode_byte_to_lsb(buffer);
    }
    extension[len] = '\0';

    strcpy(dencInfo->extn_secret_file,extension);

    // if(argv[3] != NULL)
    // {
    //     char *ext = strrchr(argv[3],'.');
    //     if(ext != NULL)
    //     {
    //         int base = ext - argv[3];
    //         strncpy(dencInfo->output_fname,argv[3],base);
    //         dencInfo->output_fname[base] = '\0';
    //     }
    //     else
    //     {
    //         strcpy(dencInfo->output_fname, argv[3]);
    //     }
    //     strcat(dencInfo->output_fname, dencInfo->extn_secret_file);
    // }
    // else
    // {
    //     strcpy(dencInfo->output_fname,"output");
    //     strcat(dencInfo->output_fname,dencInfo->extn_secret_file);
    // }
    if(dencInfo->output_fname[0] == '\0')
    {
        strcpy(dencInfo->output_fname,"output");
        strcat(dencInfo->output_fname,extension);
        return e_success;
    }

    char *ext = strrchr(dencInfo->output_fname,'.');

    if(ext != NULL)
    {
        if(strcmp(ext,extension) != 0)
        {
            int base_len = ext - dencInfo->output_fname;
            dencInfo->output_fname[base_len] = '\0';
            strcat(dencInfo->output_fname,extension);
        }
        else
        {
            strcat(dencInfo->output_fname,extension);
        }
    }
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *dencInfo)
{
    char buffer[32];
    if(fread(buffer,1,32,dencInfo->fptr_stego_image)!=32)
        return e_failure;
    dencInfo->secret_file_size = decode_size_to_lsb(buffer);
    return e_success;
}


Status decode_secret_file_data(DecodeInfo *dencInfo)
{
    
    dencInfo->fptr_output = fopen(dencInfo->output_fname, "wb");
    if(dencInfo->fptr_output == NULL)
    {
        perror("fopen");
        return e_failure;
    }
    for(int i = 0;i<dencInfo->secret_file_size;i++)
    {
        char buffer[8];
        if(fread(buffer,1,8,dencInfo->fptr_stego_image)!=8)
            return e_failure;
        char decoded = decode_byte_to_lsb(buffer);
        fwrite(&decoded,1,1,dencInfo->fptr_output);
    }
    return e_success;
}

char decode_byte_to_lsb(char *image_buffer)
{
    int len = 8;
    char c = 0;
    for(int i = 0;i < len; i++)
    {
        c = c<<1;
        c |= (image_buffer[i]&1);
    }
    return c;
}

int decode_size_to_lsb(char *image_buffer)
{
    int len = 32;
    int size = 0;
    for(int i = 0;i < 32; i++)
    {
        size = size << 1;
        size |= (image_buffer[i] & 1);
    }
    return size;
}

Status do_decoding(DecodeInfo *dencInfo,char *argv[])
{
    if (open_decode_files(dencInfo) == e_failure)
    {
        printf("Error: Unable to open stego image file\n");
        return e_failure;
    }
    printf("Stego image opened successfully\n");

    if (decode_magic_string(MAGIC_STRING, dencInfo) == e_failure)
    {
        printf("Error: Magic string mismatch\n");
        fclose(dencInfo->fptr_stego_image);
        return e_failure;
    }
    printf("Magic string verified successfully\n");

    if (decode_secret_file_extn_size(dencInfo) == e_failure)
    {
        printf("Error: Failed to decode extension size\n");
        fclose(dencInfo->fptr_stego_image);
        return e_failure;
    }
    printf("Extension size decoded successfully\n");

    if (decode_secret_file_extn(dencInfo,argv) == e_failure)
    {
        printf("Error: Failed to decode extension\n");
        fclose(dencInfo->fptr_stego_image);
        return e_failure;
    }
    printf("Extension decoded successfully\n");

    if (decode_secret_file_size(dencInfo) == e_failure)
    {
        printf("Error: Failed to decode secret file size\n");
        fclose(dencInfo->fptr_stego_image);
        return e_failure;
    }
    printf("Secret file size decoded successfully\n");

    if (decode_secret_file_data(dencInfo) == e_failure)
    {
        printf("Error: Failed to decode secret file data\n");
        fclose(dencInfo->fptr_stego_image);
        return e_failure;
    }
    printf("Secret file data decoded successfully\n");

    fclose(dencInfo->fptr_stego_image);
    fclose(dencInfo->fptr_output);

    printf("Decoding completed successfully\n");

    return e_success;
}
