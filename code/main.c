#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define TIMEOUT 110 // Don't need 111th time
#define BEGIN_SIZE 1050 // Begin data

// just print "Pass"
void printPass() {
    for(int time = 0; time < TIMEOUT; time++)
        printf("PASS\n");

    return;
}

// Check if we read to the end
// Else she takes the length of value et fgets()
void cleanBuffer() {
    int c = 0;
    while (c != '\n' && c != EOF)
        c = getchar();

    return;
}

int readStdin(char *data)
{
    char *positionEntree = NULL;

    // cleanbuffer will take all element so length set 10
    if (fgets(data, 10, stdin) != NULL) {
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


    int NJ, J; // nombre et numéro de pythies
    char* C[50]; // Concept
    char* W[1000]; // Mot de la liste 
    int S[1000][50]; // Le score associé au mot W
    
    char* data = malloc(sizeof(char*));

    
    // while(readStdin(data)) {
    //     int NJ = data[0];
    //     printf("PASS\n");
    //     fprintf(stderr, "test =   %d\n", NJ);
    // }


    for(int time = 0; time < BEGIN_SIZE + (3 * (TIMEOUT - 1)); time++) {
        printf("PASS\n");
        // printf("GUESS routine\n");

        // data d'entrée
        readStdin(data);

            fprintf(stderr, "%d , data all =   %s\n", time, data); // TODO: debug

        // if(time > BEGIN_SIZE) {
        //     fprintf(stderr, "%d , data all =   %s\n", time, data); // TODO: debug
            
        //     fprintf(stderr, "data C bis =   %s\n", C[1]); // TODO: debug
        //     fprintf(stderr, "data C bis =   %s\n", C[3]); // TODO: debug

        //     // for (int i = 0; i < 50; i++) {
        //     //     fprintf(stderr, "data C bis =   %s\n", C[i]); // TODO: debug
        //     // }
        // }
                // if(time == 0) {
                //     char* tmp = malloc(sizeof(char*));
                //     strcpy(tmp, strtok(data, " "));

                //     for(int col = 0; col < 2; col++) {
                //         if (col == 0)
                //             NJ = atoi(tmp);
                //         if (col == 1)
                //             J = atoi(tmp);

                //         tmp = strtok(NULL, " ");
                //     }
                //     free(tmp);

                //     fprintf(stderr, "NJ = %d \n",NJ); // TODO: debug
                //     fprintf(stderr, "J = %d \n",J); // TODO: debug
                // }
                // else if (time <= 50) {
                //     int row = time - 1;
                //     C[row] = data; // add each string in C

                //     fprintf(stderr, "C in %d =>  %s\n", row, C[row]); // TODO: debug
                // }
                // else {
                //     int row = time - 51;

                //     fprintf(stderr, "\n%d ,  W in =>  \n%s\n", row, data); // TODO: debug

                //     char* tmp = malloc(50 * sizeof(char*));
                //     tmp = strtok(data, " ");

                //     for(int col=0; col<=50; col++) {
                //         if (col == 0) 
                //             W[row] = data; 
                //         else 
                //             S[row][col - 1] = atoi(tmp);
                        
                //         tmp = strtok(NULL, " "); // delete first argument
                //     }
                    
                //     free(tmp);
                //     // fprintf(stderr, "data string : %s \n", W[row]); // TODO: debug
                //     // fprintf(stderr, "S %d test sur le 2eme  => %d \n\n",row, S[row][2]); // TODO: debug

                // }

    }
    
    free(data);

    return 0;
}
