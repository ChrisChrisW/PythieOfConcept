/*

--> PHASE 1: =======
    L'algorithme qu'on utilise pour estimer p dans la phase 1 est le suivant:

    On attend la réception de tous les 20 concepts. On sait que dans ce 20 concepts il y a les 10-p les moins forts et les 10+p les plus forts pour le mot secret.

    Pour chaque mot dans la liste de 1000 mots, on prend la liste des poids et nous la trions du plus petit au plus grand poids LC_STRING = [20, 30, 45, ..., 70, 81] 50 elements 

    On la transforme en indices de concepts (en utilisant l'indice de chaque poids dans la liste initiale des poids) LC_INDEX = [13, 5, 20, ..., 2, 0, 45]  50 elements

    On prend la liste des concepts donnés par la déesse ex: CD = [13, 15, 80, ..., 45, 0, 30] 20 elements

    On projette la comparaison des deux listes CD et LC_INDEX dans une nouvelle liste initialisée à 0 => en mettant des 1 pour les elements de CD qui existent dans LC_INDEX

    ça donne une liste du type LISTE_FINALE = [1, 0, 0, ..., 0, 1, 1]

    Si cette liste calculée est de la forme [11111100000000000000000111111111111] (il y a exactement deux changements de 1 à 0 et de 0 à 1 en parcourant la liste)
            --> cela veut dire que c'est le probablement le bon mot et on calcul le paramètre p à partir du nombre de 1 au début de la liste
            --> ATTENTION: avant de dire que c'est le bon mot, on vérifie la condition sur 3 <= p <= 7
    
    Dans mon algorithme, je ne cherche pas les LISTE_FINALE avec exactement deux changements de 1 à 0 et de 0 à 1, 
    Je cherche plutot les minimum de changements de 1 à 0 et de 0 à 1.
    Parce que parfois, on peut ne pas trouver un mot correspondant à exactement 2 changements

--> PHASE 2, 3, 4, 5: =======
    On utilise un procédé par élimination.
    A chaque fois qu'on reçoit un nouveau concept
        -> On prend tous les 1000 mots dans chaque round
            -> S'il y a une contradiction avec les paramètres p ou a,b,c qu'on a estimé avant, alors le mot est eliminé
        -> Quand il ne reste qu'un seul mot non eliminé, on le propose
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_CONCEPTS 50
#define NUMBER_OF_WORDS 1000
#define WORD_SIZE 26
#define CONCEPT_SIZE 16
#define NUMBER_OF_ROUNDS_PER_PHASE 20

#define STEPS 5
#define PRECISION 3
//  Plus ce parametre est petit plus l'estimation du triplet a,b,c est précise 
//  ATTENTION NE PAS METTRE UN NOMBRE TRES FAIBLE SI VOUS N'AVEZ PAS UN BON PROCESSEUR
//  SI VOUS METTEZ UNE VALEUR TRES PETITE, VOTRE PC NE SERA PAS EN CAPACITE D'ESTIMER LE TRIPLET DANS L'INTERVAL DE TEMPS QUE LA DEESSE PERMET

struct W_and_S{  // Structure pour storer un mot avec les poids correspondants pour tout les concepts
    char word[WORD_SIZE+1];
    int scores[NUMBER_OF_CONCEPTS];
};

void recuperer_information_debut_phase(int NJ){  // Récupérer les informations données par la déesse en début de phase
    int J, P, END;
    for (int i = 0; i<NJ; i++){
        scanf("%d %d %d ", &J, &P, &END);
    }
}

void recuperer_information_fin_round(int NJ){   // Récupérer les informations données par la déesse en fin de chaque round
    char prop1[WORD_SIZE+1];
    int prop2, prop3;
    
    for (int i = 0; i<NJ; i++)
        scanf("%s %d %d ", prop1, &prop2, &prop3);
}

int cmpfunc (const void * a, const void * b) {  // Fonction auxiliaire de comparaison utilisé pour la fonction de tri qsort
   return ( *(int*)a - *(int*)b );
}

int sum_array(int array[], int len){            // Donne la somme des élements dans le tableau array
    int sum = 0;
    for (int i = 0; i < len;i ++){
        sum += array[i];
    }
    return sum;
}

int check_if_letter_exists_in_string(char letter, char *my_string){   // Vérifier si une lettre existe dans une chaine de caractères
    char *cursor = my_string;
    while (*cursor != '\0'){
        if (letter == *cursor){
            return 1;
        }
        cursor ++;
    }
    return 0;
}

int number_of_letters_in_word(char *word1, char *word2){ // combien de lettres du mot word1 existe dans word2
    int count = 0;
    char *cursor1 = word1;
    while (*cursor1 != '\0'){
        if (check_if_letter_exists_in_string(*cursor1, word2)){
            count ++;
        }
        cursor1++;
    }
    return count;
}

int length_difference_two_strings(char *word1, char *word2){  // Calcule la différence ( en valeurs absolue ) des longeurs entre deux strings
    if (strlen(word1) >= strlen(word2)) return strlen(word1) - strlen(word2);
    return strlen(word2) - strlen(word1);
}

void copy_array(int source[], int dest[], int len){     // Copie le teableau source dans le tableau dest
  for (int i = 0; i < len; i++){
    dest[i] = source[i];
  }
}

int get_index_of_element_in_array(int x, int array[], int len, int last_searched_weight, int last_found_index){  // Chercher l'index d'un element dans un tableau en tenant en compte le dernier element cherché pour ne pas donner le même index pour un même élement cherché
    for (int i = 0; i < len; i++){
        if (array[i] == x){
            if (last_searched_weight == x){
                if (i > last_found_index){
                    return i;
                }
            }
            else return i;
        }
    }
    return -1;
}

int search_index_of_element_in_array(int x, int *array, int len){
    for (int i = 0; i < len; i++){
        if (array[i] == x)
            return i;
    }
    return -1;
}

int convert_to_concept_ids(int *extract, int *copy_of_weights, int original_weights, int len){
    int last_searched_weight = -300;
    int last_found_index = -300;
    for (int i = 0; i < len; i++){
        int searched_weight = copy_of_weights[i];
        extract[i] = get_index_of_element_in_array(searched_weight, original_weights, len, last_searched_weight, last_found_index);
        last_searched_weight = searched_weight;
        last_found_index = extract[i];
    }
    return -1;
}

void init_algo(struct W_and_S my_W_and_S, int extract[50]) {
        int copy_of_weights[50];
        copy_array(my_W_and_S.scores, copy_of_weights, 50);
        qsort(copy_of_weights, 50, sizeof(int), cmpfunc);
        convert_to_concept_ids(extract, copy_of_weights, my_W_and_S.scores, 50);
}

int formula(int concept_index, char concepts[NUMBER_OF_CONCEPTS][CONCEPT_SIZE+1],int word_index, struct W_and_S all_words_with_weights[1000], int a, int b, int c){ // Cette fonction calcule la valeur de la fonction utilisée par la déesse
    int s = all_words_with_weights[word_index].scores[concept_index];
    int t = number_of_letters_in_word(all_words_with_weights[word_index].word, concepts[concept_index]);
    int u = length_difference_two_strings(all_words_with_weights[word_index].word, concepts[concept_index]);
    return a*s + 10*b*t + 10*c*u;
}

int is_formula_is_gretter_than_next(int concept_index, int concept_next_index, char concepts[NUMBER_OF_CONCEPTS][CONCEPT_SIZE+1],int word_index, struct W_and_S all_words_with_weights[1000], int a, int b, int c){
    int f_current = formula(concept_index, concepts, word_index, all_words_with_weights, a, b, c);
    int f_next = formula(concept_next_index, concepts, word_index, all_words_with_weights, a, b, c);

    if(f_current < f_next)
        return 0;

    return 1;
}

int get_concept_index(char concepts[NUMBER_OF_CONCEPTS][CONCEPT_SIZE+1], char *concept){  // Cette fonction retourne l'index d'un concept étant donné le string du concept
    for (int i = 0; i<50; i++){
        if (strcmp(concepts[i], concept) == 0) return i;
    }
    return -1;
}

void readStdin(char * concept_by_goddess, int * concept_by_goddess_index, char C[NUMBER_OF_CONCEPTS][CONCEPT_SIZE+1]) {
    fgets(concept_by_goddess, 16, stdin);
    concept_by_goddess[strcspn(concept_by_goddess, "\n")] = 0; // enlever les retours à la ligne
    *concept_by_goddess_index = get_concept_index(C, concept_by_goddess); // transformer le concept string en index de concept
}

// Check si la liste extract créée pour chaque mot dans la liste des 1000 mots donnés au début correspond aux concepts données par la déesse
int array_comp(int *extract, int *concepts_by_goddess_indexes, int len1, int len2, int *p1, int *p2){
    int array[50] = {0};
    for (int i = 0; i < len2; i++){
        for (int j = 0; j < len1; j++){
            if (extract[j] == concepts_by_goddess_indexes[i]){
                array[j] = 1;
                break;
            }
        }
    }

    int count = 0;
    if (array[0] == 0) return 0;
    for (int i = 0; i < len1-1; i++){
        if (array[i] != array[i+1]){
            count ++;
        }
    }

    int count_p1 = 1;
    for (int i = 0; i < len1-1; i++){
        if (array[i] == array[i+1]){
            count_p1 ++;
        }
        else break;
    }

    int count_p2 = 1;
    for (int i = len1-1; i > 0; i--){
        if (array[i] == array[i-1]){
            count_p2 ++;
        }
        else break;
    }

    if (count == 2) {
        *p1 = count_p1;
        *p2 = count_p2;
    }
    return count;
}

int main(void){
    fflush(stdout);
    // Reception des informations de la partie avant première manche
    int NJ, J;
    scanf("%d %d ", &NJ, &J);
    char concepts[NUMBER_OF_CONCEPTS][CONCEPT_SIZE+1];
    for (int i = 0; i<NUMBER_OF_CONCEPTS; i++){
        scanf("%s ", concepts[i]);
    }
    // Reception de la liste des mots avec les poids pour chaque concept
    struct W_and_S all_words_with_weights[NUMBER_OF_WORDS];
    //size_t len;
    for (int i = 0; i < NUMBER_OF_WORDS; i++){
        scanf("%s ", all_words_with_weights[i].word);
        for (int j = 0; j < NUMBER_OF_CONCEPTS; j++){
            scanf("%d ", &all_words_with_weights[i].scores[j]);
        }
    }
    
                // ======================   Phases du jeu   ====================== //
//================>>>>>> PHASE 1

    char concepts_by_goddess[20][16];
    int concepts_by_goddess_indexes[20];
    // Récupérer les informations de la phase en cours pour tous les joueurs
    recuperer_information_debut_phase(NJ);
    
    // Comportement similaire à celui de iaDelphe pour la première phase afin de trouver le paramètre p
    for (int i = 0; i < 20; i++){
        readStdin(concepts_by_goddess[i], &concepts_by_goddess_indexes[i], concepts);
        printf("PASS\n");
        fflush(stdout);
        // Récuperation des informations des essaies des autres joueurs pour chaque round de la phase
        recuperer_information_fin_round(NJ);
    }

    char *the_word = (char*)malloc(sizeof(char)*26); // initialization du mot qu'on va proposer à la déesse 
    int p1; // initialization du paramètre 10 - p qu'on va essayer d'estimer par la suite
    int p2; // initialization du paramètre 10 + p qu'on va essayer d'estimer par la suite
    int index_of_the_most_close; // indexe du mot le plus probable 
    int most_low_count = 49; // minimum de count, avec count le nombre de changement de 1 à 0 out de 0 à 1 dans la liste comparative ( regarder explication de l'algo tout en haut du code )
    
    // Algorithme pour trouver le premier mot que choisi la déesse et aussi le paramètre p qu'elle choisi
    for (int i = 0; i < NUMBER_OF_WORDS; i++){
        struct W_and_S my_W_and_S = all_words_with_weights[i];
        int extract[50];
        init_algo(my_W_and_S, extract);
        
        int temp_p1 = 0;
        int temp_p2 = 0;
        int similarity = array_comp(extract, concepts_by_goddess_indexes, 50, 20, &temp_p1, &temp_p2);
        if (similarity <= most_low_count && (temp_p1 < temp_p2) && (abs(temp_p1+temp_p2 -20) < abs(p1 + p2 - 20)) ){
            p1 = temp_p1;
            p2 = temp_p2;
            most_low_count = similarity;
            index_of_the_most_close = i;
        }
    }
    int p = 10 - p1;  // l'estimation du paramètre p choisi par la déesse 

    // ---> recuperation des informations des autres pythies pour la dernière round de la phase 1
    recuperer_information_fin_round(NJ);

    // Algorithme pour faire une estimation de a, b, c
    int total_of_combinnations = 0;
    int is_bad_combinations = 0;
    int ma=0, mb=0, mc=0;
    
    for (int a = 1; a <= 100; a+=PRECISION){
        for (int b = 1; b <= 100; b+=PRECISION){
            for (int c = 1; c <= 100; c+=PRECISION){
                is_bad_combinations = 0;
                for (int k = 0; k < NUMBER_OF_ROUNDS_PER_PHASE-1; k++){
                    if (is_formula_is_gretter_than_next(concepts_by_goddess_indexes[k], concepts_by_goddess_indexes[k+1], concepts, index_of_the_most_close, all_words_with_weights, a, b, c)){
                        is_bad_combinations = 1;
                        break; 
                    }
                }
                if (is_bad_combinations == 0){
                    ma += a;
                    mb += b; 
                    mc += c;
                    total_of_combinnations++;                
                }
            }
        }
    }

    int a_estim = total_of_combinnations != 0 ? ma/total_of_combinnations : 0;
    int b_estim = total_of_combinnations != 0 ? mb/total_of_combinnations : 0;
    int c_estim = total_of_combinnations != 0 ? mc/total_of_combinnations : 0;

//================>>>>>> PHASE 2, 3, 4, 5

    for (int step = 0; step < STEPS; step++){
        int words_left[1000] = {0};
        int found_word = 0;
        char concept_by_goddess[16];
        char new_concept_by_goddess[16];
        int concept_by_goddess_index = -1;
        int new_concept_by_goddess_index = -1;
        // Récupérer les informations de la phase en cours pour tous les joueurs
        recuperer_information_debut_phase(NJ);
        
        // Toutes les rounds d'une phase
        for (int i = 0; i < NUMBER_OF_ROUNDS_PER_PHASE; i++){
            // Récupération du concept donnée par la déesse 
            readStdin(new_concept_by_goddess, &new_concept_by_goddess_index, concepts);

            // Algorithme pour éliminer les mots qu'on sait qu'ils sont mauvais au fur et à mésure qu'on reçoit les concepts de la déesse
            for (int i = 0; i < NUMBER_OF_WORDS; i++){
                struct W_and_S my_W_and_S = all_words_with_weights[i];
                int extract[50];
                init_algo(my_W_and_S, extract);
                int index_of_received_concept = search_index_of_element_in_array(new_concept_by_goddess_index, extract, 50);
                
                // elemination du mot s'il est mauvais en utilisant le paramètre p estimé auparavant
                // on elimine le mot, si on trouve que le concept reçu n'apartient ni aux 10-p premiers concepts ni aux 10+p derniers concepts
                if (index_of_received_concept >= (10-p) && (index_of_received_concept <= 49-10-p)){
                    words_left[i] = 1;
                }
                
                // elemination du mot s'il est mauvais en utilisant les paramètres a, b, c estimés auparavant
                if (concept_by_goddess_index != -1 ){
                    // Si t(c1) > t(c2) on elimine le mot, avec t la fonction utilisé par la déesse, c1 le concept courant et c2 le concept précedent
                    if (!is_formula_is_gretter_than_next(new_concept_by_goddess_index, concept_by_goddess_index, concepts, i, all_words_with_weights, a_estim, b_estim, c_estim))
                        words_left[i] = 1;
                }

            }

            // S'il ne reste qu'on mot non eliminé, c'est qu'il est probablement le bon mot
            // Alors, on propose ce mot
            if (sum_array(words_left, 1000) == 999 && found_word == 0){
                found_word = 1;
                int word_index = search_index_of_element_in_array(0, words_left, 1000);
                the_word = all_words_with_weights[word_index].word;
                printf("GUESS %s\n", the_word);
            }
            else {
                printf("PASS\n");
            }
            fflush(stdout);
            recuperer_information_fin_round(NJ);
            
            // Sauvegarder le concept courant pour servir comme concept precedent dans la round suivante
            strcpy(concept_by_goddess, new_concept_by_goddess);
            concept_by_goddess_index = new_concept_by_goddess_index;
        }
    }
}