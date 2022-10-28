#include <stdio.h>

int main(void)
{
    char C; // nombre de pythies
    int NJ; // nombre de pythies
    // int J;  // numéro de pythie

    // int W = NULL;
    // int ST = 0;

    int i = 0;
    while (1)
    {
        // print PASS
        printf("PASS\n");

        int nbLus = scanf("%c", &C);
        if(nbLus != 1) {
            // erreur : on n’a pas pu lire un entier
            fprintf(stderr, "Erreur : lecture entier impossible !") ;
            return 1 ; // sortie du programme
        }
        
        if(i == 0) fprintf(stderr, "PASS NJ = %c\n", C);
        else if(i == 2) fprintf(stderr, "PASS J = %c\n", C);
        else if(i > 3 && i < 10) fprintf(stderr, "PASS C = %c\n", C);
        else if(i > 10 && i < 16) fprintf(stderr, "PASS W = %c\n", C); # TODO: modifier pour faire un tableau de valeur
        else {
            fprintf(stderr, "PASS %d ### %c\n", i, C);
        }

        i++;
    }

    return 0 ;
}
