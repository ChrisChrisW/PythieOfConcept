#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONCEPTS_NB 50
#define MAX_CONCEPT_SIZE 16

#define WORDS_NB 1000
#define SCORES_NB 50
#define MAX_WORD_SIZE 26

#define ROUNDS_NB 20

#define CALIBRATION 5

/**
 * NJ J
*/
typedef struct {
    int NJ;
    int J;
} NJ_and_J;

/**
 * W S0..Sn variables
*/
typedef struct {
    char word[MAX_WORD_SIZE];
    int scores[CONCEPTS_NB];
} W_and_S;

/**
 * J P END values
*/
typedef struct {
    int J;
    int P;
    int END;
} Pythie_status_at_start;


/**
 * W J ST values
*/
typedef struct {
    char W[MAX_WORD_SIZE];
    int J;
    int ST;
} Pythie_status_in_round;

/**
 * Set player/pythie info at start
 * 
 * @param NJ_and_J NJ_and_J
 * @param Pythie_status_at_start Pythie_status_at_start
 * @return void
*/
void set_player_info_at_start(NJ_and_J NJ_and_J, Pythie_status_at_start Pythie_status_at_start){
    for (int player = 0; player < NJ_and_J.NJ; player++) {
        if (fscanf(stdin, "%d %d %d ", &(Pythie_status_at_start.J), &(Pythie_status_at_start.P), &(Pythie_status_at_start.END)) != 3) {
            fprintf(stderr, "Error reading player/pythie info at start\n");
        }
    }
}

/**
 * Set player/pythie info data in each round
 * 
 * @param NJ_and_J NJ_and_J
 * @param Pythie_status_in_round Pythie_status_in_round
 * @return void
*/
void set_player_info_in_each_round(NJ_and_J NJ_and_J, Pythie_status_in_round Pythie_status_in_round){
    for (int player = 0; player < NJ_and_J.NJ; player++) {
        if (fscanf(stdin, "%s %d %d ", Pythie_status_in_round.W, &(Pythie_status_in_round.J), &(Pythie_status_in_round.ST)) != 3) {
            fprintf(stderr, "Error reading player/pythie info in each round\n");
        }
    }
}

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

/**
 * 
 * @param (char*)               word
 * @param (char*)               concept
 * @param int                   s
 * @param int                   a
 * @param int                   b
 * @param int                   c
 * @return int
*/
int calculate_formula_value(char * word, char * concept, int s, int a, int b, int c) {
    int t = 0;
    int u = strlen(word) - strlen(concept);

    // Compare the word and concept strings
    while (word[t] == concept[t]) t++;

    t = strlen(word) - t;

    return a * s + 10 * b * t + 10 * c * u;
}

