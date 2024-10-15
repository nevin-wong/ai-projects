/*
	CSC D84 - Unit 2 - MiniMax search and adversarial games

	This file contains stubs for implementing a MiniMax search
        procedure with alpha-beta pruning. Please read the assignment
	handout carefully - it describes the game, the data you will
	have to handle, and the search functions you must provide.

	Once you have read the handout carefully, implement your search
	code in the sections below marked with

	**************
	*** TO DO:
	**************

	Make sure to add it to your report.txt file - it will be marked!

	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Sep. 15
*/

#include "MiniMax_search.h"
#include <limits.h>
#include <cfloat>
#include <math.h>

double MiniMax(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), int agentId, int depth, int maxDepth, double alpha, double beta)
{
 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in MiniMax_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:

	Exactly the same as for Assignment 1 - have a look at your code if you need a reminder of how the adjacency
	list and agent positions are stored.	

	Note that in this case, the path will contain a single move - at the top level, this function will provide
	an agent with the 'optimal' mini-max move given the game state.


   IMPORTANT NOTE: Mini-max is a recursive procedure. This function will need to fill-in the mini-max values for 
	 	   all game states down to the maximum search depth specified by the user. In order to do that,
		   the function needs to be called with the correct state at each specific node in the mini-max
		   search tree.

		   The game state is composed of:

			* Mouse, cat, and cheese positions (and number of cats and cheeses)
			
		   At the top level (when this function is called by the mini-max driver code), the game state
		   correspond to the current situation of the game. But once you start recursively calling
		   this function for lower levels of the search tree the positions of agents will have changed.
		   
		   Therefore, you will need to define local variables to keep the game state at each node of the
		   mini-max search tree, and you will need to update this state when calling recursively so that
		   the search does the right thing.

		   This function *must check* whether:
			* A candidate move results in a terminal configuration (cat eats mouse, mouse eats cheese)
			  at which point it calls the utility function to get a value
	 		* Maximum search depth has been reached (depth==maxDepth), at which point it will also call
			  the utility function to get a value
			* Otherwise, call recursively using the candidate configuration to find out what happens
			  deeper into the mini-max tree.

   Arguments:
		gr[graph_size][4]   		- This is an adjacency list for the maze
		path[1][2] 			- Your function will return the optimal mini-max move in this array.
		minmax_cost[size_X][size_Y]	- An array in which your code will store the
						  minimax value for maze locations expanded by
						  the search *when called for the mouse, not
						  for the cats!*

						  This array will be used to provide a visual 
						  display of minimax values during the game.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- No alpha-beta pruning
					mode = 1	- Alpha-beta pruning

		(*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]);
				- This is a pointer to the utility function which returns a value for a specific game configuration

				   NOTE: Unlike the search assignment, this utility function also gets access to the graph so you can do any processing 					 
				   that requires knowledge of the maze for computing the utility values.

				  * How to call the utility function from within this function : *
					- Like any other function:
						u = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
						
		agentId: Identifies which agent we are doing MiniMax for. agentId=0 for the mouse, agentId in [1, cats] for cats. Notice that recursive calls
                         to this function should increase the agentId to reflect the fact that the next level down corresponds to the next agent! For a game
                         with two cats and a mouse, the agentIds for the recursion should look like 0, 1, 2, 0, 1, 2, ...
	
		depth: Current search depth - whether this is a MIN or a MAX node depends both on depth and agentId.
		
		maxDepth: maximum desired search depth - once reached, your code should somehow return
			  a minimax utility value for this location.

		alpha. beta: alpha and beta values passed from the parent node to constrain search at this
			     level.

   Return values:
		Your search code will directly update data passed-in as arguments:
		
		- Mini-Max value	: Notice this function returns a double precision number. This is
					  the minimax value at this level of the tree. It will be used 
					  as the recursion backtracks filling-in the mini-max values back
					  from the leaves to the root of the search tree. 

		- path[1][2]		: Your MiniMax function will return the location for the agent's 
					  next location (i.e. the optimal move for the agent). 
		- minmax_cost[size_X][size_Y] 	:  Your search code will update this array to contain the
						   minimax value for locations that were expanded during
						   the search. This must be done *only* for the mouse.

						   Values in this array will be in the range returned by
						   your utility function.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *
			
		That's that, now, implement your solution!
 */

 /********************************************************************************************************
 * 
 * TO DO:	Implement code to perform a MiniMax search. This will involve a limited-depth BFS-like
 *              expansion. Once nodes below return values, your function will propagate minimax utilities
 *		as per the minimax algorithm.
 *	
 *		Note that if alpha-beta pruning is specified, you must keep track of alphas and betas
 *		along the path.
 *
 *		You can use helper functions if it seems reasonable. Add them to the MiniMax_search.h
 *		file and explain in your code why they are needed and how they are used.
 *
 *		Recursion should appear somewhere.
 *
 *		MiniMax cost: If the agentId=0 (Mouse), then once you have a MiniMax value for a location
 *		in the maze, you must update minmax_cost[][] for that location.
 *
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/
	
	if (depth == maxDepth || checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses)) {
		return utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
	}
	
	int i;
	int value;
	double bestValue;
	int nextAgentId = (agentId + 1) % (cats + 1);
	int moveIndex;
	int nextMove[1][2];
		
	if (agentId == 0) { // Mouse - Max node
	
		bestValue = -DBL_MAX;
		
		int moved_mouse_loc[1][2];
		int mouseIndex = mouse_loc[0][0] + (mouse_loc[0][1]*size_X);
		
		for (i = 0; i < 4; i++) {
			moveIndex = directionToNewIndex(i, mouseIndex);
			
			if (gr[mouseIndex][i] == 1.0) { // If this move is a valid move
				moved_mouse_loc[0][0] = moveIndex % size_X; // x
				moved_mouse_loc[0][1] = moveIndex / size_Y; // y
								
				// Recursively evaluate this move
				value = MiniMax(gr, nextMove, minmax_cost, cat_loc, cats, cheese_loc, cheeses, moved_mouse_loc, mode, utility, nextAgentId, depth + 1, maxDepth, alpha, beta);
				
				// Update bestValue
				if (value > bestValue) {
					bestValue = value;
					path[0][0] = moved_mouse_loc[0][0];
					path[0][1] = moved_mouse_loc[0][1];
				}
				
				// Prune if alpha >= beta
				if (mode == 1) { 
					alpha = fmax(alpha, bestValue);
					
					if (alpha >= beta) {
						break;
					}
				}
			}
		}
		
		minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]] = bestValue;
		
	} else { // Cat - Min node
		bestValue = DBL_MAX;
		
		int moved_cat_loc[10][2];
		memcpy(moved_cat_loc, cat_loc, sizeof(moved_cat_loc));
		int catIndex = cat_loc[agentId - 1][0] + (cat_loc[agentId - 1][1]*size_X);
		for (i = 0; i < 4; i++) {
			moveIndex = directionToNewIndex(i, catIndex);
			
			if (gr[catIndex][i] == 1.0) {
				moved_cat_loc[agentId - 1][0] = moveIndex % size_X; // x
				moved_cat_loc[agentId - 1][1] = moveIndex / size_Y; // x
				
				// Recursively evaluate this move
				value = MiniMax(gr, nextMove, minmax_cost, moved_cat_loc, cats, cheese_loc, cheeses, mouse_loc, mode, utility, nextAgentId, depth + 1, maxDepth, alpha, beta);
				
				// Update bestValue
				if (value < bestValue) {
					bestValue = value;
				}
				
				// Prune if beta <= alpha
				if (mode == 1) { 
					beta = fmin(beta, bestValue);
					
					if (beta <= alpha) {
						break;
					}
				}
			}
		}
	}
	
	return bestValue;
}

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4])
{
 /*
	This function computes and returns the utility value for a given game configuration.
	As discussed in lecture, this should return a positive value for configurations that are 'good'
	for the mouse, and a negative value for locations that are 'bad' for the mouse.

	How to define 'good' and 'bad' is up to you. Note that you can write a utility function
	that favours your mouse or favours the cats, but that would be a bad idea... (why?)

	Input arguments:

		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		depth - current search depth
		gr - The graph's adjacency list for the maze

		These arguments are as described in A1. Do have a look at your solution!
 */
 
	double utility;
 
	double autoWin = 40000;
	double autoLose = -40000;
 
	// Weights
	double far_from_cat_wt = cats;
	double close_to_cheese_wt = 2.0;
	double depth_wt = 2.0; 
	
	// Consider cats 6 or less units away "close"
	int proximityThreshold = 6;
	int numCloseCats = 0;
	
	// Mouse and cheese same tile - mouse wins
	if (index_is_in_array(mouse_loc[0][0] + (mouse_loc[0][1]*size_X), cheese_loc, cheeses))
		return autoWin - depth_wt*depth;

	// Mouse and cat same tile - mouse loses
	if (index_is_in_array(mouse_loc[0][0] + (mouse_loc[0][1]*size_X), cat_loc, cats)) 
		return autoLose - depth_wt*depth;

	// Shortest path length to any cat
	// (want this to be large to be far from cats)
	int shortest = INT_MAX;
	for (int i = 0; i < cats; i++) {
		int path_length = shortestPathLengthAStarSearch(cat_loc[i][0], cat_loc[i][1], gr, mouse_loc, NULL, 0);
		if (path_length > 0 && path_length < shortest) {
			shortest = path_length;
		}
		if (path_length <= proximityThreshold) {
			numCloseCats++;
		}
	}
	int far_from_cat_feature = shortest;
	
	// Fight-or-flight!
	// If half or more the cats are close, don't worry about cheese,
	// prioritize getting the hell out of there
	if (numCloseCats >= cats / 2) {
		close_to_cheese_wt = 0.5;
	}

	// Shortest path length to any cheese
	// (want to be close to be small to be close to cheese, 
	// so we'll take the negative as the feature)
	shortest = INT_MAX;
	bool exists_path_to_cheese = false;
	for (int i = 0; i < cheeses; i++) {
		int path_length = shortestPathLengthAStarSearch(cheese_loc[i][0], cheese_loc[i][1], gr, mouse_loc, cat_loc, cats);
		if (path_length > 0 && path_length < shortest) {
			shortest = path_length;
			exists_path_to_cheese = true;
		}
	}

	// No path to cheese - mouse loses
	if (!exists_path_to_cheese) return autoLose - depth_wt*depth;

	int close_to_cheese_feature = -shortest;

	utility = far_from_cat_wt*far_from_cat_feature + close_to_cheese_wt*close_to_cheese_feature - depth_wt*depth;
	// Subtract depth (multiplied by a weight) to prioritize more immediate rewards/reach the cheese quicker

	return utility;
}

