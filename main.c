#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <wctype.h>
#include <wchar.h>
#include <locale.h>

#define MAX_WORD_SIZE 30
#define MAX_UNIQUE_WORDS 10000

// check if a character is a vowel
bool is_vowel(wchar_t c) {
    c = towlower(c);
    return wcschr(L"aeiouаеиіїоуюя", c) != NULL; // English and Ukrainian vowels
}

// check if it has more vowels than consonants
bool has_more_vowels(const wchar_t *word) {
    int vowel_count = 0, consonant_count = 0;
    for (int i = 0; word[i] != L'\0'; i++) {
        if (is_vowel(word[i])) {
            vowel_count++;
        } else {
            consonant_count++;
        }
    }
    return vowel_count > consonant_count;
}

// Check if a word is in the unique words array
bool is_word_unique(const wchar_t *word, const wchar_t unique_words[MAX_UNIQUE_WORDS][MAX_WORD_SIZE + 1], int count) {
    for (int i = 0; i < count; i++) {
        if (wcscmp(word, unique_words[i]) == 0) {
            return false;
        }
    }
    return true;
}

void process_word(wchar_t *word, wchar_t unique_words[MAX_UNIQUE_WORDS][MAX_WORD_SIZE + 1], int *unique_count, FILE *outputFile) {
    if (is_word_unique(word, unique_words, *unique_count) && has_more_vowels(word)) {
        wprintf(L"%ls\n", word);
        if (outputFile) {
            fwprintf(outputFile, L"%ls\n", word);
            fflush(outputFile);
        }
        wcscpy(unique_words[*unique_count], word);
        (*unique_count)++;
        if (*unique_count >= MAX_UNIQUE_WORDS) {
            fwprintf(stderr, L"Reached maximum unique words limit. Exiting...\n");
            exit(1);
        }
    }
}


int main() {
    system("chcp 65001");
    setlocale(LC_ALL, "Ukrainian");

    wchar_t filepath[256];
    wchar_t outputPath[256];
    wchar_t choice;
    FILE *outputFile = NULL;

    wprintf(L"Enter the path to the input file: ");
    wscanf(L"%255ls", filepath);

    wprintf(L"Do you want to save output to a file? (y/n): ");
    wscanf(L" %lc", &choice);

    if (choice == L'y' || choice == L'Y') {
        wprintf(L"Enter the path to the output file: ");
        wscanf(L"%255ls", outputPath);
        outputFile = _wfopen(outputPath, L"w,ccs=UTF-8");
        if (!outputFile) {
            perror("Error creating output file");
            return 1;
        }
    }


    FILE *file = _wfopen(filepath, L"r,ccs=UTF-8");
    if (!file) {
        perror("Error opening input file");
        return 1;
    }

    wchar_t word[MAX_WORD_SIZE + 1];
    wchar_t unique_words[MAX_UNIQUE_WORDS][MAX_WORD_SIZE + 1];
    int unique_count = 0;
    int word_len = 0;
    wchar_t ch;

    wchar_t peek;

    while ((ch = fgetwc(file)) != WEOF) {
        if (iswalpha(ch)) {
            if (word_len < MAX_WORD_SIZE) {
                word[word_len++] = ch;
            }
        } else if (ch == L'\'' && word_len > 0) {
            peek = fgetwc(file);
            if (peek != WEOF && iswalpha(peek)) {
                if (word_len < MAX_WORD_SIZE) {
                    word[word_len++] = ch;
                }
                ungetwc(peek, file);
            } else {
            // End of word reached, process the word
                word[word_len] = L'\0';
                process_word(word, unique_words, &unique_count, outputFile);
                word_len = 0;
            }
        } else {
        // End of word reached, process the word
            if (word_len > 0) {
                word[word_len] = L'\0';
                process_word(word, unique_words, &unique_count, outputFile);
                word_len = 0;
            }
        }
    }



    if (word_len > 0) {
        word[word_len] = L'\0';
        if (is_word_unique(word, unique_words, unique_count) && has_more_vowels(word)) {
            wprintf(L"%ls\n", word);
            if (outputFile) {
                fwprintf(outputFile, L"%ls\n", word);
            }
        }
    }

    fclose(file);
    if (outputFile) {
        fclose(outputFile);
    }
    return 0;
}
