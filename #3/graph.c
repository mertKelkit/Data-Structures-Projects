/*
 * Mert KELKİT
 * 150115013
 * Project #3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "graph.h"

//linked list node for reading text
struct word_node {
    char* word;
    word_node* next;
};

//linked list node in order to record vertices
struct vertex_node {
    int period;
    char* vertex_name;
    vertex_node* next;
};

//linked list node in order to hold each line's info
struct line_by_line {
    char* course;
    line_by_line* next;
};

//graph struct with adjacency and vertex lists
struct graph_t {
    vertex_node** adjacency_list;
    vertex_node* vertex_list;
};

//stack implementation
struct stack {
    int index;
    stack* next;
};

int line_count = 0;

//builds the graph
graph_t* get_graph(char* file_path) {
    graph_t* g = (graph_t*)malloc(sizeof(graph_t));

    word_node* root = NULL;
    root = get_vertex_names(root, file_path);

    vertex_node* vertex_root = NULL;
    vertex_root = word_to_vertex(root);

    line_by_line** lines = get_line_by_line(file_path);
    vertex_node** adjacency_list = connect_vertices(lines, vertex_root);

    g->adjacency_list = adjacency_list;
    g->vertex_list = vertex_root;
    return g;
}

void print_adjacency_list(graph_t* g) {
    int i;
    vertex_node** adjacency_list = g->adjacency_list;
    int vertex_count = get_vertex_count(g->vertex_list);
    for(i=0; i<vertex_count; i++) {
        vertex_node* current = *(adjacency_list + i);
        print_vertex(current);
        printf("\n");
    }
}

//gets names of vertices
word_node* get_vertex_names(word_node* root, char* file_path) {
    FILE* file;
    char temp;
    char* temp_courses = (char*)malloc(60);
    if(file = fopen(file_path, "r")) {
        while(!feof(file)) {
            do {
                temp = fgetc(file);
            } while (temp != ':');
            fscanf(file, "%s", temp_courses);
            root = split_courses(root, temp_courses, ',');
            line_count++;
        }
        fclose(file);
    }
    else {
        printf("text file cannot be opened - filehandler.c -\n");
        exit(0);
    }
    //then sorts the list with the quick sort algorithm
    root = sort_list(root, 0, get_len(root)-1);
    return root;
}

//string splitting
word_node* split_courses(word_node* root, char* courses, char delimeter) {
    char* temp;
    size_t word_idx = 0;
    int i;
    size_t total_len = strlen(courses);
    for(i=0; i<total_len; i++) {
        if(*(courses + i) != delimeter) {
            if(word_idx == 0)
                temp = (char*)malloc(1);
            else
                temp = (char*)realloc(temp, 1);
            *(temp + word_idx++) = *(courses + i);
        }
        if(*(courses + i) == delimeter || i == total_len - 1) {
            temp = (char*)realloc(temp, 1);
            *(temp + word_idx) = '\0';
            word_idx = 0;
            root = insert_word(root, temp);
        }
    }
    return root;
}

word_node* insert_word(word_node* root, char* course) {
    word_node* controller = root;
    while(controller != NULL) {
        if (!strcmp(course, controller->word))
            return root;
        controller = controller->next;
    }
    if(root == NULL) {
        root = (word_node*)malloc(sizeof(word_node));
        root->word = course;
        root->next = NULL;
        return root;
    }
    word_node* new_node = (word_node*)malloc(sizeof(word_node));
    new_node->word = course;
    new_node->next = NULL;
    word_node* iter = root;
    while(iter->next != NULL)
        iter = iter->next;
    iter->next = new_node;
    return root;
}

/* QUICK SORT */

word_node* sort_list(word_node* root, int low, int high) {
    if (low < high) {
        int pi = partition(&root, low, high);
        root = sort_list(root, low, pi - 1);
        root = sort_list(root, pi + 1, high);
    }
    return root;
}

