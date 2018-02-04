/*
 * Mert KELKİT
 * 150115013
 * CSE 225 Project#2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//input files
#define INPUT_1 "Input1.txt"
#define INPUT_2 "Input2.txt"

//first sentence lengths for input files
#define INPUT_1_STC 19
#define INPUT_2_STC 18

//in order to calculate execution time
clock_t begin;
clock_t end;

//node struct for binary search tree
typedef struct bst_node {
    char* data;
    struct bst_node* left;
    struct bst_node* right;
} bst_node;

//node struct for avl tree
typedef struct avl_node {
    //it also has a height field to calculate balance
    int height;
    char* data;
    struct avl_node* left;
    struct avl_node* right;
} avl_node;

/* Function Prototypes */
void read_for_bst(char*, bst_node**, int);
avl_node* read_for_avl(char*, avl_node*, int);
void insert_bst(bst_node**, char*);
avl_node* insert_avl(avl_node*, char*);
bst_node* delete_from_bst(bst_node*, char*);
avl_node* delete_from_avl(avl_node*, char*);
bst_node* max_bst(bst_node*);
bst_node* min_bst(bst_node*);
avl_node* max_avl(avl_node*);
avl_node* min_avl(avl_node*);
void print_lnr_bst(bst_node*);
void print_lnr_avl(avl_node*);
int get_height(avl_node*);
int max(int, int);
int get_balance(avl_node*);
avl_node* right_rotate(avl_node*);
avl_node* left_rotate(avl_node*);
int get_node_count_bst(bst_node*);
int get_node_count_avl(avl_node*);

int main() {
    printf("-------------------- Welcome to the BST - AVL Comparator -------------------\n\n");
    bst_node* first_root_bst = NULL;
    bst_node* second_root_bst = NULL;
    avl_node* first_root_avl = NULL;
    avl_node* second_root_avl = NULL;
    read_for_bst(INPUT_1, &first_root_bst, INPUT_1_STC);
    read_for_bst(INPUT_2, &second_root_bst, INPUT_2_STC);
    first_root_avl = read_for_avl(INPUT_1, first_root_avl, INPUT_1_STC);
    second_root_avl = read_for_avl(INPUT_2, second_root_avl, INPUT_2_STC);
    return 0;
}

void read_for_bst(char* file_name, bst_node** root, int mode) {
    FILE* file;
    char* temp = NULL;
    char temp_char;
    int ctr = 0;
    //opens selected text file
    if(file = fopen(file_name, "r")) {
        //get start time for execution time
        begin = clock();
        while(!feof(file)) {
            //read a character until find space, dot or end of file
            temp_char = fgetc(file);
            if(temp_char == '.' || temp_char == EOF || temp_char == ' ') {
                //if you find at least one of them, insert the scanned word to the bst
                insert_bst(root, temp);
                temp = NULL;
                ctr = 0;
                continue;
            }
            if(temp == NULL) {
                //first getchar
                temp = (char*)malloc(1);
                temp[ctr++] = temp_char;
            }
            else {
                temp = realloc(temp, 1);
                temp[ctr++] = temp_char;
            }
            temp = realloc(temp, 1);
            temp[ctr] = '\0';
        }
        end = clock();
        //get end time for execution time and calculate it
        if(!strcmp(file_name, INPUT_1))
            printf("Execution time for the BST insertion with the first input file: %lf\n", (double)(end-begin)/CLOCKS_PER_SEC);
        else
            printf("Execution time for the BST insertion with the second input file: %lf\n", (double)(end-begin)/CLOCKS_PER_SEC);
        printf("Memory usage of this tree after insertion: %.2lf KB\n\n", (double)get_node_count_bst(*root) * sizeof(bst_node) / 1024);
        //take the cursor to the beginning of the file in order to delete the first sentence
        rewind(file);
        int i = 0;
        ctr = 0;
        temp = NULL;
        //get starting time of deletion
        begin = clock();
        while(ctr < mode) {
            temp_char = fgetc(file);
            //if you scan a word...
            if (temp_char == '.' || temp_char == EOF || temp_char == ' ') {
                bst_node* temp_node = *root;
                //remove this word
                temp_node = delete_from_bst(temp_node, temp);
                *root = temp_node;
                i = 0;
                ctr++;
                continue;
            }
            if (temp == NULL) {
                temp = (char *) malloc(1);
                temp[i++] = temp_char;
            } else {
                temp = realloc(temp, 1);
                temp[i++] = temp_char;
            }
            temp = realloc(temp, 1);
            temp[i] = '\0';
        }
        //get the ending time to calculate execution time
        end = clock();
        fclose(file);
        if(!strcmp(file_name, INPUT_1))
            printf("Execution time for the BST deletion with the first input file: %lf\n", (double)(end-begin)/CLOCKS_PER_SEC);
        else
            printf("Execution time for the BST deletion with the second input file: %lf\n", (double)(end-begin)/CLOCKS_PER_SEC);
        printf("Memory usage of this tree after deletion: %.2lf KB\n", (double)get_node_count_bst(*root) * sizeof(bst_node) / 1024);
        printf("----------------------------------------------------------------------------\n");
    }
    else
        printf("File cannot found.");
}

