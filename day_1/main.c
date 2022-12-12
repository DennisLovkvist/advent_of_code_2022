#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char* input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    char line[10];
    memset(&line,'\0',sizeof(char)*10);

    int carret = 0;
    int value = 0;
    int index = 0;

    int max_value[3] = {0,0,0};
    int max_index[3] = {0,0,0};

    for (size_t i = 0; i < length; i++)
    {
        if(input_raw[i] == '\n')
        {
            if(carret == 0)
            {
                for (size_t j = 0; j < 3; j++)
                {
                    if(value > max_value[j])
                    {
                        max_value[j] = value;
                        max_index[j] = index;
                        break;
                    }
                }
                
                
                index ++;
                value = 0;
            }
            else
            {
                line[carret] = '\0';
                int number = atoi(line);
                value += number;
            }

            carret=0;
        }
        else
        {
            line[carret] = input_raw[i];
            carret++;
        }
    }

    int sum = max_value[0]+max_value[1]+max_value[2];
    
    printf("%s%i%s%i%s\n","Elf number ",max_index[0]+1," with ", max_value[0], " calories.");
    
    printf("%s%i%s\n","The top three Elves are carrying ",sum," calories in total.");
    
    return 1;
}