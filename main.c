#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include "decode.h"

OperationType check_operation_type(char *);

int main(int argc, char *argv[])
{
    int r = check_operation_type(argv[1]);

    if(r == e_encode)
    {
        EncodeInfo encInfo;

        if(read_and_validate_encode_args(argv,&encInfo)==e_failure)
        {
            printf("ERROR: Invalid code arguments\n");
            return 1;
        }
        
        if(do_encoding(&encInfo)==e_failure)
        {
            printf("Error: encoding failed\n");
            return 1;
        }

    }
    
    else if(r == e_decode)
    {
        DecodeInfo dencInfo;
        if(read_and_validate_decode_args(argv,&dencInfo) == e_failure)
        {
            printf("Error: Invalid code arguments\n");
            return 1;
        }
        if(do_decoding(&dencInfo,argv) == e_failure)
        {
            printf("Error: decoding failed\n");
        }
    }
    else
    {
        printf("Not valid option\n");
        printf("use\n");
        printf("-e for encoding\n");
        printf("-d for decoding\n");
    }
    //step3 -> return value == e_decode
            // --
    //step3 -> return value == e_unsupported
            // --> print invalid arg
            // -e -> encode
            // -d  -> decode
    return e_unsupported;
}

OperationType check_operation_type(char *symbol)
{

    if(strcmp(symbol,"-e") == 0)
        return e_encode;
    else if(strcmp(symbol,"-d") == 0)
        return e_decode;
    else 
        return e_unsupported;
}
