
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define MAX_SIZE 100000
#define MAX_FILES 32
#define MAX_SUB_DIRECTORIES 32
#define NAME_BUFFER_SIZE 32

#define TOTAL_DISK_SPACE 70000000
#define REQUIRED_FREE_SPACE 30000000

typedef struct Directory Directory;
typedef struct File File;
struct Directory
{
    char name[NAME_BUFFER_SIZE];
    Directory *parent;
    int count_sub_directories,count_files;
    Directory *sub_directories[MAX_SUB_DIRECTORIES];
    File *files[MAX_FILES];
    int size;
};
struct File
{
    char name[NAME_BUFFER_SIZE];
    int size;
    Directory *parent;
};

void file_init(File *file, char *name, char*size_on_disk)
{
    int name_length = strlen(name);

    int n = (name_length >= NAME_BUFFER_SIZE-1) ? NAME_BUFFER_SIZE-1:name_length;
    size_t size = sizeof(char)*n;
    memcpy(&file->name,name, size);
    for (size_t i = n; i < NAME_BUFFER_SIZE; i++)
    {
        file->name[i] = '\0';
    }
    file->size = atoi(size_on_disk);
}
void directory_init(Directory *directory, char *name)
{
    int name_length = strlen(name);

    int n = (name_length >= NAME_BUFFER_SIZE-1) ? NAME_BUFFER_SIZE-1:name_length;
    size_t size = sizeof(char)*n;
    memcpy(&directory->name,name, size);
    for (size_t i = n; i < NAME_BUFFER_SIZE; i++)
    {
        directory->name[i] = '\0';
    }
    directory->count_sub_directories = 0;
    directory->count_files = 0;
    directory->size = 0;
}
Directory *directory_add_child(Directory *directory, char *child_name)
{   
    Directory *child = malloc(sizeof(Directory));
    directory->sub_directories[directory->count_sub_directories] = child;

    directory_init(child,child_name);
    directory->count_sub_directories ++;

    child->parent = directory;
    return child;
}

void directory_print(Directory *directory,int depth)
{
    for (size_t i = 0; i < depth; i++)
    {
        printf("  ");
    }
    printf(" -%s (dir, size=%i)\n",directory->name,directory->size);

    for (size_t i = 0; i < directory->count_sub_directories; i++)
    {
        directory_print(directory->sub_directories[i],depth+1);
    }
    for (size_t i = 0; i < directory->count_files; i++)
    {
        for (size_t i = 0; i < depth+1; i++)
        {
            printf("  ");
        }
        printf(" -%s (file, size=%i)\n",directory->files[i]->name,directory->files[i]->size);
    }
}
void parse_command(char *line,int length,unsigned int *command, char*argument)
{
    int index = 0;
    while(line[index] != '\0')
    {
        if(line[index] == 'c')
        {
            *command = 1;
            index+=2;
            break;
        }
        else if(line[index] == 'l')
        {
            *command = 2;
            index+=2;
            break;
        }
        index++;
    }
    if(*command == 1)//cd
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
File *parse_file(char *line,int length)
{
    int index = 0;
      
    int start_size = index;
    int end_size = 0;
    int start_name = 0;
    int end_name = 0;

    while(line[index] != '\0')
    {
        if(line[index] == ' ')
        {
            end_size = index;
            start_name = index +1;
        }
        index++;
    }

    int len = end_size-start_size;
    char *size = alloca(sizeof(char)*(len+1));
    memcpy(size,&line[start_size],sizeof(char)*len);
    size[len] = '\0';
    len = index-start_name;
    char *name = alloca(sizeof(char)*(len+1));
    memcpy(name,&line[start_name],sizeof(char)*len);
    name[len] = '\0';

    File *file = malloc(sizeof(File));
    file_init(file,name,size);

    return file;
    
    
}
void calc_size(Directory *directory, int *sum)
{
    directory->size = 0;
    for (size_t i = 0; i < directory->count_files; i++)
    {
        directory->size += directory->files[i]->size;
    }
    for (size_t i = 0; i < directory->count_sub_directories; i++)
    {
        calc_size(directory->sub_directories[i],sum);
        directory->size += directory->sub_directories[i]->size;
    }
    
    if(directory->size <= MAX_SIZE)
    {
        *sum += directory->size;
    }
}
void find_optimal_directory_for_deletion(Directory *directory,int used_disk_space, int *nearest,Directory **directory_optimal)
{

    int free_disk_space = TOTAL_DISK_SPACE - used_disk_space + directory->size;
    
    if(free_disk_space > REQUIRED_FREE_SPACE)
    {
        int delta = free_disk_space - REQUIRED_FREE_SPACE;
        if(delta < *nearest)
        {
            *directory_optimal = directory;
            *nearest = delta;
        }
    }

    for (size_t i = 0; i < directory->count_sub_directories; i++)
    {
        find_optimal_directory_for_deletion(directory->sub_directories[i],used_disk_space, nearest,directory_optimal);
    }

    
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
int main()
{

    char* input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    int index = 0;
    int start = 0;

    Directory root;
    directory_init(&root,"/");
    root.parent = &root;


    Directory *current = &root;

    while(index <= length)
    {
        char c = input_raw[index];

        if(c == '\n' || c == '\0')
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
                parse_command(line,length,&cmd,argument);

                if(cmd == 1)
                {
                    if(strcmp(argument,"..") == 0)
                    {   
                        current = current->parent;
                    }
                    else if(strcmp(argument,"/") == 0)
                    {
                        current = &root;
                    }
                    else
                    {
                        for (size_t i = 0; i < current->count_sub_directories; i++)
                        {
                            if(strcmp(argument,current->sub_directories[i]->name) == 0)
                            {
                                current = current->sub_directories[i];
                            }
                        }
                        Directory *child = directory_add_child(current,argument);
                        child->parent = current;
                        current = child;
                    }
                }
            }
            else if(line[0] != 'd')
            {
                File *file = parse_file(line,length);

                current->files[current->count_files] = file;
                current->count_files++;
                file->parent = current;
            }
            start = index+1;
        }
        index++;
    }

    
    int sum = 0;
    calc_size(&root,&sum);
    //directory_print(&root,0);

    int nearest = TOTAL_DISK_SPACE;
    Directory *directory_optimal;
    find_optimal_directory_for_deletion(&root,root.size,&nearest,&directory_optimal);
    

    printf("combined size of all directories under %i is: %i\n",MAX_SIZE ,sum);

    printf("%s is the most optimal directory for removal with a size of: %i\n",directory_optimal->name,directory_optimal->size);

    return 1;
}

