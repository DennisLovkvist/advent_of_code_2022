
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STRIDE 173
#define HEIGHT 41

typedef struct Node Node;
struct Node
{
    char height,visual;
    int x,y,index;
    int G,H;
    int list_index;
    Node *parent;
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
int distance(int x1, int y1, int x2, int y2)
{
    int dx = abs(x1-x2);
    int dy = abs(y1-y2);

    if(dx > dy)
    {
        return 14*dy + 10*(dx-dy);
    }
    else
    {
        return 14*dx + 10*(dy-dx);
    }
}
unsigned int within_bounds(int x, int y)
{
    if(x < 0 || x >= STRIDE)
    {
        return 0;
    }
    if(y < 0 || y >= HEIGHT)
    {
        return 0;
    }
    return 1;
}
int index_from_xy(int x, int y, int stride)
{
    return y * stride + x;
}
unsigned int valid_move(Node *node_from,Node *node_to)
{

    if(node_to->height == 'a')
    {
        return 0;
    }
    else if(node_to->height > node_from->height + 1)
    {
        return 0;
    }
    else
    {

        return 1;
    }
    /*
    if(node_from->height >= node_to->height)
    {
        return 1;
    }
    else if(node_from->height+1 == node_to->height)
    {
        return 1;
    }
    else
    {
        return 0;
    }*/
}
unsigned int list_contains(Node *node, Node **list, int list_size)
{
    if(node->list_index == -1)return 0;
    if(node->list_index >= list_size)return 0;
    return (list[node->list_index]->index == node->index);
}
void list_add(Node *node, Node **list, int *list_length)
{
    node->list_index = *list_length;
    list[*list_length] = node;
    *list_length = *list_length + 1;
}
void list_remove(Node *node, Node **list, int *list_length)
{
    int temp = node->list_index;
    list[temp] = list[*list_length-1];
    list[temp]->list_index = temp;
    list[*list_length-1] = NULL;
    *list_length = *list_length -1;
    node->list_index = -1;
}

void find_path(Node *map, int x1, int y1, int x2, int y2, Node **list_opened,Node **list_closed)
{
    int index_opened = 0,index_closed = 0;
    int index = index_from_xy(x1,y1,STRIDE);
    Node *node_start = &map[index];

    index = index_from_xy(x2,y2,STRIDE);
    Node *node_end = &map[index];


    list_add(node_start,list_opened,&index_opened);

    while(index_opened > 0)
    {
        Node *node = list_opened[0];
        for (int i = 1; i < index_opened; i++)
        {
            if((list_opened[i]->G + list_opened[i]->H) < (node->G + node->H))
            {
                node = list_opened[i];
            }
            else if((list_opened[i]->G + list_opened[i]->H) == (node->G + node->H))
            {
                if(list_opened[i]->H < node->H)
                {
                    node = list_opened[i];
                }
            }
        }

        

        list_remove(node,list_opened,&index_opened);
        list_add(node,list_closed,&index_closed);

        if(node->visual == 'E')
        {
            int steps = 1;
            Node *temp_node = node->parent;
            while(temp_node->visual != node_start->visual)
            {
                temp_node->visual = '#';
                temp_node = temp_node->parent;
                steps ++;
            }
            printf("steps %i\n",steps);
            return;
        }
        else
        {
            int kernel[4*2] = 
            {
                 0,-1,
                 1, 0,
                 0, 1,
                -1, 0
            };

            for (size_t i = 0; i <= 6; i+=2)
            {
                int nbx = node->x + kernel[i];
                int nby = node->y + kernel[i+1];

                if(within_bounds(nbx,nby))
                {
                    int nb_index = index_from_xy(nbx,nby,STRIDE);
                    Node *nb = &map[nb_index];

                    unsigned int vm = valid_move(node,nb);
                    if(list_contains(nb,list_closed, index_closed)|| !vm)// 
                    {
                        goto lol;
                    }
                    

                    int G = node->G + distance(node->x,node->y,nb->x,nb->y);
                    if(G < nb->G || !list_contains(nb,list_opened,index_opened))
                    {
                        nb->G = G;
                        nb->H = distance(nb->x,nb->y,node_end->x,node_end->y);
                        nb->parent = node;

                        if(!list_contains(nb,list_opened,index_opened))
                        {
                            list_add(nb,list_opened,&index_opened);
                        }
                    }
                    lol:
                }
                
                
            }
            
        }
    }

        Node *node = &map[0];

        for (size_t i = 0; i < STRIDE*HEIGHT; i++)
        {
            printf("%i\n",map[i].G);
            if(map[i].G < node->G)
            {
                node = &map[i];
            }
        }
        


        node->visual = ' ';

    end:


}
void debug_print(Node *map)
{
    for (size_t y = 0; y < HEIGHT; y++)
    {
        for (size_t x = 0; x < STRIDE; x++)
        {
            int index = index_from_xy(x,y,STRIDE);
           printf("%c", map[index].visual);
        }
        printf("\n");
    }
    
}
int main()
{
    srand(time(NULL));

    char* input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    int nodes_length = length;
    Node *map = malloc(sizeof(Node)*nodes_length);

    int index = 0, next = 0;
    int xs,ys,xe,ye;
    while(index < length && next < nodes_length)
    {
        char c = input_raw[index];

        if(c != '\n')
        {
            Node *node = &map[next];
            node->visual = c;
            node->height = c;
           
            node->index = next;
            node->x = node->index % STRIDE;
            node->y = node->index / STRIDE;
            node->G = 0;
            node->H = 0;
            node->list_index = -1;
            if(c == 'S')
            {
                xs = node->x;
                ys = node->y;
                node->height = 'a';
            }
            else if(c == 'E')
            {
                xe = node->x;
                ye = node->y;
                node->height = 'z';
            }

            next++;
        }
        index ++;
    }

    Node **nodes_open = malloc(sizeof(Node*)*1000);
    Node **nodes_closed = malloc(sizeof(Node*)*1000);

    find_path(&map[0],xs,ys,xe,ye, &nodes_open[0],&nodes_closed[0]);

    debug_print(&map[0]);


    


    return 1;
}

