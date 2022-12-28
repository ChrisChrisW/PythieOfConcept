/*

--> PHASE 1: =======
    L'algorithme qu'on utilise pour estimer p dans la phase 1 est le suivant:

    On attend la réception de tous les 20 C. On sait que dans ce 20 C il y a les 10-p les moins forts et les 10+p les plus forts pour le mot secret.

    Pour chaque mot dans la liste de 1000 mots, on prend la liste des poids et nous la trions du plus petit au plus grand poids LC_STRING = [20, 30, 45, ..., 70, 81] 50 elements 

    On la transforme en indices de C (en utilisant l'indice de chaque poids dans la liste initiale des poids) LC_INDEX = [13, 5, 20, ..., 2, 0, 45]  50 elements

    On prend la liste des C donnés par la déesse ex: CD = [13, 15, 80, ..., 45, 0, 30] 20 elements

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

#define CONCEPTS_NB 50
#define CONCEPT_SIZE 16 // TODO

#define WORDS_NB 1000
#define SCORES_NB 50
#define MAX_WORD_SIZE 26

#define ROUNDS_NB 20

#define STEPS 5
#define PRECISION 3 // TODO

// W S0..Sn variables
struct W_and_S{
    char word[MAX_WORD_SIZE+1];
    int scores[CONCEPTS_NB];
};

void set_player_info(int players_number){
    int J, P, END;
    for (int player = 0; player < players_number; player++)
        scanf("%d %d %d ", &J, &P, &END);
}

void set_player_info_in_each_round(int players_number){
    char prop1[MAX_WORD_SIZE+1]; // TODO
    int prop2, prop3; // TODO
    for (int player = 0; player < players_number; player++)
        scanf("%s %d %d ", prop1, &prop2, &prop3);
}

int get_score(int target, int * scores, int last_target_score, int last_target_index){
    for (int score = 0; score < CONCEPTS_NB; score++){
        if (scores[score] == target && (last_target_score != target || score > last_target_index)) return score;
    }
    return -1;
}

int intComparator(const void * first, const void * second){
    int firstInt = * (const int *) first;
    int secondInt = * (const int *) second;
    return firstInt - secondInt;
}

void init_algo(struct W_and_S my_W_and_S, int extract[CONCEPTS_NB]) {
    // Make a copy
    int scores[CONCEPTS_NB];
    for (int i = 0; i < CONCEPTS_NB; i++)
        scores[i] = my_W_and_S.scores[i];

    // Sort scores variable
    qsort(scores, CONCEPTS_NB, sizeof(int), intComparator);
        
    // Sort extract variable
    int last_target_score, last_target_index;
    for (int i = 0; i < CONCEPTS_NB; i++){
        int target = scores[i];
        extract[i] = get_score(target, my_W_and_S.scores, last_target_score, last_target_index);

        last_target_score = target;
        last_target_index = extract[i];
    }
}

int formula(int concept_index, char C[CONCEPTS_NB][CONCEPT_SIZE+1], int word_index, struct W_and_S all_W_and_S[WORDS_NB], int a, int b, int c){ // Cette fonction calcule la valeur de la fonction utilisée par la déesse
    char *word = all_W_and_S[word_index].word;
    char *concept = C[concept_index];

    int s = all_W_and_S[word_index].scores[concept_index];
    
    int t = 0;
    while (*word != '\0'){
        if (*word != *concept) t++;
        word++; // next letter
    }

    int u = abs((int)(strlen(word) - strlen(concept)));

    return a*s + 10*b*t + 10*c*u;
}

int is_formula_is_gretter_than_next(int concept_index, int concept_next_index, char C[CONCEPTS_NB][CONCEPT_SIZE+1],int word_index, struct W_and_S all_W_and_S[WORDS_NB], int a, int b, int c){
    int f_current = formula(concept_index, C, word_index, all_W_and_S, a, b, c);
    int f_next = formula(concept_next_index, C, word_index, all_W_and_S, a, b, c);

    if(f_current < f_next) return 0;
    return 1;
}

void readStdin(char * concept, int * concept_index, char C[CONCEPTS_NB][CONCEPT_SIZE+1]) {
    fgets(concept, 16, stdin);
    concept[strcspn(concept, "\n")] = 0;
    // get index of C
    *concept_index = -1;
    for (int i = 0; i < CONCEPTS_NB; i++){
        if (strcmp(C[i], concept) == 0) *concept_index = i;
    }
}

int get_target_array_index(int target, int * array, int size){
    for (int index = 0; index < size; index++){
        if (array[index] == target) return index;
    }
    return -1;
}

// Check si la liste extract créée pour chaque mot dans la liste des 1000 mots donnés au début correspond aux C données par la déesse
int array_comp(int *extract, int *C_in_round_indexes, int * p1, int * p2){
    int array[CONCEPTS_NB] = {0};
    
    for (int i = 0; i < ROUNDS_NB; i++){
        for (int j = 0; j < CONCEPTS_NB; j++){
            if (extract[j] == C_in_round_indexes[i]){
                array[j] = 1;
                break;
            }
        }
    }
    if (array[0] == 0) return 0;

    int count = 0;
    for (int i = 0; i < CONCEPTS_NB-1; i++){
        if (array[i] != array[i+1]) count ++;
    }

    if (count == 2) {
        *p1 = 1;
        *p2 = 1;

        for (int i = 0; i < CONCEPTS_NB-1; i++){
            if (array[i] != array[i+1]) break;
            *p1 = *p1 + 1;
        }

        for (int i = CONCEPTS_NB-1; i > 0; i--){
            if (array[i] != array[i-1]) break;
            *p2 = *p2 + 1;
        }
    }

    return count;
}



int main(void){
    fflush(stdout);

    // Init pythies
    int NJ, J;
    scanf("%d %d ", &NJ, &J);

    // Init concepts
    char C[CONCEPTS_NB][CONCEPT_SIZE+1];
    for (int concept_index = 0; concept_index < CONCEPTS_NB; concept_index++)
        scanf("%s ", C[concept_index]);
    
    // Init W S0..Sn
    struct W_and_S all_W_and_S[WORDS_NB];
    for (int word_index = 0; word_index < WORDS_NB; word_index++){
        scanf("%s ", all_W_and_S[word_index].word);
        for (int score_index = 0; score_index < SCORES_NB; score_index++)
            scanf("%d ", &all_W_and_S[word_index].scores[score_index]);
    }
    
                // ======================   Phases du jeu   ====================== //
//================>>>>>> PHASE 1

    char C_in_round[20][CONCEPT_SIZE];
    int C_in_round_indexes[20];
    set_player_info(NJ);
    
    // Comportement similaire à celui de iaDelphe pour la première phase afin de trouver le paramètre p
    for (int i = 0; i < 20; i++){
        readStdin(C_in_round[i], &C_in_round_indexes[i], C);
        printf("PASS\n"); fflush(stdout);
        set_player_info_in_each_round(NJ);
    }

    char *guess_word = (char*)malloc(sizeof(char)*26);
    int p1, p2; // 10 - p, 10 + p
    int index_of_the_most_close; // indexe du mot le plus probable 
    int most_low_count = 49; // minimum de count, avec count le nombre de changement de 1 à 0 out de 0 à 1 dans la liste comparative ( regarder explication de l'algo tout en haut du code )
    
    // Find word and p
    for (int i = 0; i < WORDS_NB; i++){
        struct W_and_S my_W_and_S = all_W_and_S[i];
        int extract[CONCEPTS_NB];
        init_algo(my_W_and_S, extract);
        
        int tmp_p1, tmp_p2;
        int similarity = array_comp(extract, C_in_round_indexes, &tmp_p1, &tmp_p2);
        if (similarity <= most_low_count && (tmp_p1 < tmp_p2) && (abs(tmp_p1+tmp_p2 -20) < abs(p1 + p2 - 20)) ){
            p1 = tmp_p1;
            p2 = tmp_p2;
            most_low_count = similarity;
            index_of_the_most_close = i;
        }
    }
    int p = 10 - p1;  // l'estimation du paramètre p choisi par la déesse 

    // ---> recuperation des informations des autres pythies pour la dernière round de la phase 1
    set_player_info_in_each_round(NJ);

    // Algorithme pour faire une estimation de a, b, c
    int total_of_combinnations = 0;
    int is_bad_combinations = 0;
    int ma=0, mb=0, mc=0;
    
    for (int a = 1; a <= 100; a+=PRECISION){
        for (int b = 1; b <= 100; b+=PRECISION){
            for (int c = 1; c <= 100; c+=PRECISION){
                is_bad_combinations = 0;
                for (int k = 0; k < ROUNDS_NB-1; k++){
                    if (is_formula_is_gretter_than_next(C_in_round_indexes[k], C_in_round_indexes[k+1], C, index_of_the_most_close, all_W_and_S, a, b, c)){
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
        int words[WORDS_NB] = {0};
        int is_great_word = 0;
        char concept_by_goddess[16];
        char new_concept_by_goddess[16];
        int concept_by_goddess_index = -1;
        int new_concept_by_goddess_index = -1;
        // Récupérer les informations de la phase en cours pour tous les joueurs
        set_player_info(NJ);
        
        // Toutes les rounds d'une phase
        for (int round = 0; round < ROUNDS_NB; round++){
            // Récupération du concept donnée par la déesse 
            readStdin(new_concept_by_goddess, &new_concept_by_goddess_index, C);

            // Algorithme pour éliminer les mots qu'on sait qu'ils sont mauvais au fur et à mésure qu'on reçoit les C de la déesse
            for (int i = 0; i < WORDS_NB; i++){
                struct W_and_S my_W_and_S = all_W_and_S[i];
                int extract[CONCEPTS_NB];
                init_algo(my_W_and_S, extract);
                int index_of_received_concept = get_target_array_index(new_concept_by_goddess_index, extract, CONCEPTS_NB);
                
                // elemination du mot s'il est mauvais en utilisant le paramètre p estimé auparavant
                // on elimine le mot, si on trouve que le concept reçu n'apartient ni aux 10-p premiers C ni aux 10+p derniers C
                if (index_of_received_concept >= (10-p) && (index_of_received_concept <= 49-10-p)) words[i] = 1;
                
                // elemination du mot s'il est mauvais en utilisant les paramètres a, b, c estimés auparavant
                if (concept_by_goddess_index != -1 && !is_formula_is_gretter_than_next(new_concept_by_goddess_index, concept_by_goddess_index, C, i, all_W_and_S, a_estim, b_estim, c_estim)) words[i] = 1;
            }

            // S'il ne reste qu'on mot non eliminé, c'est qu'il est probablement le bon mot
            // Alors, on propose ce mot
            int sum_words = 0;
            for (int i = 0; i < WORDS_NB; i++) sum_words += words[i];
            
            if (sum_words == (WORDS_NB - 1) && is_great_word == 0){
                is_great_word = 1;
                int word_index = get_target_array_index(0, words, WORDS_NB);
                guess_word = all_W_and_S[word_index].word;
                printf("GUESS %s\n", guess_word);
            } else printf("PASS\n");
            fflush(stdout);
            set_player_info_in_each_round(NJ);
            
            // Sauvegarder le concept courant pour servir comme concept precedent dans la round suivante
            strcpy(concept_by_goddess, new_concept_by_goddess);
            concept_by_goddess_index = new_concept_by_goddess_index;
        }
    }

    free(guess_word);
}