avl_node* read_for_avl(char* file_name, avl_node* root, int mode) {
    //reading text file for avl tree
    FILE* file;
    char* temp = NULL;
    char temp_char;
    int ctr = 0;
    if(file = fopen(file_name, "r")) {
        //get starting time
        begin = clock();
        while (!feof(file)) {
            temp_char = fgetc(file);
            if (temp_char == '.' || temp_char == EOF || temp_char == ' ') {
                //insert word
                root = insert_avl(root, temp);
                temp = NULL;
                ctr = 0;
                continue;
            }
            if (temp == NULL) {
                temp = (char *) malloc(1);
                temp[ctr++] = temp_char;
            } else {
                temp = realloc(temp, 1);
                temp[ctr++] = temp_char;
            }
            temp = realloc(temp, 1);
            temp[ctr] = '\0';
        }
        end = clock();
        //calculate execution time
        if(!strcmp(file_name, INPUT_1))
            printf("Execution time for the AVL insertion with the first input file: %lf\n", (double)(end-begin)/CLOCKS_PER_SEC);
        else
            printf("Execution time for the AVL insertion with the second input file: %lf\n", (double)(end-begin)/CLOCKS_PER_SEC);
        printf("Memory usage of this tree after insertion: %.2lf KB\n\n", (double)get_node_count_avl(root) * sizeof(avl_node) / 1024);
        rewind(file);
        int i = 0;
        ctr = 0;
        temp = NULL;
        begin = clock();
        //read the file again and delete the first sentence from the tree
        while(ctr < mode) {
            temp_char = fgetc(file);
            if (temp_char == '.' || temp_char == EOF || temp_char == ' ') {
                //delete the word
                root = delete_from_avl(root, temp);
                temp = NULL;
                i = 0;
                ctr++;
                continue;
            }
            if (temp == NULL) {
                temp = (char *) malloc(1);
                temp[i++] = temp_char;
            } else {
                temp = realloc(temp, 1);
                temp[i++] = temp_char;
            }
            temp = realloc(temp, 1);
            temp[i] = '\0';
        }
        end = clock();
        //calculate execution time
        if(!strcmp(file_name, INPUT_1))
            printf("Execution time for the AVL deletion with the first input file: %lf\n", (double)(end-begin)/CLOCKS_PER_SEC);
        else
            printf("Execution time for the AVL deletion with the second input file: %lf\n", (double)(end-begin)/CLOCKS_PER_SEC);
        printf("Memory usage of this tree after deletion: %.2lf KB\n", (double)get_node_count_avl(root) * sizeof(avl_node) / 1024);
        printf("----------------------------------------------------------------------------\n");
        fclose(file);
    }
    return root;
}

