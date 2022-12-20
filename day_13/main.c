
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ELEMENT_TYPE_UNKNOWN 0
#define ELEMENT_TYPE_LIST 1
#define ELEMENT_TYPE_DIGIT 2

#define CMP_RESULT_LOWER 0
#define CMP_RESULT_EQUAL 1
#define CMP_RESULT_HIGHER 2


typedef struct Element Element;
struct Element
{
    Element *parent;
    Element **children;
    int child_count;
    unsigned int type;
    int* list;
    int digit;
    int depth;
    int index;
    int list_index;
};

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
Element *new_element(int depth)
{
    Element *e = malloc(sizeof(Element));
    e->depth = depth;
    e->type = ELEMENT_TYPE_UNKNOWN;
    e->digit = 0;
    e->list_index = 0;
    e->index = 0;
    e->child_count = 0;
    e->children = malloc(0);
    e->parent = NULL;
    return e;
}
int parse_digit(char *line, int s, int e)
{
    int length = e-s;
    char *digit = alloca(sizeof(char)*(length+1));
    memcpy(digit,&line[s],sizeof(char)*length);
    digit[length] = '\0';
    return atoi(digit);
}
void append_child(Element *parent,Element  *child)
{
    parent->child_count ++;
    parent->children = realloc(&parent->children[0],sizeof(Element*)*parent->child_count);
    parent->children[parent->child_count-1] = child;
}
Element *parse(char *line)
{
    int index = 0;
    char c = line[index];

    int opened = 1;
    int closed = 0;
    int depth = 1;

    Element *root = new_element(0);
    root->type = ELEMENT_TYPE_LIST;
    Element *current_element = root;
    root->parent = NULL;

    int digit_new = 1,digit_start = 0,digit_end = 0;

    while(c != '\0')
    {
        if(c == ']')
        {
            if(current_element->type == ELEMENT_TYPE_DIGIT)
            {
                digit_end = index;
                int digit = parse_digit(line,digit_start,digit_end);
                current_element->digit = digit;
                append_child(current_element->parent,current_element);
                current_element = current_element->parent;
                digit_new = 1;
                //parse digit
            }

            closed ++;
            depth--;
            current_element->type = ELEMENT_TYPE_LIST;
            if(current_element->parent != NULL)
            {
                append_child(current_element->parent,current_element);
                current_element = current_element->parent;
            }
            
        }
        else if(c == '[')
        {
            Element *e = new_element(depth);
            e->type = ELEMENT_TYPE_LIST;
            e->parent = current_element;
            current_element = e;
            opened ++;
            depth++;
        }
        else if(c == ',')
        {
            if(current_element->type == ELEMENT_TYPE_DIGIT)
            {
                digit_end = index;
                int digit = parse_digit(line,digit_start,digit_end);
                current_element->digit = digit;
                append_child(current_element->parent,current_element);
                current_element = current_element->parent;
                digit_new = 1;
                current_element->list_index++;
            }
            else if(current_element->type == ELEMENT_TYPE_LIST)
            {
                append_child(current_element->parent,current_element);
                current_element->list_index++;
            }

            
            
        }
        else
        {
            if(digit_new)
            {
                Element *e = new_element(depth);//create new element
                e->type = ELEMENT_TYPE_DIGIT;
                e->parent = current_element;//set parent to the current element
                e->index = current_element->list_index;//set the elements index as the current list index in the parent
                current_element->list_index++;//increment the parents list index
                current_element = e;//switch to elment as the current element
                digit_start = index;
                digit_new = 0;
            }
        }
        index++;
        c = line[index];
    }
    return root;
}
void  debug_print_element_tree(Element *e )
{
    printf("%s","[");
    for (size_t i = 0; i < e->child_count; i++)
    {
        if(e->children[i]->type == ELEMENT_TYPE_DIGIT)
        {
            printf("%i",e->children[i]->digit);
            
        }
        else
        {
            debug_print_element_tree(e->children[i]);
        }
        if(i != e->child_count-1)
        {
            printf(",");
        }
    }
    printf("%s","]");
    
}   
unsigned int cmp_elements(Element *e1,Element *e2)
{
    if(e1->type == ELEMENT_TYPE_DIGIT && e2->type == ELEMENT_TYPE_DIGIT)
    {
        //printf("- Compare %i vs %i\n",e1->digit , e2->digit);
        if(e1->digit < e2->digit)
        {
            return CMP_RESULT_LOWER;
        }
        else if(e1->digit == e2->digit)
        {
            return CMP_RESULT_EQUAL;
        }
        else if(e1->digit > e2->digit)
        {
            return CMP_RESULT_HIGHER;
        }
    }
    else if(e1->type == ELEMENT_TYPE_LIST && e2->type == ELEMENT_TYPE_LIST)
    {
        
        for (size_t i = 0; i < e1->child_count && i < e2->child_count; i++)
        {
            unsigned int cmp_result = cmp_elements(e1->children[i],e2->children[i]);

            if(cmp_result == CMP_RESULT_LOWER)
            {
                return CMP_RESULT_LOWER;
            }
            else if(cmp_result == CMP_RESULT_HIGHER)
            {
                return CMP_RESULT_HIGHER;
            }
        }
        if(e1->child_count == e2->child_count)
        {
            return CMP_RESULT_EQUAL;
        }
        else if(e1->child_count < e2->child_count)
        {
            return CMP_RESULT_LOWER;
        }
        else
        {
            return CMP_RESULT_HIGHER;
        }
        
    }
    else if(e1->type == ELEMENT_TYPE_DIGIT && e2->type == ELEMENT_TYPE_LIST)
    {
        for (size_t i = 0; i < 1 && i < e2->child_count; i++)
        {
            unsigned int cmp_result = cmp_elements(e1,e2->children[i]);

            if(cmp_result == CMP_RESULT_LOWER)
            {
                return CMP_RESULT_LOWER;
            }
            else if(cmp_result == CMP_RESULT_HIGHER)
            {
                return CMP_RESULT_HIGHER;
            }
        }
        if(1 == e2->child_count)
        {
            return CMP_RESULT_EQUAL;
        }
        else if(1 < e2->child_count)
        {
            return CMP_RESULT_LOWER;
        }
        else
        {
            return CMP_RESULT_HIGHER;
        }
    }
    else if(e1->type == ELEMENT_TYPE_LIST && e2->type == ELEMENT_TYPE_DIGIT)
    {
        for (size_t i = 0; i < 1 && i < e1->child_count; i++)
        {
            unsigned int cmp_result = cmp_elements(e1->children[i],e2);

            if(cmp_result == CMP_RESULT_LOWER)
            {
                return CMP_RESULT_LOWER;
            }
            else if(cmp_result == CMP_RESULT_HIGHER)
            {
                return CMP_RESULT_HIGHER;
            }
        }
        if(1 == e1->child_count)
        {
            return CMP_RESULT_EQUAL;
        }
        else if(1 > e1->child_count)
        {
            return CMP_RESULT_LOWER;
        }
        else
        {
            return CMP_RESULT_HIGHER;
        }
    }
} 
void part1()
{
    char* input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    int index = 0;
    int start = 0;

    Element *packets[2];
    unsigned int packet = 0;
    unsigned int sum = 0;
    int pair = 0;
    while(index <= length)
    {
        char c = input_raw[index];
        if(c == '\n' || c == '\0')
        {
            int len = index -start;
            if(len > 0)
            {
                char *line = alloca(sizeof(char)*(len+1));
                memcpy(line,&input_raw[start],sizeof(char)*len);
                line[len] = '\0';

                packets[packet] = parse(line);
                packet ++;
            }
            else
            {                
                pair ++;
                unsigned int cmp_res = cmp_elements(packets[0]->children[0],packets[1]->children[0]);

                if(cmp_res != CMP_RESULT_HIGHER)
                {

                    sum +=  pair;
                }
                //free_packet(packets[0]);TODO
                //free_packet(packets[1]);TODO
                
                packet = 0;
            }

            start = index +1;
        }
        
        index++;
    }

    printf("packets in order: %i\n",sum);

}
void sort_packets(Element **packets, int packets_length)
{
    int i, j;
    Element *packet;
    for (i = 1; i < packets_length; i++) 
    {
        packet = packets[i];
        j = i - 1;

        while (j >= 0 && cmp_elements(packets[j],packet) != CMP_RESULT_LOWER) 
        {
            packets[j + 1] = packets[j];
            j = j - 1;
        }
        packets[j + 1] = packet;
    }
}
void part2()
{
    char* input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    int index = 0;
    int start = 0;

    int count = 0;
    while(index <= length)
    {
        char c = input_raw[index];
        if(c == '\n' || c == '\0')
        {
            if(input_raw[index-1] == ']')
            {
                count ++;
            }
        }
        index++;
    }

    Element **packets = malloc(sizeof(Element*)*count);
    unsigned int packet = 0;

    index = 0;
    while(index <= length)
    {
        char c = input_raw[index];
        if(c == '\n' || c == '\0')
        {
            int len = index -start;
            if(len > 0)
            {
                char *line = alloca(sizeof(char)*(len+1));
                memcpy(line,&input_raw[start],sizeof(char)*len);
                line[len] = '\0';

                Element *root = parse(line);
                packets[packet] = root->children[0];
                packet ++;
            }

            start = index +1;
        }
        
        index++;
    }

    sort_packets(packets,count);

    int divider_2 = 0;
    int divider_6 = 0;
    int packet_index = 0;
    for (size_t i = 0; i < count; i++)
    {
        packet_index ++;
        if(packets[i]->child_count == 1)
        {
            if(packets[i]->children[0]->child_count == 1)
            {
                if(packets[i]->children[0]->children[0]->digit == 2)
                {
                    divider_2 = packet_index;
                }
                else if(packets[i]->children[0]->children[0]->digit == 6)
                {
                    divider_6 = packet_index;
                }
            }
        }
    }

    printf("%i\n",divider_2*divider_6);
    

    

}
int main()
{
    //part1();
    part2();
    return 1;
}

