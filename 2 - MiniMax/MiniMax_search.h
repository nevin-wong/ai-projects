/*
	CSC D84 - Unit 2 - MiniMax and adversarial games

	This file contains the API function headers for your assignment.
	Please pay close attention to the function prototypes, and
	understand what the arguments are.

	Stubs for implementing each function are to be found in MiniMax_search.c,
	along with clear ** TO DO markers to let you know where to add code.

	You are free to add helper functions within reason. But you must
	provide a prototype *in this file* as well as the implementation
	in the .c program file.

	Script by: F.J.E., Sep. 2015
*/

#ifndef __MiniMax_search_header

#define __MiniMax_search_header

// Generally needed includes
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#include<string.h>

#include "board_layout.h"

// Function prototypes for D84 - Unit 2 - MiniMax assignment solution
double MiniMax(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), int agentId, int depth, int maxDepth, double alpha, double beta);

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]);

int checkForTerminal(int mouse_loc[1][2],int cat_loc[10][2],int cheese_loc[10][2],int cats,int cheeses);
// If you need to add any function prototypes yourself, you can do so *below* this line.

int directionToNewIndex(int direction, int oldIndex);
struct PQNode;
struct PriorityQueue;
struct PriorityQueue* create_queue();
bool is_empty(struct PriorityQueue* pq);
int get_min(struct PriorityQueue* pq);
void remove_node(struct PriorityQueue* pq, int pos);
void push(struct PriorityQueue* pq, int node, int cost);
void update_cost(struct PriorityQueue* pq, int node, int new_cost);
bool is_in_queue(struct PriorityQueue* pq, int node);
void initialize_actual_cost(int actual_cost[]);
void initialize_predecessor(int predecessor[]);
void initialize_visited(bool visited[]);
int calculateCostPath(int curr_index, int predecessor[graph_size]);
bool index_is_in_array(int index, int arr[10][2], int amount);
int hCostManhattan(int x, int y, int mouse_loc[1][2]);
int shortestPathLengthAStarSearch(int x, int y, double gr[graph_size][4], int mouse_loc[1][2], int obstacle_loc[10][2], int obstacles);

#endif