void insert_bst(bst_node** root, char* data) {
    bst_node* new_node = (bst_node*)malloc(sizeof(bst_node));
    bst_node* current = *root;
    //if tree(or subtree) is empty, insert directly
    if(*root == NULL) {
        *root = new_node;
        new_node->data = data;
        new_node->right = NULL;
        new_node->left = NULL;
        return;
    }
    //if the data comes before the node's data in dictionary, go left on the tree
    if(strcmp(current->data, data) > 0)
        insert_bst(&(current->left), data);
    //if the data comes after the node's data in dictionary, go left on the tree
    else if(strcmp(current->data, data) < 0)
        insert_bst(&(current->right), data);
}

avl_node* insert_avl(avl_node* root, char* data) {
    avl_node* new_node = (avl_node*)malloc(sizeof(avl_node));
    //if the tree(maybe subtree of main tree) is empty, insert directly
    if(root == NULL) {
        new_node->data = data;
        new_node->right = NULL;
        new_node->left = NULL;
        //initially every nodes inserted as leaves. so their height is 1 initially
        new_node->height = 1;
        return new_node;
    }
    //if the data comes before the node's data in dictionary, go left on the tree
    if(strcmp(root->data, data) > 0)
        root->left = insert_avl(root->left, data);
    //if the data comes after the node's data in dictionary, go left on the tree
    else if(strcmp(root->data, data) < 0)
        root->right = insert_avl(root->right, data);
    //if same data is found, do not insert
    else
        return root;
    //calculate and set the height of the root
    //height of the root is the longest path to the root from leave(s)
    root->height = max(get_height(root->right), get_height(root->left)) + 1;
    //get the height difference of the current node(leftSubtree.height - rightSubtree.height, might be negative)
    int difference = get_balance(root);
    // R/R case
    if(difference < -1 && strcmp(data, root->right->data) > 0) {
        return left_rotate(root);
    }
    // L/L case
    if(difference > 1 && strcmp(data, root->left->data) < 0) {
        return right_rotate(root);
    }
    //right subtree of the left child case
    if(difference > 1 && strcmp(data, root->left->data) > 0) {
        //double rotation for R/L case
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }
    //left subtree of the right child case
    if(difference < -1 && strcmp(data, root->right->data) < 0) {
        //double rotation for L/R case
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }
    return root;
}

bst_node* delete_from_bst(bst_node* root, char* data) {
    if(root == NULL)
        return NULL;
    //search the word according to its recursive comparison(go left or right until finding the same data)
    if(strcmp(data, root->data) < 0) {
        root->left = delete_from_bst(root->left, data);
    }
    else if(strcmp(data, root->data) > 0) {
        root->right = delete_from_bst(root->right, data);
    }
    //if the same data is found, it will be removed right now
    else {
        if(root->left == NULL) {
            //if there is no node in the left, right node will be the parent
            bst_node* temp = root->right;
            free(root);
            return temp;
        }
        else if(root->right == NULL) {
            //if there is no node in the right, left node will be the parent
            bst_node* temp = root->left;
            free(root);
            return temp;
        }
        //if they are full, get the minimum valued node of the right subtree.
        bst_node* temp = min_bst(root->right);
        //and make it the root
        root->data = temp->data;
        //also change the root of every subtree, because we are carrying the main root one step for each function call
        //so there may occur a node duplication
        root->right = delete_from_bst(root->right, temp->data);
    }
    return root;
}


