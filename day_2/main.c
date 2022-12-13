
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

    int me = 0;
    int opponent = 0;

    int points = 0;

    for (size_t i = 0; i < length; i+=4)
    {
        /*
        transform into integers
        A=1; B=2; C=3
        X=1; Y=2; Z=3
        faster comparison and can be used to calculate point from type
        */
        opponent = input_raw[i] - 64;
        me = input_raw[i+2]-23- 64;

        //No branching
        //Assume loss
        points += me;

        //g* = 1 if scenario is true, otherwise 0
        int g1 = ((opponent == 1)*(me == 2));//win
        int g2 = ((opponent == 2)*(me == 3));//win
        int g3 = ((opponent == 3)*(me == 1));//win
        int g4 = (opponent == me);//draw

        //add points and remove points from assumed loss
        //addition and subtraction is multiplied by g*
        //points increment by 0 if scenario (g*) is 0
        points += ((6+me) * g1) - (me*g1);
        points += ((6+me) * g2) - (me*g2);
        points += ((6+me) * g3) - (me*g3);
        points += ((3+me) * g4) - (me*g4);
    }
    
    printf("%s%i\n","points: ", points);

    return 1;
}