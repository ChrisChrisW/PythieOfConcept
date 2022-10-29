#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define TIMEOUT 110 // Don't need 111th time
#define BEGIN_SIZE 1050

void printPass() {
    sleep(1);
    for(int time = 0; time < TIMEOUT; time++)
        printf("PASS\n");

    return;
}

// Check if we read to the end
void cleanBuffer() {
    int c = 0;
    while (c != '\n' && c != EOF)
        c = getchar();

    return;
}
    
int readStdin(char *data, int length)
{
    char *positionEntree = NULL;

    if (fgets(data, length, stdin) != NULL) {
        positionEntree = strchr(data, '\n');

        if (positionEntree != NULL)
            *positionEntree = '\0';
        else
            cleanBuffer();

        return 1;
    }

    cleanBuffer();
    return 0;
}


/* --- MAIN --- */
int main(void)
{
    printPass();

    // int NJ = 0; // nombre de pythies
    // int J = 0;  // numéro de pythie
    // char *C;
    // int W = NULL;
    // int ST = 0;

    // data d'entrée
    char* zone = malloc(BEGIN_SIZE * sizeof(char));
    for (int i = 0; i <= BEGIN_SIZE; i++)
    {
        readStdin(zone, BEGIN_SIZE);
        fprintf(stderr, "data %d:  %s\n", i, zone);
    }
    free(zone);

    // zone = malloc(BEGIN_SIZE * sizeof(char));
    // int i = 0;
    // while (1)
    // {
    //     readStdin(zone, BEGIN_SIZE);
    //     fprintf(stderr, "data %d:  %s\n", i, zone);
    //     i++;
    // }
    // free(zone);

    return 0 ;
}
