#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_WORD_SIZE 30
#define MAX_UNIQUE_WORDS 10000  // A limit for unique words

// Check if a character is a vowel
bool is_vowel(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

// Process a word to check if it has more vowels than consonants
bool has_more_vowels(const char *word) {
    int vowel_count = 0, consonant_count = 0;
    for (int i = 0; word[i] != '\0'; i++) {
        if (is_vowel(word[i])) {
            vowel_count++;
        } else {
            consonant_count++;
        }
    }
    return vowel_count > consonant_count;
}

// Check if a word is in the unique words array
bool is_word_unique(const char *word, const char unique_words[MAX_UNIQUE_WORDS][MAX_WORD_SIZE + 1], int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word, unique_words[i]) == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    char filepath[256];
    char outputPath[256];
    char choice;
    FILE *outputFile = NULL;

    printf("Enter the path to the input file: ");
    scanf("%255s", filepath);

    printf("Do you want to save output to a file? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        printf("Enter the path to the output file: ");
        scanf("%255s", outputPath);
        outputFile = fopen(outputPath, "w");
        if (!outputFile) {
            perror("Error creating output file");
            return 1;
        }
    }

    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Error opening input file");
        return 1;
    }

    char word[MAX_WORD_SIZE + 1];
    char unique_words[MAX_UNIQUE_WORDS][MAX_WORD_SIZE + 1];
    int unique_count = 0;
    int word_len = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF) {
        if (isalpha(ch)) {
            if (word_len < MAX_WORD_SIZE) {
                word[word_len++] = ch;
            }
        } else {
            if (word_len > 0) {
                word[word_len] = '\0';

                if (is_word_unique(word, unique_words, unique_count) && has_more_vowels(word)) {
                    printf("%s\n", word);
                    if (outputFile) {
                        fprintf(outputFile, "%s\n", word);
                    }
                    strcpy(unique_words[unique_count++], word);
                    if (unique_count >= MAX_UNIQUE_WORDS) {
                        fprintf(stderr, "Reached maximum unique words limit. Exiting...\n");
                        break;
                    }
                }

                word_len = 0;
            }
        }
    }

    if (word_len > 0) {
        word[word_len] = '\0';
        if (is_word_unique(word, unique_words, unique_count) && has_more_vowels(word)) {
            printf("%s\n", word);
            if (outputFile) {
                fprintf(outputFile, "%s\n", word);
            }
        }
    }

    fclose(file);
    if (outputFile) {
        fclose(outputFile);
    }
    return 0;
}
