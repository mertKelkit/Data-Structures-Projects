/*
 * Mert KELKİT
 * 150115013
 * Project#1
 */

/* I didn't include the text files to my submission in order to prevent file complexity */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
//standard file names
#define FIRST_FILE "file1.txt"
#define SECOND_FILE "file2.txt"
#define EXISTS 1
#define DOESNT_EXIST 0

//node struct for words and their lengths
typedef struct wn {
    int len;
	char* word;
	struct wn* next;
} word_node;

//node struct for letters as char
typedef struct ln {
	char letter;
	struct ln* next;
} letter_node;

/*function prototypes*/
//this function fills linked lists with words from text files
word_node* fill_linked_list(char*);
//this function inserts letters to the letter linked list
letter_node* insert_letter(letter_node*, char);
//this function clears letter linked list
void empty_list(letter_node**);
//this function fills word nodes with the letter node's every chars
word_node* letters_to_word(letter_node*, word_node*, int, char*);
//this function returns indexed letter from letter linked list
char get_letter(letter_node*, int);
//this function finds common words from two linked lists which are filled with text files
void find_common_words(word_node*, word_node*);
//this function inserts found common words to the "common words" linked list
word_node* insert_common_word(word_node*, char*, int);
//this function checks that a common word is inserted to the "common words" linked list before or not
int common_exists(word_node*, char*);
//this function prints common words
void print_common_words(word_node*, size_t);
//this function finds two-gram words from text files and linked lists which include text files' datas
void find_two_grams(word_node*, word_node*);
//this function inserts two-gram words
word_node* insert_two_grams(word_node*, char*, char*, int, int, size_t*);
//this function checks that a two-gram word group is inserted to the "2-gram words" linked list before or not
int two_grams_exists(word_node*, char*);
//this fucntion prints two-grams
void print_two_grams(word_node*, size_t);
//this function sorts words with alphabetically ascending order with word nodes
word_node* sort_words(word_node*);