int partition(word_node** root_ref, int low, int high) {
    word_node* root = *root_ref;
    char* pivot = get_word(root, high);    // pivot
    int i = (low - 1);  // Index of smaller element
    int j;
    for (j=low; j<high; j++) {
        // If current element is smaller than or
        // equal to pivot
        if (strcmp(get_word(root, j), pivot) <= 0) {
            i++;    // increment index of smaller element
            word_node* first = get_node(&root, i);
            word_node* second = get_node(&root, j);
            swap(&first, &second);
        }
    }
    word_node* first = get_node(&root, i+1);
    word_node* second = get_node(&root, high);
    swap(&first, &second);
    return (i + 1);
}

/* QUICK SORT END */

char* get_word(word_node* root, int idx) {
    int i;
    for(i=0; i<idx; i++) {
        root = root->next;
    }
    return root->word;
}

//returns the node itself
word_node* get_node(word_node** root_ref, int idx) {
    word_node* root = *root_ref;
    int i;
    for(i=0; i<idx; i++) {
        root = root->next;
    }
    return root;
}

//returns length of the linked list
int get_len(word_node* root) {
    int ctr = 0;
    while(root != NULL) {
        root = root->next;
        ctr++;
    }
    return ctr;
}

void swap(word_node** node_1, word_node** node_2) {
    word_node* first = *node_1;
    word_node* second = *node_2;
    char* temp = first->word;
    first->word = second->word;
    second->word = temp;
}

int get_line_count() {
    return line_count;
}

//creates vertices from given vertex names
vertex_node* word_to_vertex(word_node* root) {
    vertex_node* vertex_root = NULL;
    int i;
    for(i=0; i<get_len(root); i++)
        vertex_root = insert_vertex(vertex_root, get_word(root, i));
    return vertex_root;
}

//vertex list insertion
vertex_node* insert_vertex(vertex_node* root, char* vertex_name) {
    vertex_node* controller = root;
    while(controller != NULL) {
        if (!strcmp(vertex_name, controller->vertex_name))
            return root;
        controller = controller->next;
    }
    if(root == NULL) {
        root = (vertex_node*)malloc(sizeof(vertex_node));
        root->vertex_name = vertex_name;
        root->next = NULL;
        root->period = 0;
        return root;
    }
    vertex_node* iter = root;
    while(iter->next != NULL)
        iter = iter->next;
    vertex_node* new_node = (vertex_node*)malloc(sizeof(vertex_node));
    new_node->period = 0;
    new_node->vertex_name = vertex_name;
    iter->next = new_node;
    new_node->next = NULL;
    return root;
}

void print_vertex(vertex_node* root) {
    while(root != NULL) {
        if(root->next != NULL)
            printf("%s -> ", root->vertex_name);
        else
            printf("%s", root->vertex_name);
        root = root->next;
    }
}

//gets line by line info
line_by_line** get_line_by_line(char* file_path) {
    line_by_line** line_by_line_info;
    line_by_line_info = (line_by_line**)malloc(sizeof(line_by_line*) * get_line_count());
    FILE* file;
    char temp;
    char* temp_courses = (char*)malloc(60);
    int i = 0;
    if(file = fopen(file_path, "r")) {
        while(!feof(file)) {
            do {
                temp = fgetc(file);
            } while (temp != ':');
            fscanf(file, "%s", temp_courses);
            *(line_by_line_info + i) = split_courses_line(*(line_by_line_info + i), temp_courses, ',');
            i++;
        }
        fclose(file);
    }
    else {
        printf("text file cannot be opened - filehandler.c -\n");
        exit(0);
    }
    return line_by_line_info;
}

