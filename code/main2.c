#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE 1053

char * readStdin();
// void extract_score (char *s );
int main(void)
{
    char * data;
    data = readStdin();

    int NJ = atoi(&data[0]);
    int J = atoi(&data[2]);

    char * C [50];
    char * W [1000];


    // Set C init value
    for (int i = 0 ; i < 50 ; i ++){
        C[i] = readStdin();
    }
        
    // Set W S1 ... Sn init values
    for (int i = 0 ; i < 1000 ; i ++){
        W[i] = readStdin();
    }
    


    // fprintf(stderr, readStdin());
    // fprintf(stderr, readStdin());
    

    fprintf(stderr, "%d", J);
    fprintf(stderr, "%d", NJ);
    fprintf(stderr, "%s", C[1]);
    fprintf(stderr, "%s", W[1]);

    // PASS or GUESS word
    for (int i = 0 ; i < 110 ;i ++) {
        printf("PASS\n");
        fprintf(stderr, "J'adore la FR");
    }

    //fprintf(stderr,"%d\n", k);

    free(data);
    return 0;
}


char * readStdin()
{
    char *s = malloc(sizeof(char *)*1000);
    fgets(s,1000,stdin);
    return s;
}

// void extract_score(char * s){
//     int d[50];
//     for (int i = 1 ; i < 50; i ++) {
        
//     }
// }