#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include "dictionary.h"
#include <stdlib.h>
#include <check.h>


// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
//#define LENGTH 45
//#define HASH_SIZE 2000
//#define MAX_MISSPELLED 1000


/*typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}
node;

typedef node* hashmap_t;
*/

/**
 * Array misspelled is populated with words that are misspelled. Returns the length of misspelled.
 */
/**
 * Inputs:
 *  fp:         A file pointer to the document to check for spelling errors.
 *  hashtable:  The hash table used to determine spelling
 *  misspelled: An empty char* array to be populated with misspelled words.
 *              This array will never be greater than 1000 words long.
 *            
 * Returns:
 *  int:        The number of words in the misspelled arary.
 *
 * Modifies:
 *  misspelled: This array will be filled with misspelled words.
 *
 * Example:
 *  int num_misspelled = check_words(text_file, hashtable, misspelled);
 **/
int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]);

/**
 * Returns true if word is in dictionary else false.
 */
/**
 * Inputs:
 *  word:       A word to check the spelling of.
 *  hashtable:  The hash table used to determine spelling
 *            
 * Returns:
 *  bool:       A boolean value indicating if the word was correctly spelled.
 *
 * Modifies:
 *  
 * Example:
 *  bool correct  = check_word(word, hashtable);
 **/
bool check_word(const char* word, hashmap_t hashtable[]){
    int bucket = hash_function(word);
    hashmap_t cursor = hashtable[bucket];
    while(cursor != NULL){
	bool isMatch = true;
	while(isMatch){	
            for(int i = 0; i < strlen(word); i++){
	        if(tolower(word[i]) != cursor->word[i])
		    isMatch = false;
	        if(i = (strlen(word) - 1))
		    return true;

	    }
	}
	cursor = cursor->next;
    }
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
/**
 * Inputs:
 *  dictionary_file:    Path to the words file.
 *  hashtable:          The hash table to be populated.
 *            
 * Returns:
 *  bool:       Whether or not the hashmap successfully populated.
 *
 * Modifies:
 *  hashtable: This hashmap should be filled with words from the file provided.
 *
 * Example:
 *  bool success = load_dictionary("wordlist.txt", hashtable);
 **/
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]){
    for(int i=0; i < HASH_SIZE; i++){
        hashtable[i] = NULL;
    }
    FILE *fp = fopen(dictionary_file, "r");
    if(fp == NULL){
	printf("%s", "Failed to open file");
	return false;
    }

    int c;
    int bucket;
    int count=0;
    char buffer[LENGTH+1];
    while((c = fgetc(fp)) != EOF){
	//If c is a space, then have completed a word. Check if last character is a special character. If so, overwrite. 
	if(isspace(c)){
	    if(!isalpha(buffer[count - 1]))
		count = count - 1;
	    buffer[count] = '\0';
	    //Add word to hashtable
	    hashmap_t new_node = (hashmap_t)malloc(sizeof(node));
	    new_node->next = NULL;
	    for (int i = 0; i < count + 1; i++){
                new_node->word[i] = buffer[i];
	    }
	    new_node->word[count] = '\0';
            bucket = hash_function(new_node->word);
            if(hashtable[bucket] == NULL){
                hashtable[bucket] = new_node;
            }
	    else{
		new_node->next = hashtable[bucket];
                hashtable[bucket] = new_node;
	    }
	    count = 0;
	}
	//If count == LENGTH, and c is not a space, then we have reached the max length allowed for a word. To avoid a buffer overflow, return false.
	else if(count == LENGTH){
            return false;
        }
	else if((count == 0) & !(isalpha(c)))
	    continue;
        else{
	    c = tolower(c);
	    buffer[count] = c;
	    count += 1;
	}

    }
    fclose(fp);
    return true;
}