int directionToNewIndex(int direction, int oldIndex) {
	// top
	if (direction == 0) {
		return (oldIndex - size_X);
	// right
	} else if (direction == 1) {
		return (oldIndex + 1);
	// bottom
	} else if (direction == 2) { 
		return (oldIndex + size_X);
	// left
	} else { 
		return (oldIndex - 1);
	}
}

struct PQNode {
	int node;
	int cost;
};

struct PriorityQueue {
	struct PQNode nodes[graph_size];
	int size;
};

struct PriorityQueue* create_queue() {
	struct PriorityQueue* pq = (struct PriorityQueue*)malloc(sizeof(struct PriorityQueue));
	pq->size = 0;
	return pq;
}

bool is_empty(struct PriorityQueue* pq) {
	return (pq->size == 0);
}

int get_min(struct PriorityQueue* pq) {
	int min_pos = 0;
	for (int i = 1; i < pq->size; i++) {
		if (pq->nodes[i].cost < pq->nodes[min_pos].cost) {
			min_pos = i;
		}
	}
	return min_pos;
}

void remove_node(struct PriorityQueue* pq, int pos) {
	// Basically just replacing that node with the last node
	pq->nodes[pos] = pq->nodes[pq->size - 1];
	pq->size--;
}

void push(struct PriorityQueue* pq, int node, int cost) {
	pq->nodes[pq->size].node = node;
	pq->nodes[pq->size].cost = cost;
	pq->size++;
}

