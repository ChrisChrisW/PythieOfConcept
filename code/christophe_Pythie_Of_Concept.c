/**
 * The game consists of 20 rounds, in which Pythie must make a guess for each round. The program calculates the best guess for Pythie based on the available information and outputs it to stdout.
 * The program begins by reading in the number of concepts (NJ) and the number of words (J) from stdin, and then reading in the concepts themselves. Next, it reads in the words and their corresponding scores for each concept.
 * After this, the program enters a loop for each round of the game. In each round, it reads in the current word (W) and the score (ST) that Pythie has achieved so far. It then calculates the best guess for Pythie using a formula that takes into account the current word, the scores of the concepts, and the position of the guess in the current round.
 * The program uses several helper functions to perform these calculations, including functions to sort the scores, calculate the formula value, and determine the best guess based on the available information.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -- DEFINE -- */

#define CONCEPTS_NB 50
#define MAX_CONCEPT_SIZE 16

#define WORDS_NB 1000
#define SCORES_NB 50
#define MAX_WORD_SIZE 26

#define ROUNDS_NB 20

#define STEPS 5

/* -- END DEFINE -- */

/* -- STRUCT -- */

/**
 * NJ J
 * @param int       NJ
 * @param int       J
*/
typedef struct {
    int NJ;
    int J;
} NJ_and_J;

/**
 * W S0..Sn variables
 * 
 * @param (char *)      word
 * @param (int *)       scores
*/
typedef struct {
    char word[MAX_WORD_SIZE];
    int scores[CONCEPTS_NB];
} W_and_S;

/**
 * J P END values
 * 
 * @param int       J
 * @param int       P
 * @param int       END
*/
typedef struct {
    int J;
    int P;
    int END;
} Pythie_status_at_start;

/**
 * W J ST values
 * 
 * @param (char *)  W
 * @param int       J
 * @param int       ST
*/
typedef struct {
    char W[MAX_WORD_SIZE];
    int J;
    int ST;
} Pythie_status_in_round;

/* -- END STRUCT -- */

/* -- DECLARE FUNCTIONS -- */

int get_target_array_index(int target, int * arr, int size);
int calculate_formula_value(W_and_S W_and_S, int target_index, char * concept, int a, int b, int c);
void set_a_b_c_variables(int * a, int * b, int * c, int * save_concepts_indexes, int target_index);
void populate_tmp_concepts_array(W_and_S my_W_and_S, int tmp_concepts[CONCEPTS_NB]);
int get_similar_values_between_two_concepts_and_set_p_min_and_p_max(int * tmp_concepts, int * save_concepts_indexes, int * p_min, int * p_max);

/* -- END DECLARE FUNCTIONS -- */

