/**
 * The game consists of 20 rounds, in which Pythie must make a guess for each round. The program calculates the best guess for Pythie based on the available information and outputs it to stdout.
 * The program begins by reading in the number of concepts (NJ) and the number of words (J) from stdin, and then reading in the concepts themselves. Next, it reads in the words and their corresponding scores for each concept.
 * After this, the program enters a loop for each round of the game. In each round, it reads in the current word (W) and the score (ST) that Pythie has achieved so far. It then calculates the best guess for Pythie using a formula that takes into account the current word, the scores of the concepts, and the position of the guess in the current round.
 * The program uses helper functions to perform these calculations, including functions to sort the scores and determine the best guess based on the available information.
 * 
 * 
 * 1) The function "populate_sort_concepts_array" sorts the scores of the concepts in the "my_W_and_S" struct and populates the "sort_concepts" array with them.
 * It takes in two parameters, a "W_and_S" struct called "my_W_and_S" and an integer array called "sort_concepts".
 * It creates a new array called "scores" and copies the scores from the "my_W_and_S" struct to the "scores" array.
 * It then uses a nested loop to sort the "scores" array in ascending order using the bubble sort algorithm.
 * Next, it initializes some variables to keep track of the last target score and last target index.
 * Then, it enters a loop to iterate through the sorted "scores" array.
 * For each score in the sorted array, it uses another nested loop to find the index of that score in the original scores array of the "my_W_and_S" struct.
 * It then assigns the index to the corresponding index in the "sort_concepts" array.
 * The last_target_score and last_target_index variables are updated with the current score and index.
 * When the loop is finished, the "sort_concepts" array will be populated with the indexes of the scores sorted in ascending order.
 * 
 * 
 * 2) The function "get_similar_values_between_two_concepts_and_set_p_min_and_p_max" takes two arrays of integers and pointers to two integers, it creates a hash table of the second array, mark the common elements in the two arrays, counts the number of transitions between similar and different elements, if the number of transitions is two it sets the two pointers with the position of the transitions, and returns the number of transitions.
 * Initialize an array called "concepts" of size "CONCEPTS_NB" with all elements set to 0.
 * Initialize an array called "hash_table" of size "CONCEPTS_NB" with all elements set to 0.
 * Loop through the "save_concepts_indexes" array and for each element, set the corresponding element in the "hash_table" array to 1.
 * Loop through the "sort_concepts" array, for each element, check if the corresponding element in the "hash_table" array is set to 1. If so, set the corresponding element in the "concepts" array to 1.
 * Initialize a variable "count" to 0.
 * Loop through the "concepts" array, starting from the first element and ending at the second to last element. For each element, check if it is different from the next element in the array. If so, increment "count" by 1.
 * If "count" is equal to 2, initialize two variables "tmp_min" and "tmp_max" to 1.
 * Loop through the "concepts" array, starting from the first element, and for each element that is equal to the next element, increment "tmp_min" by 1.
 * Loop through the "concepts" array, starting from the last element, and for each element that is equal to the previous element, increment "tmp_max" by 1.
 * Set the values of "p_min" and "p_max" to "tmp_min" and "tmp_max" respectively.
 * Return "count".
 *
 *  
 * 3) The part delete wrong word with a function "get_target_array_index" select the potential word.
 * This code checks if the current word's score for the current round's concept is within a certain range relative to the value of p, and if it is, marks it as valid by setting its corresponding index in the "words" array to 1, otherwise it eliminates it as a potential guess.
 * (word_index >= (10 - p) && (target_index <= ((word_index - alpha) - (10 + p))))
 * The alpha value is the experience of several trials.
 * 
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
 * @param int NJ
 * @param int J
*/
typedef struct {
    int NJ;
    int J;
} NJ_and_J;

/**
 * W S0..Sn variables
 * 
 * @param (char *) word
 * @param (int *) scores
*/
typedef struct {
    char word[MAX_WORD_SIZE];
    int scores[CONCEPTS_NB];
} W_and_S;

/**
 * J P END values
 * 
 * @param int J
 * @param int P
 * @param int END
*/
typedef struct {
    int J;
    int P;
    int END;
} Pythie_status_at_start;

/**
 * W J ST values
 * 
 * @param (char *) W
 * @param int J
 * @param int ST
*/
typedef struct {
    char W[MAX_WORD_SIZE];
    int J;
    int ST;
} Pythie_status_in_round;

/* -- END STRUCT -- */

/* -- DECLARE FUNCTIONS -- */