int main() {
    clock_t begin = clock();    //this is for execution time
    //fill first linked list with first text file
	word_node* first_list = fill_linked_list(FIRST_FILE);
    //fill second linked list with second text file
	word_node* second_list = fill_linked_list(SECOND_FILE);
    //finds common words between first text file and second text file
    find_common_words(first_list, second_list);
    printf("\n");
    //finds 2-gram words between first text file and second text file
    find_two_grams(first_list, second_list);

    /*these are for execution time*/
    clock_t end = clock();
    printf("Execution time is -> %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
	return 0;
}

word_node* fill_linked_list(char* file_name) {
	word_node* words = NULL;
	letter_node* letters = NULL;
	FILE* file;
	char* word = NULL;
	char temp;
	int counter = 0;
    //open text file
	if(file = fopen(file_name, "r")) {
        //until the text file ends...
		while(!feof(file)) {
            //get the char which cursor points
			temp = fgetc(file);
            //check that if the temp char is end of the word
            //program checks also punctuations
			if(temp == ' ' || temp == EOF || (temp <= 47 && temp >= 33) || (temp <= 64 && temp >= 58) || (temp <= 96 && temp >= 91)) {
                //if temp is the end of the word and there is some letters in the linked list...
				if(letters != NULL) {
                    //convert letters to a word node with given linked lists, lengths and with a char* variable "word".
					words = letters_to_word(letters, words, counter, word);
                    //and then empty letter container linked list.
					empty_list(&letters);
                    //set conunter to 0 because we need to calculate another word's length.
					counter = 0;
				}
                //if there is no letters in the letter node(s), just continue to loop and check
				continue;
			}
            //if a letter is a capital letter
			if(temp >= 65 && temp <= 90)
                //make it lower letter(?)
            	temp = tolower(temp);
            //insert valid letters to the letter node in order to create a valid word
            letters = insert_letter(letters, temp);
            //increment counter because of getting the length of the word.
            counter++;
		}
        //eventually close the text file
		fclose(file);
	}
    //and then return the linked list which has words
	return words;
}

letter_node* insert_letter(letter_node* letters, char c) {
    //if letter node is null, open a new node and set it's letter as given parameter "c"
    //then set it's next pointer to NULL in order to prevent wrong accesses
    //and then return the letter linked list
	if (letters == NULL) {
		letters = (letter_node*)malloc(sizeof(letter_node));
		letters->letter = c;
		letters->next = NULL;
		return letters;
	}
    //if list is not empty
	letter_node* iter = letters;
    //go until the last node
	while (iter->next != NULL)
		iter = iter->next;
    //and allocate some memory for it's next and move the iterator node to the next of it
	iter->next = (letter_node*)malloc(sizeof(letter_node));
	iter = iter->next;
    //set it's letter to given character "c"
	iter->letter = c;
    //in order to prevent bad access
	iter->next = NULL;
    //then return the linked list
	return letters;
}

word_node* letters_to_word(letter_node* ln, word_node* wn, int counter, char* word) {
    int i;
    //allocate memory for our word with len+1 so as to put NULL to it's ending
    word = (char*)malloc(counter+1);
    for(i=0; i<counter; i++)
        //get every single letter from the linked list and insert to the word variable which has the type char*
        *(word+i) = get_letter(ln, i);
    //and put NULL to the end
    *(word+i) = '\0';
    //in order to insert word
    //if word list is empty, allocate some memory for it, set it's word to local variable word
    //set it's length to counter which is number of letters
    //and set it's next to NULL
    if (wn == NULL) {
		wn = (word_node*)malloc(sizeof(word_node));
		wn->word = word;
        wn->len = counter;
		wn->next = NULL;
		return wn;
	}
    //if word list is not empty
	word_node* iter = wn;
    //go to the last node of the list
	while (iter->next != NULL)
		iter = iter->next;
    //then allocate memory for the next of it and go to the next of it with iter node
	iter->next = (word_node*)malloc(sizeof(word_node));
	iter = iter->next;
    //set it's word and length
	iter->word = word;
    iter->len = counter;
    //and put a NULL next of it
	iter->next = NULL;
    //then return the word linked list
	return wn;
}

char get_letter(letter_node* ln, int index) {
    int i;
    //it's like random access of arrays, go "index" times next of the given letter linked list
    for(i=0; i<index; i++)
        ln = ln->next;
    //then return wanted letter
    return ln->letter;
}

void empty_list(letter_node** ln) {
    //start deleting from the first node
    letter_node* del = *ln;
    //we don't want to miss next nodes, so there is a forward node
    letter_node* fw;
    while(del != NULL) {
        //set forward next of the node which wanted to be deleted
        fw = del->next;
        //delete del node
        free(del);
        //and then let the forward node to be the node which wanted to be deleted
        del = fw;
    }
    //then set the head of the list to NULL in order to use again safely
    *ln = NULL;
}

void find_common_words(word_node* first_list, word_node* second_list) {
    size_t ctr = 0;
	word_node* common_words = NULL;
	word_node* first_iter;
	word_node* second_iter;
    //wanders around words in the first text file
	for(first_iter=first_list; first_iter!=NULL; first_iter=first_iter->next) {
        //if the word from the first text file was inserted before, just continue to outer loop
        if (common_exists(common_words, first_iter->word) == EXISTS)
            continue;
        //wanders around words in the second text file
        for (second_iter=second_list; second_iter!=NULL; second_iter=second_iter->next) {
            //if the words' lengths are equal to each other, continue checking
            if (first_iter->len == second_iter->len) {
                //if the words are equivalent to each oter
                if (!strcmp(first_iter->word, second_iter->word)) {
                    //same word
                    //insert them to the common words linked list
                    common_words = insert_common_word(common_words, first_iter->word, first_iter->len);
                    //and increment the number of common words
                    ctr++;
                    //then exit the loop because one of the common words is found
                    break;
                }
            }
        }
    }
    //sort words and then print
    common_words = sort_words(common_words);
    print_common_words(common_words, ctr);
}

word_node* insert_common_word(word_node* common, char* word, int len) {
    //if the list is NULL, allocate memory
    //set the word and length of it
    //and set it's next to NULL for safety
    //then return it
	if(common == NULL) {
		common = (word_node*)malloc(sizeof(word_node));
		common->word = (char*)malloc(len+1);
		common->word = word;
		common->len = len;
		common->next = NULL;
		return common;
	}
    //if the list is not empty
	word_node* iter = common;
    //use an iterator in order to go to the next node
	while(iter->next != NULL)
		iter = iter->next;
    //then allocate some memory
    //and then set the word and the length of it
	iter->next = (word_node*)malloc(sizeof(word_node));
	iter = iter->next;
	iter->word = (char*)malloc(len+1);
	iter->word = word;
	iter->len = len;
    //set next to NULL for safety
	iter->next = NULL;
    //then return the list
	return common;
}

int common_exists(word_node* common, char* word) {
    //wanders around every word in the common words linked list
    while(common != NULL) {
        //if it finds the same word in it
		if(!strcmp(word, common->word))
            //returns an integer in order to show it exists
			return EXISTS;
		common = common->next;
	}
    //if a word does not exist...
	return DOESNT_EXIST;
}

void print_common_words(word_node* common, size_t ctr) {
    //prints number of the common words
    printf("Number of common words: %zu\n", ctr);
    printf("Common words: \n");
    //then prints every common words in the linked list
    while(common != NULL) {
        printf("%s\n", common->word);
        common = common->next;
    }
}

void find_two_grams(word_node* first_list, word_node* second_list) {
    size_t ctr = 0;
    word_node* first_iter;
    word_node* second_iter;
    word_node* two_grams = NULL;
    int temp_len_1, temp_len_2;
    for(first_iter=first_list; first_iter->next!=NULL; first_iter=first_iter->next) {
        for (second_iter=second_list; second_iter->next!=NULL; second_iter=second_iter->next) {
            //if a word's len is equal to the other word's len
            //also this word's node's next's len is equal to the other word's node's next node's len
            if(first_iter->len == second_iter->len && first_iter->next->len == second_iter->next->len) {
                temp_len_1 = first_iter->len;
                temp_len_2 = first_iter->next->len;
                //check that these word's are equivalent. if they are equavalent, insert them to the two-grams linked list
                if((!strncmp(first_iter->word, second_iter->word, temp_len_1)) && (!strncmp(first_iter->next->word, second_iter->next->word, temp_len_2))) {
                    two_grams = insert_two_grams(two_grams, first_iter->word, first_iter->next->word, temp_len_1, temp_len_2, &ctr);
                    break;
                }
            }
        }
    }
    //sort words and then print them
    two_grams = sort_words(two_grams);
    print_two_grams(two_grams, ctr);
}

word_node* insert_two_grams(word_node* two_grams, char* first, char* second, int first_len, int second_len, size_t* ctr) {
    //+1 for one white space, +1 for NULL to the end
    int total_len = first_len + second_len + 2;
    //allocate memory for the last word
    char* final_s = (char*)malloc(total_len);
    int i, j, k = 0;
    //fill the two-gram word with given two words' every chars.
    for(i=0; i<first_len; i++)
        //insert first word
        final_s[k++] = first[i];
    //then insert a space
    final_s[k++] = ' ';
    for(j=0; j<second_len; j++)
        //and then inser the second word
        final_s[k++] = second[j];
    //put a NULL to the end
    final_s[k] = '\0';
    //check if the two-gram words inserted before or not
    if(two_grams_exists(two_grams, final_s) == EXISTS)
        //if it exists, return it as the same before
        return two_grams;
    //if it is not inserted before, this means that we have a two-gram word
    //because of that, increment our counter
    (*ctr)++;
    //if linked list is NULL, allocate some memory, and set it's word to current two-gram words
    if(two_grams == NULL) {
        two_grams = (word_node *) malloc(sizeof(word_node));
        two_grams->word = final_s;
        two_grams->next = NULL;
        two_grams->len = total_len - 1;
        //return list
        return two_grams;
    }
    //if list is not empty, insert two-gram word to the end of the list
    word_node* iter = two_grams;
    while(iter->next != NULL)
        iter = iter->next;
    iter->next = (word_node*)malloc(sizeof(word_node));
    iter = iter->next;
    iter->word = final_s;
    iter->next = NULL;
    iter->len = total_len - 1;
    //then return list
    return two_grams;
}

int two_grams_exists(word_node* two_grams, char* word) {
    //wanders around every node of the two-grams linked list and checks the existence of given "word" parameter.
    //if it exists, return an integer in order to show it's existence
    while(two_grams != NULL) {
        if(!strcmp(word, two_grams->word))
            return EXISTS;
        two_grams = two_grams->next;
    }
    return DOESNT_EXIST;
}

void print_two_grams(word_node* two_grams, size_t ctr) {
    //prints the number of two-gram words
    printf("Number of 2-grams: %zu\n", ctr);
    printf("2-grams: \n");
    //wanders around every node of the list and prints their words
    while(two_grams != NULL) {
        printf("%s\n", two_grams->word);
        two_grams = two_grams->next;
    }
}

word_node* sort_words(word_node* wn) {
    word_node* first_iter,* second_iter;
    //i used selection sort
    for(first_iter=wn; first_iter->next!=NULL; first_iter=first_iter->next) {
        for(second_iter=first_iter->next; second_iter!=NULL; second_iter=second_iter->next) {
            //if a word has to exist before of the other word but it does not exist before
            if(strcmp(first_iter->word, second_iter->word) > 0) {
                //these codes swap them
                char* temp_word = first_iter->word;
                char* temp_len = first_iter->len;
                first_iter->word = second_iter->word;
                second_iter->word = temp_word;
                first_iter->len = second_iter->len;
                second_iter->len = temp_len;
            }
        }
    }
    //and then return sorted linked list
    return wn;
}