//splits courses in order to determine adjacents of vertices
line_by_line* split_courses_line(line_by_line* root, char* courses, char delimeter) {
    char* temp;
    size_t word_idx = 0;
    int i;
    size_t total_len = strlen(courses);
    for(i=0; i<total_len; i++) {
        if(*(courses + i) != delimeter) {
            if(word_idx == 0)
                temp = (char*)malloc(1);
            else
                temp = (char*)realloc(temp, 1);
            *(temp + word_idx++) = *(courses + i);
        }
        if(*(courses + i) == delimeter || i == total_len - 1) {
            temp = (char*)realloc(temp, 1);
            *(temp + word_idx) = '\0';
            word_idx = 0;
            root = insert_line(root, temp);
        }
    }
    return root;
}

//linked list insertion function
line_by_line* insert_line(line_by_line* root, char* course) {
    if(root == NULL) {
        root = (line_by_line*)malloc(sizeof(line_by_line));
        root->course = course;
        root->next = NULL;
        return root;
    }
    line_by_line* new_node = (line_by_line*)malloc(sizeof(line_by_line));
    new_node->course = course;
    new_node->next = NULL;
    line_by_line* iter = root;
    while(iter->next != NULL)
        iter = iter->next;
    iter->next = new_node;
    return root;
}

//connecting vertices by adjacency list
vertex_node** connect_vertices(line_by_line** line_info, vertex_node* vertex_list) {
    int vertex_count = get_vertex_count(vertex_list);
    vertex_node** adjacency_list = (vertex_node**)malloc(sizeof(vertex_node*) * vertex_count);
    int i, j;
    for(i=0; i<vertex_count; i++) {
        for(j=0; j<line_count; j++) {
            line_by_line* iter = line_info[j];
            line_by_line* iter_adder = line_info[j];
            while(iter != NULL) {
                if(!strcmp(get_vertex_name(vertex_list, i), iter->course)) {
                    adjacency_list[i] = insert_vertex(adjacency_list[i], iter->course);
                    while(iter_adder != NULL) {
                        adjacency_list[i] = insert_vertex(adjacency_list[i], iter_adder->course);
                        iter_adder = iter_adder->next;
                    }
                    break;
                }
                iter = iter->next;
            }
        }
        //sorts each adjacent of a vertex
        adjacency_list[i]->next = sort_vertex_list(adjacency_list[i]->next);
    }
    return adjacency_list;
}

//returns vertex number
int get_vertex_count(vertex_node* root) {
    int ctr = 0;
    while(root != NULL) {
        ctr++;
        root = root->next;
    }
    return ctr;
}

//returns vertex name of a given index
char* get_vertex_name(vertex_node* root, int index) {
    int i;
    for(i=0; i<index; i++)
        root = root->next;
    return root->vertex_name;
}

//returns the index of given vertex name
int get_vertex_index(vertex_node* root, char* vertex_name) {
    int i = 0;
    int lim = get_vertex_count(root);
    for(i=0; i<lim; i++) {
        if(!strcmp(root->vertex_name, vertex_name))
            return i;
        root = root->next;
    }
    return -1;
}

//selection sort
vertex_node* sort_vertex_list(vertex_node* root) {
    vertex_node* i,* j;
    for(i=root; i->next!=NULL; i=i->next) {
        for(j=i->next; j!=NULL; j=j->next) {
            if(strcmp(i->vertex_name, j->vertex_name) > 0) {
                char* temp = i->vertex_name;
                i->vertex_name = j->vertex_name;
                j->vertex_name = temp;
            }
        }
    }
    return root;
}

/* STACK FUNCTIONS */
void push(stack** s, int index) {
    stack* st = *s;
    if(st == NULL) {
        st = (stack*)malloc(sizeof(stack));
        st->index = index;
        st->next = NULL;
    }
    else {
        stack *new = (stack*)malloc(sizeof(stack));
        new->index = index;
        new->next = st;
        st = new;
    }
    *s = st;
}

int pop(stack** s) {
    stack *st = *s;
    if(st != NULL) {
        stack *temp = st;
        int ret_val = st->index;
        st = st->next;
        temp->next = NULL;
        free(temp);
        *s = st;
        return ret_val;
    }
    else
        return -1;
}