/**
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
int compare_concepts(int concept_index, int concept_next_index, char C[CONCEPTS_NB][MAX_CONCEPT_SIZE], int word_index, W_and_S all_W_and_S[WORDS_NB], int a, int b, int c) {
    char *word = all_W_and_S[word_index].word;
    int s = all_W_and_S[word_index].scores[concept_index];

    int f_current_value = calculate_formula_value(word, C[concept_index], s, a, b, c);
    int f_next_value = calculate_formula_value(word, C[concept_next_index], s, a, b, c);

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
                    if (compare_concepts(C_in_round_indexes[r], C_in_round_indexes[r + 1], C, target_index, all_W_and_S, a_value, b_value, c_value)){
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
    for (int index = 0; index < size; index++)
        if (arr[index] == target) return index;
        
    return -1;
}

/**
 * Get similar values bw 2 variables,
 * set p_substraction and p_addition
 * 
 * @param (int*)    tmp_concepts
 * @param (int*)    C_in_round_indexes
 * @param (int*)    p_substraction
 * @param (int*)    p_addition
 * @return int
*/
int get_similar_values_between_two_concepts_and_set_p_substraction_and_p_addition(int * tmp_concepts, int * C_in_round_indexes, int * p_substraction, int * p_addition){
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

    // For p_substraction and p_addition parameters
    if (count == 2) {
        *p_substraction = 1; *p_addition = 1;

        for (int i = 0; i < size; i++){
            if (concepts[i] != concepts[i + 1]) break;
            *p_substraction = *p_substraction + 1;
        }

        for (int i = size; 0 < i; i--){
            if (concepts[i] != concepts[i - 1]) break;
            *p_addition = *p_addition + 1;
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

    char input[100]; // buffer
   
    // Init NJ J
    NJ_and_J NJ_and_J;
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    if (sscanf(input, "%d %d", &(NJ_and_J.NJ), &(NJ_and_J.J)) != 2) {
        fprintf(stderr, "Error parsing input when we init pythie\n");
        return EXIT_FAILURE;
    }

    // Init C
    char C[CONCEPTS_NB][MAX_CONCEPT_SIZE];
    for (int concept_index = 0; concept_index < CONCEPTS_NB; concept_index++) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error reading input when we init concept\n");
            return EXIT_FAILURE;
        }
        sscanf(input, "%15s", C[concept_index]);
    }
    
    // Init W S0..Sn
    // Todo : Can't sscanf...
    W_and_S all_W_and_S[WORDS_NB];
    for (int word_index = 0; word_index < WORDS_NB; word_index++){
        scanf("%s", all_W_and_S[word_index].word); // init W

        for (int score_index = 0; score_index < SCORES_NB; score_index++)
            scanf("%d", &all_W_and_S[word_index].scores[score_index]); // init S
    }
    
    /* STARTING GAME */
    
    // Init J P END
    Pythie_status_at_start Pythie_status_at_start;
    // Init W J ST
    Pythie_status_in_round Pythie_status_in_round;

    /* STEP 1 */

    set_player_info_at_start(NJ_and_J, Pythie_status_at_start);
    
    // Get concept in each rounds in 1rd step
    char C_in_round[ROUNDS_NB][MAX_CONCEPT_SIZE];
    int C_in_round_indexes[ROUNDS_NB];
    for (int round = 0; round < ROUNDS_NB; round++){
        readStdin(C_in_round[round], &C_in_round_indexes[round], C);
        printf("PASS\n"); fflush(stdout); // debug
        set_player_info_in_each_round(NJ_and_J, Pythie_status_in_round);
    }
        
    int     a = 0, b = 0, c = 0;
    int     p = 7; // Init p, between 3 and 7
    int     p_substraction, p_addition; // (10 - p), (10 + p)
    int     min_count = 0, count_similar_value = 0;
    int     target_index;
    char    save_concepts[MAX_CONCEPT_SIZE];
    int     save_concepts_index;

    /* ALL STEPS */
    int phase = 1;
    while(1) {            
        // EACH ROUNDS
        char * guess_word = (char *) malloc(MAX_WORD_SIZE * sizeof(char));
        int is_great_word = 0; // bool
        int words[WORDS_NB] = {0};
        char current_concepts[MAX_CONCEPT_SIZE];
        int current_concepts_index;

        set_player_info_at_start(NJ_and_J, Pythie_status_at_start);
        
        for (int round = 0; round < ROUNDS_NB; round++){
            readStdin(current_concepts, &current_concepts_index, C);

            // Loop to calibrate p and to delete wrong word
            for (int word_index = 0; word_index < WORDS_NB; word_index++){
                W_and_S my_W_and_S = all_W_and_S[word_index];
                int tmp_concepts[CONCEPTS_NB];
                populate_tmp_concepts_array(my_W_and_S, tmp_concepts);
                
                // Calibrate p and search word
                int current_p_substraction, current_p_addition;
                count_similar_value = get_similar_values_between_two_concepts_and_set_p_substraction_and_p_addition(tmp_concepts, C_in_round_indexes, &current_p_substraction, &current_p_addition);
                if(count_similar_value <= min_count){
                    int sum_current_p = current_p_substraction + current_p_addition;
                    int sum_p = p_substraction + p_addition;
                    if (abs(sum_current_p - 20) < abs(sum_p - 20)){
                        min_count = count_similar_value;

                        target_index = word_index;
                        set_a_b_c_variables(&a, &b, &c, C_in_round_indexes, target_index, all_W_and_S, C);

                        p_addition = current_p_addition;
                        p_substraction = current_p_substraction; 
                        p = 10 - p_substraction;
                    }
                }

                // delete wrong word not to be between p_substraction and p_addition => p
                int target_index = get_target_array_index(current_concepts_index, tmp_concepts, CONCEPTS_NB);
                if (target_index >= (10 - p) && (target_index <= ((CONCEPTS_NB - 1) - (10 + p)))) words[word_index] = 1;
                // delete wrong word with formula
                if (save_concepts_index != -1 && !compare_concepts(current_concepts_index, save_concepts_index, C, word_index, all_W_and_S, a, b, c)) words[word_index] = 1;
            }
            
            // Find the last one
            int sum_words = 0;
            for (int i = 0; i < WORDS_NB; i++) sum_words += words[i];

            // GUESS OR PASS
            if (is_great_word == 0 && sum_words == (WORDS_NB - 1)){
                int target_index = get_target_array_index(0, words, WORDS_NB);
                printf("GUESS %s\n", all_W_and_S[target_index].word); // guess word
                is_great_word = 1;
            } else printf("PASS\n");
            fflush(stdout);
            set_player_info_in_each_round(NJ_and_J, Pythie_status_in_round);
            
            save_concepts_index = current_concepts_index;
            copy_string(current_concepts, save_concepts);
        }
        
        phase++;

        if(phase > 5) {
            free(guess_word);
            break;
        }
    }

    return EXIT_SUCCESS;
}