int get_target_array_index(int target, int * arr, int size);
void populate_sort_concepts_array(W_and_S my_W_and_S, int sort_concepts[CONCEPTS_NB]);
int get_similar_values_between_two_concepts_and_set_p_min_and_p_max(int * sort_concepts, int * save_concepts_indexes, int * p_min, int * p_max);

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
    W_and_S all_W_and_S[WORDS_NB];
    for (int word_index = 0; word_index < WORDS_NB; word_index++) {
        // init W
        if(scanf("%s", all_W_and_S[word_index].word) == 0) {
            fprintf(stderr, "Error fgets word values\n");
            return EXIT_FAILURE;
        }

        for (int score_index = 0; score_index < SCORES_NB; score_index++)
            // init S
            if (scanf("%d", &all_W_and_S[word_index].scores[score_index]) == 0) {
                fprintf(stderr, "Error fgets scores values\n");
                return EXIT_FAILURE;
            }
    }
    
    /* -- END INIT -- */
    /* -- STARTING GAME -- */
    
    // Init J P END
    Pythie_status_at_start Pythie_status_at_start;
    // Init W J ST
    Pythie_status_in_round Pythie_status_in_round;
   
    int save_concepts_indexes[ROUNDS_NB];
    int upper_p = 7, lower_p = 3;
    int p = (rand() % ((upper_p + 1) - (lower_p - 1) + 1)) + (lower_p - 1); // Init p, between 3 and 7
    p = 3;
    int p_min, p_max; // (10 - p), (10 + p)
    int min_count_similar_value = CONCEPTS_NB - 2, count_similar_value = 0;

    /* -- STEPS -- */
    for (int step = 0; step < STEPS; step++){
        int is_great_word = 0; // bool
        int p_found_value = 0;
        int words[WORDS_NB] = {0};

        /* -- SET PLAYER/PYTHIE INFO AT START -- */
        for (int player = 0; player < NJ_and_J.NJ; player++) {
            // J P END status in Pythie_status_at_start struct
            if (fscanf(stdin, "%d %d %d ", &(Pythie_status_at_start.J), &(Pythie_status_at_start.P), &(Pythie_status_at_start.END)) != 3)
                fprintf(stderr, "Error fscanf player/pythie info at start\n");
        }
        /* -- END SET PLAYER/PYTHIE INFO AT START -- */

        W_and_S my_W_and_S; // W S0...Sn

        /* -- ROUNDS -- */
        for (int round = 0; round < ROUNDS_NB; round++) {
            /* -- GET CONCEPT AND INDEX IN EACH ROUNDS -- */
            fgets(input, MAX_CONCEPT_SIZE, stdin);
            char *target_line = strtok(input, "\n"); 
            if (target_line != NULL) strcpy(input, target_line); // delete all \n
            for (int i = 0; i < CONCEPTS_NB; i++)
                if (strcmp(C[i], input) == 0) save_concepts_indexes[round] = i; // compares two strings and saves the index of the first string in an array if they are equal
            /* -- END GET CONCEPT AND INDEX IN EACH ROUNDS -- */

            // Check if we find secret word, else we clean the word variable
            if(strcmp(target_line, "NULL") == 0) is_great_word = 1;
            else for (int i = 0; i < WORDS_NB; i++) words[WORDS_NB] = 0;

            // Loop to calibrate p and to delete wrong word
            for (int word_index = 0; word_index < WORDS_NB; word_index++) {
                my_W_and_S = all_W_and_S[word_index];
                int sort_concepts[CONCEPTS_NB];
        
                // Sort my_W_and_S in sort_concepts
                populate_sort_concepts_array(my_W_and_S, sort_concepts);
                
                // Calibrate p and search word
                int current_p_min, current_p_max;
                count_similar_value = get_similar_values_between_two_concepts_and_set_p_min_and_p_max(sort_concepts, save_concepts_indexes, &current_p_min, &current_p_max);
                int isMaxIsGreaterThanMin = current_p_min < current_p_max;

                if(isMaxIsGreaterThanMin == 1 && count_similar_value <= min_count_similar_value) {
                    int tmp_p = 10 - current_p_min;

                    if (tmp_p >= lower_p && tmp_p <= upper_p) {
                        int sum_current_p = abs(current_p_min + current_p_max - 20);
                        int sum_p = abs(p_min + p_max - 20);
                                        
                        // Compare current p and saved p
                        if (sum_current_p <= sum_p) {
                            p_found_value = word_index;
                            min_count_similar_value = count_similar_value;
                            p_max = current_p_max;
                            p_min = current_p_min;

                            p = tmp_p; // Set a p value
                        }
                    }
                }

                // delete wrong word not to be between p_min and p_max => p
                int target_index = get_target_array_index(save_concepts_indexes[round], sort_concepts, CONCEPTS_NB);
                int alpha = 1;
                if(p >= lower_p && p <= 4) alpha = 2;
                if (target_index >= (10 - p) && (target_index <= ((CONCEPTS_NB - alpha) - (10 + p)))) words[word_index] = 1;
            }
            
            // Find the last one
            // calculates the sum of all words to find 999 so we have a word with index = 0
            int sum_words = 0;
            for (int i = 0; i < WORDS_NB; i++) sum_words += words[i];

            /* -- GUESS AND PASS -- */
            if (is_great_word == 0 && sum_words == (WORDS_NB - 1)) {
                int target_index = get_target_array_index(0, words, WORDS_NB); // target value 0
                char * guess_word = all_W_and_S[target_index].word;
                printf("GUESS %s\n", guess_word);
            } 
            else if (is_great_word == 0 && p_found_value != 0) {
                // We are sure to find value in first round
                char * guess_word = all_W_and_S[p_found_value].word;
                printf("GUESS %s\n", guess_word);
                p_found_value = 0;
            }
            else printf("PASS\n");
            fflush(stdout); // debug
            /* -- END GUESS AND PASS -- */

            /* -- SET PLAYER/PYTHIE INFO DATA IN EACH ROUND -- */
            for (int player = 0; player < NJ_and_J.NJ; player++) {
                // W J ST status in Pythie_status_in_round struct
                if (fscanf(stdin, "%s %d %d ", Pythie_status_in_round.W, &(Pythie_status_in_round.J), &(Pythie_status_in_round.ST)) != 3)
                    fprintf(stderr, "Error fscanf player/pythie info in each round\n");
            }
            /* -- END SET PLAYER/PYTHIE INFO DATA IN EACH ROUND -- */
        }
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
int get_target_array_index(int target, int * arr, int size) {
    for (int index = 0; index < size; index++)
        if (arr[index] == target) return index;
        
    return -1;
}

/**
 * Sorts an array of scores in ascending order,
 * then populates another array with the indices of the original scores array in the sorted order.
 * 
 * @param (W_and_S) my_W_and_S
 * @param (int*) sort_concepts
 * @return void
*/
void populate_sort_concepts_array(W_and_S my_W_and_S, int sort_concepts[CONCEPTS_NB]) {
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

    // Populate the sort_concepts array using the sorted scores array
    int last_target_score = 0, last_target_index = 0;
    for (int i = 0; i < CONCEPTS_NB; i++) {
        int target_score = scores[i];
        // Find the index of the target score in the original scores array
        for (int j = 0; j < CONCEPTS_NB; j++) {
            if (my_W_and_S.scores[j] == target_score && (last_target_score != target_score || j > last_target_index)) {
                sort_concepts[i] = j;
                break;
            }
        }

        last_target_score = target_score;
        last_target_index = sort_concepts[i];
    }
}

/**
 * Find the similar values between two input arrays and 
 * set the minimum and maximum position of the similar values using a hash map.
 * 
 * @param (int*)    sort_concepts
 * @param (int*)    save_concepts_indexes
 * @param (int*)    p_min
 * @param (int*)    p_max
 * @return int
*/
int get_similar_values_between_two_concepts_and_set_p_min_and_p_max(int * sort_concepts, int * save_concepts_indexes, int * p_min, int * p_max) {
    int concepts[CONCEPTS_NB] = {0};
    int hash_table[CONCEPTS_NB] = {0};

    // Create a hash table for the save_concepts_indexes array
    for (int r = 0; r < ROUNDS_NB; r++)
        hash_table[save_concepts_indexes[r]] = 1;

    // Mark concepts that appear in both sort_concepts and save_concepts_indexes
    for (int c = 0; c < CONCEPTS_NB; c++)
        if (hash_table[sort_concepts[c]] == 1) concepts[c] = 1;

    int size = CONCEPTS_NB - 1;

    // Count the number of transitions between similar and different concepts
    int count = 0;
    for (int i = 0; i < size; i++)
        if (concepts[i] != concepts[i + 1]) count ++;

    // If there are two transitions, set p_min and p_max
    if (count == 2) {
        int tmp_min = 1, tmp_max = 1;

        for (int i = 0; i < size; i++) {
            if (concepts[i] != concepts[i + 1]) break;
            tmp_min++;
        }

        for (int i = size; 0 < i; i--) {
            if (concepts[i] != concepts[i - 1]) break;
            tmp_max++;
        }

        *p_min = tmp_min; *p_max = tmp_max;
    }

    return count;
}