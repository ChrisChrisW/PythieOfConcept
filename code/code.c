#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONCEPTS_NB 50
#define MAX_CONCEPT_SIZE 16

#define WORDS_NB 1000
#define SCORES_NB 50
#define MAX_WORD_SIZE 26

#define ROUNDS_NB 20
#define STEPS 5

#define CALIBRATION 5

/**
 * W S0..Sn variables
*/
typedef struct {
    char word[MAX_WORD_SIZE];
    int scores[CONCEPTS_NB];
} W_and_S;

/**
 * Sorts an array of scores in ascending order,
 * then populates another array with the indices of the original scores array in the sorted order.
 * 
 * @param (W_and_S) my_W_and_S
 * @param (int*) tmp_concepts
 * @return void
*/
void populate_tmp_concepts_array(W_and_S my_W_and_S, int tmp_concepts[CONCEPTS_NB]) {
    // Make a copy of the scores array
    int scores[CONCEPTS_NB];
    for (int i = 0; i < CONCEPTS_NB; i++)
        scores[i] = my_W_and_S.scores[i];

    // Sort the scores array in ascending order
    for (int i = 0; i < CONCEPTS_NB - 1; i++) {
        for (int j = 0; j < CONCEPTS_NB - i - 1; j++){
            if (scores[j] > scores[j + 1]) {
                int temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }

    // Populate the tmp_concepts array using the sorted scores array
    int last_target_score = 0, last_target_index = 0;
    for (int i = 0; i < CONCEPTS_NB; i++) {
        int target_score = scores[i];
        // Find the index of the target score in the original scores array
        for (int j = 0; j < CONCEPTS_NB; j++) {
            if (my_W_and_S.scores[j] == target_score && (last_target_score != target_score || j > last_target_index)) {
                tmp_concepts[i] = j;
                break;
            }
        }

        last_target_score = target_score;
        last_target_index = tmp_concepts[i];
    }
}

int calculate_t(char *word, char *target_concept) {
    int t = 0;

    // Compare the word and target_concept strings
    int result = strcmp(word, target_concept);
    if (result != 0) {
        // Count the number of characters that are different
        while (word[t] == target_concept[t]) {
            t++;
        }
        t = strlen(word) - t;
    }

    return t;
}

/**
 * Formula
 * init s, t and u
 * 
 * @param int                   concept_index
 * @param (char**)              C
 * @param int                   word_index
 * @param (W_and_S *)    my_W_and_S
 * @param int                   a
 * @param int                   b
 * @param int                   c
 * @return int
*/
int formula(int target_concept_index, char C[CONCEPTS_NB][MAX_CONCEPT_SIZE], int word_index, W_and_S all_W_and_S[WORDS_NB], int a, int b, int c){
    char *word = all_W_and_S[word_index].word;
    char *target_concept = C[target_concept_index];

    int s = all_W_and_S[word_index].scores[target_concept_index];
    
    int t = calculate_t(word, target_concept);

    int u = strlen(word) - strlen(target_concept);
    if(u < 0) u = -u;

    return a*s + 10*b*t + 10*c*u;
}

/**
 * In the formula, we compare between the current concept and next one, return bool
 * 
 * @param int                   concept_index
 * @param int                   concept_next_index
 * @param (char**)              C
 * @param int                   word_index
 * @param (W_and_S *)    my_W_and_S
 * @param int                   a
 * @param int                   b
 * @param int                   c
 * @return int 
*/
int is_formula_is_gretter_than_next(int concept_index, int concept_next_index, char C[CONCEPTS_NB][MAX_CONCEPT_SIZE], int word_index, W_and_S all_W_and_S[WORDS_NB], int a, int b, int c){
    int f_current_value = formula(concept_index, C, word_index, all_W_and_S, a, b, c);
    int f_next_value = formula(concept_next_index, C, word_index, all_W_and_S, a, b, c);

    if(f_current_value < f_next_value) return 0;
    return 1;
}

/**
 * Set A, B and C variables with average method
 * 
 * @param (int*)                a
 * @param (int*)                b
 * @param (int*)                c
 * @param (int*)                C_in_round_indexes
 * @param int                   target_index
 * @param (W_and_S *)    my_W_and_S
 * @param (char**)              C
 * @return void
*/
void set_a_b_c_variables(int * a, int * b, int * c, int * C_in_round_indexes, int target_index, W_and_S * all_W_and_S, char C[CONCEPTS_NB][MAX_CONCEPT_SIZE]){
    int is_great_combinations = 0; // bool
    int total_of_combinnations = 0;
    int sum_a = 0, sum_b = 0, sum_c = 0;
    int max = 100;

    for (int a_value = 1; a_value <= max; a_value+=CALIBRATION){
        for (int b_value = 1; b_value <= max; b_value+=CALIBRATION){
            for (int c_value = 1; c_value <= max; c_value+=CALIBRATION){
                is_great_combinations = 0;

                for (int r = 0; r < ROUNDS_NB; r++){
                    if (is_formula_is_gretter_than_next(C_in_round_indexes[r], C_in_round_indexes[r + 1], C, target_index, all_W_and_S, a_value, b_value, c_value)){
                        is_great_combinations = 1; break; 
                    }
                }
                
                if (is_great_combinations == 0){
                    sum_a += a_value;
                    sum_b += b_value; 
                    sum_c += c_value;
                    total_of_combinnations++;                
                }
            }
        }
    }

    // set a, b and c with average method
    if(total_of_combinnations > 0){
        *a = sum_a / total_of_combinnations;
        *b = sum_b / total_of_combinnations;
        *c = sum_c / total_of_combinnations;
    }
}

/**
 * Set player/pythie info data at start
 * 
 * @param int players_number
 * @return void
*/
void set_player_info_at_start(int players_number){
    int J, P, END;
    for (int player = 0; player < players_number; player++)
        scanf("%d %d %d ", &J, &P, &END);
}

/**
 * Set player/pythie info data in each round
 * 
 * @param int players_number
 * @return void
*/
void set_player_info_in_each_round(int players_number){
    char W[MAX_WORD_SIZE];
    int J, ST;
    for (int player = 0; player < players_number; player++)
        scanf("%s %d %d ", W, &J, &ST);
}

/**
 * Read stdin value with fgets method
 * 
 * @param (char*)   concept
 * @param (int*)    concept_index
 * @param (char**)  C
 * @return void 
*/
void readStdin(char * concept, int * concept_index, char C[CONCEPTS_NB][MAX_CONCEPT_SIZE]){
    fgets(concept, CONCEPTS_NB, stdin);
    char *newline = strtok(concept, "\n"); 
    if (newline != NULL) strcpy(concept, newline); // delete all \n

    // get index of C
    *concept_index = -1;
    for (int i = 0; i < CONCEPTS_NB; i++){
        if (strcmp(C[i], concept) == 0) *concept_index = i;
    }
}

/**
 * Get variable target index from array
 * 
 * @param int       target
 * @param (int*)    arr
 * @param int       size
 * @return int
*/
int get_target_array_index(int target, int * arr, int size){
    for (int index = 0; index < size; index++){
        if (arr[index] == target) return index;
    }
    return -1;
}

/**
 * Get similar values bw 2 variables,
 * set p1 and p2
 * 
 * @param (int*)    tmp_concepts
 * @param (int*)    C_in_round_indexes
 * @param (int*)    p1
 * @param (int*)    p2
 * @return int
*/
int get_similar_values_between_two_concepts_and_set_p1_and_p2(int * tmp_concepts, int * C_in_round_indexes, int * p1, int * p2){
    int concepts[CONCEPTS_NB] = {0};
    
    for (int r = 0; r < ROUNDS_NB; r++){
        for (int c = 0; c < CONCEPTS_NB; c++){
            if (tmp_concepts[c] == C_in_round_indexes[r]){
                concepts[c] = 1; break;
            }
        }
    }
    if (concepts[0] == 0) return 0;

    int size = CONCEPTS_NB - 1;
    int count = 0;
    for (int i = 0; i < size; i++){
        if (concepts[i] != concepts[i + 1]) count ++;
    }

    // For p1 and p2 parameters
    if (count == 2) {
        *p1 = 1; *p2 = 1;

        for (int i = 0; i < size; i++){
            if (concepts[i] != concepts[i + 1]) break;
            *p1 = *p1 + 1;
        }

        for (int i = size; 0 < i; i--){
            if (concepts[i] != concepts[i - 1]) break;
            *p2 = *p2 + 1;
        }
    }

    return count;
}

void copy_string(char *source, char *destination) {
    int i = 0;
    while (source[i] != '\0') {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
}

/**
 * Main function
 * 
 * @return int
*/
int main(void){
    fflush(stdout); // debug

    char input[100];
   
    // Init pythies
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    int NJ, J;
    if (sscanf(input, "%d %d", &NJ, &J) != 2) {
        fprintf(stderr, "Error parsing input when we init pythie\n");
        return EXIT_FAILURE;
    }

    // Init concepts
    char C[CONCEPTS_NB][MAX_CONCEPT_SIZE];
    for (int concept_index = 0; concept_index < CONCEPTS_NB; concept_index++) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error reading input when we init concept\n");
            return EXIT_FAILURE;
        }
        sscanf(input, "%15s", C[concept_index]);
    }
    
    // Init W S0..Sn
    // Can't sscanf...
    W_and_S all_W_and_S[WORDS_NB];
    for (int word_index = 0; word_index < WORDS_NB; word_index++){
        scanf("%s", all_W_and_S[word_index].word); // init W

        for (int score_index = 0; score_index < SCORES_NB; score_index++)
            scanf("%d", &all_W_and_S[word_index].scores[score_index]); // init S
    }
    
    /* STARTING GAME */
    /* STEP 1 */

    set_player_info_at_start(NJ);

    // Get concept in each rounds in 1rd step
    char C_in_round[20][MAX_CONCEPT_SIZE];
    int C_in_round_indexes[20];
    for (int round = 0; round < ROUNDS_NB; round++){
        readStdin(C_in_round[round], &C_in_round_indexes[round], C);
        printf("PASS\n"); fflush(stdout); // debug
        set_player_info_in_each_round(NJ);
    }

    char * guess_word = (char *) malloc(MAX_WORD_SIZE * sizeof(char));
    int p1, p2; // (10 - p), (10 + p)
    int target_index, min_count;

    // Find word and p
    for (int word_index = 0; word_index < WORDS_NB; word_index++){
        W_and_S my_W_and_S = all_W_and_S[word_index];
        int tmp_concepts[CONCEPTS_NB];
        populate_tmp_concepts_array(my_W_and_S, tmp_concepts); // sort tmp_concepts
        
        // Init p1 and p2
        int tmp_p1, tmp_p2;
        int count_similar_value = get_similar_values_between_two_concepts_and_set_p1_and_p2(tmp_concepts, C_in_round_indexes, &tmp_p1, &tmp_p2);
        if ((count_similar_value <= min_count) && (tmp_p1 < tmp_p2) && (abs(tmp_p1 + tmp_p2 - 20) < abs(p1 + p2 - 20))){
            target_index = word_index;
            min_count = count_similar_value;
            p1 = tmp_p1; p2 = tmp_p2;
        }
    }
    int p = 10 - p1; // Init p, between 3 and 7

    set_player_info_in_each_round(NJ);

    int a = 0, b = 0, c = 0;
    set_a_b_c_variables(&a, &b, &c, C_in_round_indexes, target_index, all_W_and_S, C);

    /* ALL STEPS */
    for (int step = 0; step < STEPS; step++){
        int words[WORDS_NB] = {0};
        int is_great_word = 0; // bool
        char save_concepts[MAX_CONCEPT_SIZE];
        char current_concepts[MAX_CONCEPT_SIZE];
        int save_concepts_index = -1;
        int current_concepts_index = -1;

        set_player_info_at_start(NJ);
        
        // EACH ROUNDS
        for (int round = 0; round < ROUNDS_NB; round++){
            readStdin(current_concepts, &current_concepts_index, C);

            // Loop to delete wrong answer
            for (int word_index = 0; word_index < WORDS_NB; word_index++){
                W_and_S my_W_and_S = all_W_and_S[word_index];
                int tmp_concepts[CONCEPTS_NB];
                populate_tmp_concepts_array(my_W_and_S, tmp_concepts);
                
                // delete wrong word not to be between p1 and p2 => p
                int target_index = get_target_array_index(current_concepts_index, tmp_concepts, CONCEPTS_NB);
                if (target_index >= (10 - p) && (target_index <= (49 - (10 + p)))) words[word_index] = 1;
                
                // delete wrong word with formula
                if (save_concepts_index != -1 && !is_formula_is_gretter_than_next(current_concepts_index, save_concepts_index, C, word_index, all_W_and_S, a, b, c)) words[word_index] = 1;
            }
            
            // Find the last one
            int sum_words = 0;
            for (int i = 0; i < WORDS_NB; i++) sum_words += words[i];

            // GUESS OR PASS
            if (is_great_word == 0 && sum_words == (WORDS_NB - 1)){
                int target_index = get_target_array_index(0, words, WORDS_NB);
                guess_word = all_W_and_S[target_index].word;
                printf("GUESS %s\n", guess_word);
                is_great_word = 1;
            } else printf("PASS\n");
            fflush(stdout);
            set_player_info_in_each_round(NJ);
            
            copy_string(current_concepts, save_concepts);
            save_concepts_index = current_concepts_index;
        }
    }

    free(guess_word);
    return EXIT_SUCCESS;
}