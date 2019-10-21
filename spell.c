#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include "dictionary.h"
#include <stdlib.h>
#include <string.h>

bool check_word(const char* word, hashmap_t hashtable[]){
     if(strlen(word) > LENGTH)
         return false;
     char buffer[LENGTH + 1];
     for (int i = 0; i < strlen(word); i++){
         buffer[i] = tolower(word[i]);
     }
     buffer[strlen(word)] = '\0';
     int bucket = hash_function(buffer);
     hashmap_t cursor = hashtable[bucket];
     while(cursor != NULL){
         bool isMatch = true;
         while(isMatch){
             for(int i = 0; i < strlen(buffer); i++){
                 if(buffer[i] != cursor->word[i]){
                     isMatch = false;
                     break;
                 }
                 if(i == (strlen(buffer) - 1)){
                     return true;
                 }
             }
         }
         cursor = cursor->next;
    }
    return false;
}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {
    int num_misspelled = 0;
    size_t len = 0;
    ssize_t read;
    char * line = NULL;
    char buffer[LENGTH + 1];
    
    int startWord = 0;
    int endWord = 0;
    int count = 0;
    while((read = getline(&line, &len, fp)) != EOF){
        for(int i = 0; i < strlen(line); i++){
            if(isspace(line[i])){
                endWord = i;
                if((endWord-startWord) > LENGTH){
                    count = 0;
                    startWord = i+1;
                }
                for(int j = startWord;j < endWord;j++){
                    if(j == startWord){
                        if(isalpha(line[j])){
                            buffer[count] = line[j];
                            count += 1;
                        }
                    }
                    else if(j == (endWord - 1)){
                        //printf("last letter %c,%d\n", line[j], count);
                        if(isalpha(line[j])){
                            buffer[count] = line[j];
                            count += 1;
                        }
                    }
                    else{
                        buffer[count] = line[j];
                        count += 1;
                    }
                }
                buffer[count] = '\0';
                //printf("%s\n", buffer);
                if(!check_word(buffer, hashtable)){
                    //printf("%s, Misspelled\n", buffer);
                    misspelled[num_misspelled] = (char*)malloc((LENGTH + 1)*sizeof(char));
                    strncpy(misspelled[num_misspelled], buffer, (count + 1));
                    misspelled[num_misspelled][count] = '\0';
                    num_misspelled += 1;
                }
                count = 0;
                startWord = i+1;
            }
        }
    }
    return num_misspelled;
}
/*
int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {
    int num_misspelled = 0;
    size_t len = 0;
    ssize_t read;
    char * line = NULL;
    char buffer[LENGTH + 1];
    char *rest = NULL;
    while((read = getline(&line, &len, fp)) != EOF){
        char *token = strtok_r(line, " \r\n\t", &rest);
        while(token){
            if(strlen(token) > LENGTH)
                token = strtok_r(NULL, " \r\n\t", &rest);
            int count = 0;
            for(int i = 0; i<strlen(token); i++){
                if(i == 0){
                    if(isalpha(token[i])){
                        buffer[count] = token[i];
                        count += 1;
                    }
                }
                else if(i == (strlen(token) - 1)){
                    if(isalpha(token[i])){
                        buffer[count] = token[i];
                        count += 1;
                    }
                }
                else{
                    buffer[count] = token[i];
                    count += 1;
                }
            }
            buffer[count] = '\0';
            if(!check_word(buffer, hashtable)){
                misspelled[num_misspelled] = (char*)malloc((LENGTH + 1)*sizeof(char));
                strncpy(misspelled[num_misspelled], buffer, (count + 1));
                misspelled[num_misspelled][count] = '\0';
                num_misspelled += 1;
            }
            token = strtok_r(NULL, " \r\n\t", &rest);
        }
    }
    return num_misspelled;
}

*/

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]){
    for(int i=0; i < HASH_SIZE; i++){
        hashtable[i] = NULL;
    }
    FILE *fp = fopen(dictionary_file, "r");
    if(fp == NULL){
        perror("fopen");
        exit(1);
    }
    int c;
    int bucket;
    int count=0;
    char buffer[LENGTH+1];
    while((c = fgetc(fp)) != EOF){
        //If count == LENGTH, and c is not a space, then we have reached the max length allowed for a word. To avoid a buffer overflow, return false.
        if(count > LENGTH){
            if(isspace(c)){
                count = 0;
            }
        }
        //If c is a space, then have completed a word. Check if last character is a special character. If so, overwrite.
        else if(isspace(c)){
            //if(!(isalpha(buffer[count - 1]))){
            //    count = count - 1;
            //}
            buffer[count] = '\0';
            //Add word to hashtable
            hashmap_t new_node = (hashmap_t)malloc(sizeof(node));
            new_node->next = NULL;
            //strncpy(new_node->word, buffer, count);
            for (int i = 0; i < count + 1; i++){
                new_node->word[i] = buffer[i];
            }
            new_node->word[count] = '\0';
            //int hash_function(const char* word);
            bucket = hash_function(new_node->word);
            if(hashtable[bucket] == NULL){
                hashtable[bucket] = new_node;
            }
            else{
                new_node->next = hashtable[bucket];
                hashtable[bucket] = new_node;
            }
            count = 0;
            //buffer[count] = '\0';
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
