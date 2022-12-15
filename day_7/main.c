
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct Directory Directory;
struct Directory
{
    char name[32];
    Directory *parent;
    Directory **children;
};

void directory_init(Directory *directory, char *name, Directory *parent)
{
    int n = (strlen(name) > 32) ? 32:strlen(name);
    size_t size = sizeof(n);
    memcpy(&directory->name,name, size);
}

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
void parse_cmd(char *line,int length,unsigned int *cmd, char*argument)
{
    int index = 0;
    while(line[index] != '\0')
    {
        if(line[index] == 'c')
        {
            *cmd = 1;
            index+=2;
            break;
        }
        else if(line[index] == 'l')
        {
            *cmd = 2;
            index+=2;
            break;
        }
        index++;
    }
    if(*cmd == 1)//cd
    {
        int carret = 0;
        
        while(line[index] != '\0')
        {
            if(line[index] != ' ')
            {
                argument[carret++] = line[index];
            }
            index++;
        }
    }
    
}
void execute_cd(char *argument)
{
    printf("cd %s\n",argument);
}
void execute_ls()
{
    printf("ls\n");
}
void parse_response(char *line,int length)
{
    
}
int main()
{

    char* input_raw = load_input("input(example).txt");
    int length = strlen(input_raw);

    int index = 0;
    int start = 0;
    while(index < length)
    {
        char c = input_raw[index];

        if(c == '\n')
        {
            int len = index-start;
            char *line = alloca(sizeof(char)*(len+1));
            memcpy(line,&input_raw[start],sizeof(char)*(len));
            line[len] = '\0';
            if(line[0] == '$')
            {
                char *argument = alloca(sizeof(char)*(len+1));
                for (size_t i = 0; i < len+1; i++)
                {
                    argument[i] = '\0';
                }
                unsigned int cmd = 0;
                parse_cmd(line,length,&cmd,argument);

                if(cmd == 1)
                {
                    execute_cd(argument);
                }
                else if(cmd == 2)
                {
                    execute_ls();
                }
            }
            else
            {
                parse_response(line,length);
            }
            start = index+1;
        }
        index++;
    }

    return 1;
}