/**
 * Main function
 * 
 * @return int
*/
int main(void){
    fflush(stdout); // debug

    /* -- INIT -- */
    
    char input[100]; // buffer
   
    // Init NJ J
    NJ_and_J NJ_and_J;
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error fgets input\n");
        return EXIT_FAILURE;
    }
    if (sscanf(input, "%d %d", &(NJ_and_J.NJ), &(NJ_and_J.J)) != 2) {
        fprintf(stderr, "Error sscanf NJ J\n");
        return EXIT_FAILURE;
    }

    // Init C
    char C[CONCEPTS_NB][MAX_CONCEPT_SIZE];
    for (int concept_index = 0; concept_index < CONCEPTS_NB; concept_index++) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error fgets C values\n");
            return EXIT_FAILURE;
        }
        sscanf(input, "%15s", C[concept_index]);
    }
    
    // Init W S0..Sn
    // TODO : Can't sscanf...
    W_and_S all_W_and_S[WORDS_NB];
    for (int word_index = 0; word_index < WORDS_NB; word_index++){
        scanf("%s", all_W_and_S[word_index].word); // init W

        for (int score_index = 0; score_index < SCORES_NB; score_index++)
            scanf("%d", &all_W_and_S[word_index].scores[score_index]); // init S
    }
    
    /* -- END INIT -- */
    /* -- STARTING GAME -- */
    
    // Init J P END
    Pythie_status_at_start Pythie_status_at_start;
    // Init W J ST
    Pythie_status_in_round Pythie_status_in_round;
        
    int a = 0, b = 0, c = 0;
    int p = 7; // Init p, between 3 and 7
    int p_min, p_max; // (10 - p), (10 + p)
    int min_count = 0, count_similar_value = 0;
    int save_concepts_indexes[ROUNDS_NB];

    /* -- STEPS -- */

    for (int step = 0; step < STEPS; step++){
        int is_great_word = 0; // bool
        int words[WORDS_NB] = {0};

        // Set player/pythie info at start
        // J P END status in Pythie_status_at_start struct
        for (int player = 0; player < NJ_and_J.NJ; player++) {
            if (fscanf(stdin, "%d %d %d ", &(Pythie_status_at_start.J), &(Pythie_status_at_start.P), &(Pythie_status_at_start.END)) != 3)
                fprintf(stderr, "Error fscanf player/pythie info at start\n");
        }

        /* -- ROUNDS -- */

        for (int round = 0; round < ROUNDS_NB; round++) {
            // Get concept index in each rounds
            fgets(input, CONCEPTS_NB, stdin);
            char *target_line = strtok(input, "\n"); 
            if (target_line != NULL) strcpy(input, target_line); // delete all \n
            for (int i = 0; i < CONCEPTS_NB; i++)
                if (strcmp(C[i], input) == 0) save_concepts_indexes[round] = i; // compares two strings and saves the index of the first string in an array if they are equal

            // Loop to calibrate p (also find a,b and c) and to delete wrong word
            for (int word_index = 0; word_index < WORDS_NB; word_index++) {
                W_and_S my_W_and_S = all_W_and_S[word_index];
                int tmp_concepts[CONCEPTS_NB];

                // Sort my_W_and_S in tmp_concepts
                populate_tmp_concepts_array(my_W_and_S, tmp_concepts);
                
                // Calibrate p and search word
                int current_p_min, current_p_max;
                count_similar_value = get_similar_values_between_two_concepts_and_set_p_min_and_p_max(tmp_concepts, save_concepts_indexes, &current_p_min, &current_p_max);
                
                if(count_similar_value <= min_count) {
                    int sum_current_p = current_p_min + current_p_max;
                    int sum_p = p_min + p_max;
                    
                    // Compare current p and saved p
                    if (abs(sum_current_p - 20) < abs(sum_p - 20)) {
                        min_count = count_similar_value;

                        p_max = current_p_max;
                        p_min = current_p_min;
                        // Set a p value
                        p = 10 - p_min;

                        // Init a,b and c
                        set_a_b_c_variables(&a, &b, &c, save_concepts_indexes, word_index);
                    }
                }

                // delete wrong word not to be between p_min and p_max => p
                int target_index = get_target_array_index(save_concepts_indexes[round], tmp_concepts, CONCEPTS_NB);
                if (target_index >= (10 - p) && (target_index <= ((CONCEPTS_NB - 1) - (10 + p)))) words[word_index] = 1;
                
                // delete wrong word with formula
                int f_current_value = calculate_formula_value(my_W_and_S, word_index, C[save_concepts_indexes[round - 1]], a, b, c);
                int f_next_value = calculate_formula_value(my_W_and_S, word_index, C[save_concepts_indexes[round]], a, b, c);
                if (save_concepts_indexes[round - 1] != -1 && (f_current_value > f_next_value)) words[word_index] = 1;
            }
            
            // Find the last one
            // calculates the sum of all words
            int sum_words = 0;
            for (int i = 0; i < WORDS_NB; i++) sum_words += words[i];

            // GUESS OR PASS
            if (is_great_word == 0 && sum_words == (WORDS_NB - 1)) {
                is_great_word = 1;
                int target_index = get_target_array_index(0, words, WORDS_NB);
                char * guess_word = all_W_and_S[target_index].word;
                printf("GUESS %s\n", guess_word);
            } else printf("PASS\n");
            fflush(stdout); // debug

            // Set player/pythie info data in each round
            // W J ST status in Pythie_status_in_round struct
            for (int player = 0; player < NJ_and_J.NJ; player++) {
                if (fscanf(stdin, "%s %d %d ", Pythie_status_in_round.W, &(Pythie_status_in_round.J), &(Pythie_status_in_round.ST)) != 3)
                    fprintf(stderr, "Error fscanf player/pythie info in each round\n");
            }
        }
    
        /* -- END ROUNDS -- */
    }

    /* -- END STEPS -- */
    /* -- END GAME -- */
    return EXIT_SUCCESS;
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
 * calculates a value based on the input parameters.
 * comparing two strings, word and concept, and using their lengths and s value, which is scores element in the W_and_S struct, to calculate the final result.
 * 
 * @param W_and_S               W_and_S
 * @param int                   target_index
 * @param (char*)               concept
 * @param int                   a
 * @param int                   b
 * @param int                   c
 * @return int