void update_cost(struct PriorityQueue* pq, int node, int new_cost) {
	for (int i = 0; i < pq->size; i++) {
		if (pq->nodes[i].node == node) {
			pq->nodes[i].cost = new_cost;
			return;
		}
	}
}

bool is_in_queue(struct PriorityQueue* pq, int node) {
	for (int i = 0; i < pq->size; i++) {
		if (pq->nodes[i].node == node) {
			return true;
		}
	}
	return false;
}

// Helper function to initialize actual_cost[] array
void initialize_actual_cost(int actual_cost[]) {
    for (int i = 0; i < graph_size; i++) {
        actual_cost[i] = INT_MAX;
    }
}

// Helper function to initialize predecessor[] array
void initialize_predecessor(int predecessor[]) {
    for (int i = 0; i < graph_size; i++) {
        predecessor[i] = -1;
    }
}

// Helper function to initialize visited[] array
void initialize_visited(bool visited[]) {
    for (int i = 0; i < graph_size; i++) {
        visited[i] = false;
    }
}

int calculateCostPath(int curr_index, int predecessor[graph_size]) {
	// Follow predecessors back to the to get the length of the path
	int curr = curr_index;
	int path_length = 0;
	while (predecessor[curr] != -1) {
		curr = predecessor[curr];
		path_length++;
	}
	return path_length;
}

