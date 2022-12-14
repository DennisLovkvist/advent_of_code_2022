
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

static char* load_input(char *path)
{
    FILE *f = fopen(path, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *source_code = malloc(fsize + sizeof(char));
    fread(source_code, fsize, 1, f);
    fclose(f);
    source_code[fsize] = '\0';
    return source_code;
}
int main()
{
    //int unique_sequence_length = 4;//part1
    int unique_sequence_length = 14;//part2

    char* input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    int index = 0;
    while (index < length)
    {
        char c = input_raw[index];
        
        if(c != '\n')
        {
            int flag = 0;
            for (size_t i = 0; i < unique_sequence_length; i++)
            {
                for (size_t j = i+1; j < unique_sequence_length; j++)
                {
                    if(input_raw[index+i] == input_raw[index+j])
                    {
                        flag = 1;
                        goto end;
                    }
                }
            }
            end:
            if(!flag)
            {
                printf("%i\n",(index +unique_sequence_length));
                return 1;
            }
        }
        index++;
    }

    return 1;
}