avl_node* delete_from_avl(avl_node* root, char* data) {
    if(root == NULL) {
        return root;
    }
    //travel around the tree according to the comparisons
    if(strcmp(data, root->data) > 0)
        root->right = delete_from_avl(root->right, data);
    else if(strcmp(data, root->data) < 0)
        root->left = delete_from_avl(root->left, data);
    //if the node which wanted to be deleted is found
    else {
        //if there is at least one NULL left or right child
        if(root->left == NULL || root->right == NULL) {
            //find which one is NULL
            avl_node* temp = root->left ? root->left : root->right;
            //then set the new root according to it
            if(temp == NULL) {
                temp = root;
                root = NULL;
            }
            else
                //copy the fields of temp into root
                *root = *temp;
            free(temp);
        }
        //if there is no NULL child of the current node
        else {
            //get the minimum valued node of the right child of the root
            avl_node* temp = min_avl(root->right);
            //copy the fields to the root
            root->data = temp->data;
            //and carry the right subtree upper
            root->right = delete_from_avl(root->right, temp->data);
        }
    }
    if(root == NULL) {
        return root;
    }
    //update the height of the root
    root->height = max(get_height(root->left), get_height(root->right)) + 1;
    //get the height difference
    int difference = get_balance(root);
    //LL case
    if(difference > 1 && get_balance(root->left) >= 0)
        return right_rotate(root);
    //RR case
    if(difference < -1 && get_balance(root->right) <= 0)
        return left_rotate(root);
    // left subtree of the right child
    if(difference < -1 && get_balance(root->right) > 0) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }
    // right subtree of the left child
    if(difference > 1 && get_balance(root->left) < 0) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }
    return root;
}

bst_node* max_bst(bst_node* root) {
    while(root->right != NULL)
        root = root->right;
    return root;
}

bst_node* min_bst(bst_node* root) {
    while(root->left != NULL)
        root = root->left;
    return root;
}

avl_node* max_avl(avl_node* root) {
    while(root->right != NULL)
        root = root->right;
    return root;
}

avl_node* min_avl(avl_node* root) {
    while(root->left != NULL)
        root = root->left;
    return root;
}

void print_lnr_bst(bst_node* root) {
    if(root != NULL) {
        print_lnr_bst(root->left);
        printf("%s ", root->data);
        print_lnr_bst(root->right);
    }
}

void print_lnr_avl(avl_node* root) {
    if(root != NULL) {
        print_lnr_avl(root->left);
        printf("%s ", root->data);
        print_lnr_avl(root->right);
    }
}

int get_height(avl_node* node) {
    if(!node)
        return 0;
    return node->height;
}

int max(int x, int y) {
    return (x > y) ? x : y;
}

int get_balance(avl_node* root) {
    //calculate height differences in the avl tree
    if(root == NULL)
        return 0;
    int balance = get_height(root->left) - get_height(root->right);
    return balance;
}

avl_node* right_rotate(avl_node* root) {
    //point the alpha node
    avl_node* alpha = root;
    avl_node* l = alpha->left;
    avl_node* sub = l->right;

    //let the alpha node to be right child of the left of the alpha node
    l->right = alpha;
    //and then put the right subtree of the alpha node's left child alpha's left
    //because this subtree's values are smaller than the alpha node's value
    alpha->left = sub;

    //update their heights
    alpha->height = max(get_height(alpha->left), get_height(alpha->right)) + 1;
    l->height = max(get_height(l->left), get_height(l->right)) + 1;
    return l;
}

avl_node* left_rotate(avl_node* root) {
    //point the alpha node
    avl_node* alpha = root;
    avl_node* r = alpha->right;
    avl_node* sub = r->left;

    //let the alpha node to be left child of the right of the alpha node
    r->left = alpha;
    //and then put the left subtree of the alpha node's right child alpha's right
    //because this subtree's values are larger than the alpha node's value
    alpha->right = sub;

    //update the heights
    alpha->height = max(get_height(alpha->left), get_height(alpha->right)) + 1;
    r->height = max(get_height(r->left), get_height(r->right)) + 1;
    return r;
}

int get_node_count_bst(bst_node* root) {
    //the node itself must be counted
    int count = 1;
    if(root == NULL)
        return 0;
    else {
        //count one by one each node
        count += get_node_count_bst(root->left);
        count += get_node_count_bst(root->right);
        return count;
    }
}

int get_node_count_avl(avl_node* root) {
    //the node itself must be counted
    int count = 1;
    if(root == NULL)
        return 0;
    else {
        //count one by one each node
        count += get_node_count_avl(root->left);
        count += get_node_count_avl(root->right);
        return count;
    }
}