bool index_is_in_array(int index, int arr[10][2], int amount)
{	
	// Used to check if an index's grid location is one of a cat/cheese
	int x = index % size_X;
	int y = index / size_Y;
	
	for (int i = 0; i < amount; i++) {
		if (x == arr[i][0] && y == arr[i][1]) {
			return true;
		}
	}
	return false;
}

// Heuristic function for A* search
int hCostManhattan(int x, int y, int mouse_loc[1][2]) {
	return (abs(mouse_loc[0][0] - x) + abs(mouse_loc[0][1] - y));
}

// Gets the distance to the given location, using A* search, returns -1 if there is no path (reworked A1/A2 code)
// obstacles - if we're finding path from mouse to cheese, we need to avoid cats
int shortestPathLengthAStarSearch(int x, int y, double gr[graph_size][4], int mouse_loc[1][2], int obstacle_loc[10][2], int obstacles) {
	struct PriorityQueue* pq = create_queue();
	
	int start_index = mouse_loc[0][0] + (mouse_loc[0][1]*size_X); // Initial mouse index
	
	int actual_cost[graph_size]; // Array of the actual cost of reaching each node from the mouse location [g(n)]
	initialize_actual_cost(actual_cost); // Set all values to INT_MAX
	
	int predecessor[graph_size]; // Initialize predecessor array
	initialize_predecessor(predecessor); // Set all values to -1
	
	bool visited[graph_size]; // Initialize visited array, set all values to false
	initialize_visited(visited); // Set all values to false
	
	actual_cost[start_index] = 0;
	
	push(pq, start_index, actual_cost[start_index] + hCostManhattan(x, y, mouse_loc));
	
	while (!is_empty(pq)) {
		int min_pos = get_min(pq);
		int curr_index = pq->nodes[min_pos].node;
		
		remove_node(pq, min_pos);
		visited[curr_index] = true;
		
		if (curr_index % size_X == x && curr_index / size_Y == y) {
			// Reached the location!
			return calculateCostPath(curr_index, predecessor);
		}
		
		int new_index;
		for (int i = 0; i < 4; i++) {
			new_index = directionToNewIndex(i, curr_index);
			if (gr[curr_index][i] == 1.0 && (obstacle_loc == NULL || !index_is_in_array(new_index, obstacle_loc, obstacles)) && !visited[new_index]) {
				if (actual_cost[curr_index] + 1 < actual_cost[new_index]) { // There's a shorter path
					actual_cost[new_index] = actual_cost[curr_index] + 1;
					predecessor[new_index] = curr_index;
					if (!is_in_queue(pq, new_index)) {
						// Add to pq
						push(pq, new_index, actual_cost[new_index] + hCostManhattan(x, y, mouse_loc));
					} else {
						update_cost(pq, new_index, actual_cost[new_index] + hCostManhattan(x, y, mouse_loc));
					}
				}
			}
		}
	} 
	
	// No path, welp the mouse has no path to this location.
	return (-1);
	free(pq);
}

int checkForTerminal(int mouse_loc[1][2],int cat_loc[10][2],int cheese_loc[10][2],int cats,int cheeses)
{
 /* 
   This function determines whether a given configuration constitutes a terminal node.
   Terminal nodes are those for which:
     - A cat eats the mouse
     or
     - The mouse eats a cheese
   
   If the node is a terminal, the function returns 1, else it returns 0
 */

 // Check for cats having lunch
 for (int i=0; i<cats; i++)
  if (mouse_loc[0][0]==cat_loc[i][0]&&mouse_loc[0][1]==cat_loc[i][1]) return(1);

 // Check for mouse having lunch
 for (int i=0; i<cheeses; i++)
  if (mouse_loc[0][0]==cheese_loc[i][0]&&mouse_loc[0][1]==cheese_loc[i][1]) return(1);

 return(0);

}