*/
int calculate_formula_value(W_and_S W_and_S, int target_index, char * concept, int a, int b, int c){
    char *word = W_and_S.word;
    int u = strlen(word) - strlen(concept);

    int t = 0;
    // Compare the word and concept strings
    while (word[t] == concept[t]) t++;
    t = strlen(word) - t;

    int s = W_and_S.scores[target_index];

    return a * s + 10 * b * t + 10 * c * u;
}

/**
 * Set A, B and C variables with average method
 * 
 * @param (int*)                a
 * @param (int*)                b
 * @param (int*)                c
 * @param (int*)                save_concepts_indexes
 * @param int                   target_index
 * @return void
*/
void set_a_b_c_variables(int * a, int * b, int * c, int * save_concepts_indexes, int target_index) {
    // // TODO : try something else but I try some values to set and I find some value which working in a,b and c
    // *a = save_concepts_indexes[target_index] * 0.5;
    // *b = save_concepts_indexes[target_index] * 0.8;
    // *c = save_concepts_indexes[target_index] * 0.3;

    int count_concept_in_round = 0;
    int tmp_a = 0, tmp_b = 0, tmp_c = 0;

    // Set tmp values and counts the number of times target_index appears in the save_concepts_indexes
    for (int concept_index = 0; concept_index < CONCEPTS_NB; concept_index++){
        if (save_concepts_indexes[concept_index] == target_index){
            count_concept_in_round++;

            switch (count_concept_in_round) {
            case 1:
                tmp_a = concept_index;
                break;
            case 2:
                tmp_b = concept_index;
                break;
            case 3:
                tmp_c = concept_index;
                break;
            default:
                break;
            }
        }
    }

    // Set a,b and c values
    *a = tmp_a; *b = tmp_b; *c = tmp_c;
    switch (count_concept_in_round) {
            case 1:
                *b = -1;
                *c = -1;
                break;
            case 2:
                *c = -1;
                break;
            default:
                break;
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
 * Get similar values bw 2 variables,
 * set p_min and p_max
 * 
 * @param (int*)    tmp_concepts
 * @param (int*)    save_concepts_indexes
 * @param (int*)    p_min
 * @param (int*)    p_max
 * @return int
*/
int get_similar_values_between_two_concepts_and_set_p_min_and_p_max(int * tmp_concepts, int * save_concepts_indexes, int * p_min, int * p_max){
    int concepts[CONCEPTS_NB] = {0};
    
    // Mark concepts that appear in both tmp_concepts and save_concepts_indexes
    for (int r = 0; r < ROUNDS_NB; r++){
        for (int c = 0; c < CONCEPTS_NB; c++){
            if (tmp_concepts[c] == save_concepts_indexes[r]){
                concepts[c] = 1; break;
            }
        }
    }
    // Return 0 if no common concepts are found
    if (concepts[0] == 0) return EXIT_SUCCESS;

    int size = CONCEPTS_NB - 1;

    // Count the number of transitions between similar and different concepts
    int count = 0;
    for (int i = 0; i < size; i++){
        if (concepts[i] != concepts[i + 1]) count ++;
    }

    // If there are two transitions, set p_min and p_max
    if (count == 2) {
        *p_min = 1; *p_max = 1;

        for (int i = 0; i < size; i++){
            if (concepts[i] != concepts[i + 1]) break;
            *p_min = *p_min + 1;
        }

        for (int i = size; 0 < i; i--){
            if (concepts[i] != concepts[i - 1]) break;
            *p_max = *p_max + 1;
        }
    }

    return count;
}