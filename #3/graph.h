/*
 * Mert KELKİT
 * 150115013
 * Project #3
 */

#ifndef GRAPH_H
#define GRAPH_H

//--------------------//--------------------//--------------------//--------------------//--------------------//--------

#include <stdio.h>

//--------------------//--------------------//--------------------//--------------------//--------------------//--------

typedef struct word_node word_node;
typedef struct vertex_node vertex_node;
typedef struct line_by_line line_by_line;
typedef struct graph_t graph_t;
typedef struct stack stack;

//--------------------//--------------------//--------------------//--------------------//--------------------//--------

graph_t* get_graph(char* file_path);
void print_adjacency_list(graph_t* g);

//--------------------//--------------------//--------------------//--------------------//--------------------//--------

word_node* get_vertex_names(word_node* root, char* file_path);
word_node* split_courses(word_node* root, char* courses, char delimeter);
word_node* insert_word(word_node* root, char* course);
word_node* sort_list(word_node* root, int low, int high);
int partition (word_node** root_ref, int low, int high);
char* get_word(word_node* root, int idx);
word_node* get_node(word_node** root_ref, int idx);
int get_len(word_node* root);
void swap(word_node** node_1, word_node** node_2);
int get_line_count(void);

//--------------------//--------------------//--------------------//--------------------//--------------------//--------

vertex_node* word_to_vertex(word_node* root);
vertex_node* insert_vertex(vertex_node* root, char* vertex_name);
void print_vertex(vertex_node* root);

//--------------------//--------------------//--------------------//--------------------//--------------------//--------

line_by_line** get_line_by_line(char* file_path);
line_by_line* split_courses_line(line_by_line* root, char* courses, char delimeter);
line_by_line* insert_line(line_by_line* root, char* course);
vertex_node** connect_vertices(line_by_line** line_info, vertex_node* vertex_list);
int get_vertex_count(vertex_node* root);
char* get_vertex_name(vertex_node* root, int index);
vertex_node* sort_vertex_list(vertex_node* root);

//--------------------//--------------------//--------------------//--------------------//--------------------//--------

/*Stack functions*/

void push(stack** s, int index);
int pop(stack** s);
int peek(stack* s);

//--------------------//--------------------//--------------------//--------------------//--------------------//--------

/* Graph Coloring functions*/

void print_exam_schedule(graph_t* g);
int* get_adjacent_periods(vertex_node** adjacency_list, vertex_node* vertex_list, int index);
int get_available_period(int* periods);
int* selection_sort(int* periods);
void print_period(vertex_node** adjacency_list, vertex_node* vertex_list, int period);
int get_lim_period(vertex_node** adjacency_list, vertex_node* vertex_list);

#endif
