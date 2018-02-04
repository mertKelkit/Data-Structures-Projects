/*
 * Mert KELKİT
 * 150115013
 * Project #3
 */

#include "graph.h"

int main() {
    printf("\n>> Exam Scheduler v0.1.2\n");
    char* file_name = "students_info.txt";
    graph_t* g = get_graph(file_name);
    printf("\n\n>> Adjacency list representation of this graph:\n\n");
    print_adjacency_list(g);
    printf("\n\n>> Arranged exam schedule according to given graph with DFs-Coloring:\n\n");
    print_exam_schedule(g);
    return 0;
}