int peek(stack* s) {
    if(s != NULL)
        return s->index;
    else
        return -1;
}

/* STACK FUNCTION END */

/*
 * This function gives exam periods of the vertices
 * (that means coloring the graph)
 */
void print_exam_schedule(graph_t* g) {
    int* adjacent_periods;
    vertex_node* vertex_list = g->vertex_list;
    vertex_node** adjacency_list = g->adjacency_list;
    int size = get_vertex_count(vertex_list);
    bool done[size];
    bool visited[size];
    int ctr;
    for(ctr=0; ctr<size; ctr++) {
        done[ctr] = false;
        visited[ctr] = false;
    }
    //period starts with 1
    int period = 1;
    int i = 0;
    stack* s = NULL;
    vertex_node* current = NULL;
    bool control = false;
    ctr = 0;
    while(!done[i]) {
        control = false;
        current = adjacency_list[i];
        push(&s, i);
        visited[i] = true;
        while(current->next != NULL) {
            current = current->next;
            if(!visited[get_vertex_index(vertex_list, current->vertex_name)]) {
                i = get_vertex_index(vertex_list, current->vertex_name);
                control = true;
                break;
            }
        }
        if(control) {
            continue;
        }
        int idx = pop(&s);
        done[idx] = true;

        adjacent_periods = get_adjacent_periods(adjacency_list, vertex_list, idx);
        adjacency_list[i]->period = get_available_period(adjacent_periods);

        //if stack is not empty
        if(peek(s) != -1)
            i = pop(&s);
    }
    int lim = get_lim_period(adjacency_list, vertex_list);
    for(i=0; i<lim; i++)
        print_period(adjacency_list, vertex_list, i+1);
}

//returns the exam periods of the adjacents of a vertex, 0 by default
int* get_adjacent_periods(vertex_node** adjacency_list, vertex_node* vertex_list, int index) {
    int* periods = NULL;
    vertex_node* current = adjacency_list[index];
    int ctr = 0;
    while(current->next != NULL) {
        current = current->next;
        if(ctr == 0)
            periods = (int*)malloc(sizeof(int));
        else
            periods = (int*)realloc(periods, sizeof(int));
        periods[ctr++] = adjacency_list[get_vertex_index(vertex_list, current->vertex_name)]->period;
    }
    periods = (int*)realloc(periods, sizeof(int));
    periods[ctr] = -1;
    return periods;
}

//returns an available exam period, created by a given set of sorted periods

//our main purpose is to give possible smallest period

int get_available_period(int* periods) {
    int i;
    int period = 1;
    periods = selection_sort(periods);
    for(i=0; periods[i]!=-1; i++) {
        if(period == periods[i]) {
            period++;
        }
    }
    return period;
}

//seleciton sort
int* selection_sort(int* periods) {
    int i, j;
    for(i=0; periods[i+1] != -1; i++) {
        for(j=i+1; periods[j] != -1; j++) {
            if(periods[i] > periods[j]) {
                int temp = periods[i];
                periods[i] = periods[j];
                periods[j] = temp;
            }
        }
    }
    return periods;
}

//prints exam period
void print_period(vertex_node** adjacency_list, vertex_node* vertex_list, int period) {
    int i;
    int ctr = 1;
    int len = get_vertex_count(vertex_list);
    printf("\n-------------------\n| EXAM PERIOD %-4d|\n-------------------\n", period);
    for(i=0; i<len; i++) {
        if(adjacency_list[i]->period == period) {
            printf("| %d - %-12s|\n-------------------\n", ctr++, adjacency_list[i]->vertex_name);
        }
    }
}

//returns maximum period
int get_lim_period(vertex_node** adjacency_list, vertex_node* vertex_list) {
    int max = adjacency_list[0]->period;
    int i;
    int len = get_vertex_count(vertex_list);
    for(i=0; i<len; i++)
        if(adjacency_list[i]->period > max)
           max = adjacency_list[i]->period;
    return max;
}