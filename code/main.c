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

    int NJ, J; // nombre et numéro de pythies
    char* C[50]; // Concept
    char* W[1000]; // Mot de la liste 
    int S[1000][50]; // Le score associé au mot W

    // data d'entrée
    char* data = malloc(BEGIN_SIZE * sizeof(char));

    for (int line = 0; line <= BEGIN_SIZE; line++)
    {
        readStdin(data, BEGIN_SIZE);

        if(line == 0) {
            char* tmp = malloc(sizeof(char*));
            strcpy(tmp, strtok(data, " "));

            for(int taille = 0; taille < 2; taille++)
            {
                if (taille == 0)
                    NJ = atoi(tmp);
                if (taille == 1)
                    J = atoi(tmp);

                tmp = strtok(NULL, " ");
            }
            free(tmp);

            fprintf(stderr, "NJ = %d \n",NJ); // TODO: debug
            fprintf(stderr, "J = %d \n",J); // TODO: debug
        }
        else if (line <= 50) {
            int row = line - 1;
            C[row] = data; // add each string in C

            fprintf(stderr, "C in %d =>  %s\n", row, C[row]); // TODO: debug
        }
        else {
            int row = line - 51;
            char* tmp = malloc(50 * sizeof(char*));
            tmp = strtok(data, " ");

            for(int col=0; col<=50; col++)
            {
                if (col == 0) 
                    W[row] = data; 
                else 
                    S[row][col - 1] = atoi(tmp);
                
                tmp = strtok(NULL, " "); // delete first argument
            }
            
            free(tmp);
            fprintf(stderr, "data string : %s \n",W[row]); // TODO: debug
            fprintf(stderr, "S %d test sur le 2eme  => %d \n\n",row, S[row][2]); // TODO: debug

            // fprintf(stderr, "data all =   %s\n", data); // TODO: debug
        } 
    }

    return 0;
}
