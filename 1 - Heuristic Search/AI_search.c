/*
	CSC D84 - Unit 1 - Search

	This file contains stubs for implementing the different search
	algorithms covered in the course. Please read the assignment
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

	Starter code: F.J.E., Jul. 15
	Updated: F.J.E., Jan. 18
*/

/**********************************************************************
% COMPLETE THIS TEXT BOX:
%
% 1) Student Name: Nevin
% 2) Student Name:		
%
% 1) Student number:
% 2) Student number:
% 
% 1) UtorID wongnevi
% 2) UtorID
% 
% We hereby certify that the work contained here is our own
%
% Nevin Wong
% (sign with your name)            (sign with your name)
***********************************************************************/

#include "AI_search.h"
#include<unistd.h>
#include <limits.h>

// Sets the path so the mouse doesn't move
void stuck_mouse_set_path(int path[graph_size][2], int mouse_loc[1][2]) {
	// printf("The mouse is stuck\n");
    path[0][0] = mouse_loc[0][0];
    path[0][1] = mouse_loc[0][1];
    path[1][0] = mouse_loc[0][0];
    path[1][1] = mouse_loc[0][1];
}

void populate_path_array(int curr_index, int predecessor[graph_size], int path[graph_size][2]) {
	// Follow predecessors back to the to get the length of the path
	int curr = curr_index;
	int path_length = 0;
	while (predecessor[curr] != -1) {
		curr = predecessor[curr];
		path_length++;
	}
	
	// Populate path array accordingly
	curr = curr_index;
	int j = path_length;
	while (predecessor[curr] != -1) {
		path[j][0] = curr % size_X;
		path[j][1] = curr / size_Y;
		curr = predecessor[curr];
		j--;
	}
}

// Helper function to initialize actual_cost[] array
void initialize_actual_cost(int actual_cost[]) {
    for (int i = 0; i < graph_size; i++) {
        actual_cost[i] = INT_MAX;
    }
}

// Helper function to initialize visited[] array
void initialize_visited(bool visited[]) {
    for (int i = 0; i < graph_size; i++) {
        visited[i] = false;
    }
}

// Helper function to initialize predecessor[] array
void initialize_predecessor(int predecessor[]) {
    for (int i = 0; i < graph_size; i++) {
        predecessor[i] = -1;
    }
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

void bfs(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2])
{	
	bool visited[graph_size]; // Initialize visited array, set all values to false
	initialize_visited(visited); // Set all values to false
	
	int predecessor[graph_size]; // Initialize predecessor array
	initialize_predecessor(predecessor); // Set all values to -1
	
	int start_index = mouse_loc[0][0] + (mouse_loc[0][1]*size_Y);
	visited[start_index] = true;
		
	int queue[graph_size];
	int front = 0;
	int rear = 0;
	
	queue[rear] = start_index;
	rear++;
	
	while (front != rear) {
		int curr_index = queue[front];	
		front++;
		visit_order[curr_index % size_X][curr_index / size_Y] = front;
		
		if (index_is_in_array(curr_index, cheese_loc, cheeses)) {
			// Cheese has been found!
			populate_path_array(curr_index, predecessor, path);
			return;
		}
		
		// top
		int top_index = curr_index - size_X;
		if (gr[curr_index][0] == 1.0 && !index_is_in_array(top_index, cat_loc, cats) && !visited[top_index]) {
			visited[top_index] = true;
			predecessor[top_index] = curr_index;
			queue[rear] = top_index;
			rear++;
		}
		
		// right
		int right_index = curr_index + 1;
		if (gr[curr_index][1] == 1.0 && !index_is_in_array(right_index, cat_loc, cats) && !visited[right_index]) {
			visited[right_index] = true;
			predecessor[right_index] = curr_index;
			queue[rear] = right_index;
			rear++;
		}
		
		// bottom
		int bottom_index = curr_index + size_X;
		if (gr[curr_index][2] == 1.0 && !index_is_in_array(bottom_index, cat_loc, cats) && !visited[bottom_index]) {
			visited[bottom_index] = true;
			predecessor[bottom_index] = curr_index;
			queue[rear] = bottom_index;
			rear++;
		}
		
		// left
		int left_index = curr_index - 1;
		if (gr[curr_index][3] == 1.0 && !index_is_in_array(left_index, cat_loc, cats) && !visited[left_index]) {
			visited[left_index] = true;
			predecessor[left_index] = curr_index;
			queue[rear] = left_index;
			rear++;
		}
	}
	
	// No path, welp the mouse is stuck.
    // Cheese is not reachable, just stay in the same position until the cat moves,
	// or the cat eats the mouse.
	stuck_mouse_set_path(path, mouse_loc);
}

bool dfs_util(int curr_index, int* amount_visted, bool visited[graph_size], int predecessor[graph_size], double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2])
{
	if (index_is_in_array(curr_index, cheese_loc, cheeses)) {
		// Cheese has been found!
		populate_path_array(curr_index, predecessor, path);
		return true;
	}
	
	visited[curr_index] = true;
	(*amount_visted)++;
	visit_order[curr_index % size_X][curr_index / size_Y] = *amount_visted;
	
	// top
	int top_index = curr_index - size_X;
	if (gr[curr_index][0] == 1.0 && !index_is_in_array(top_index, cat_loc, cats) && !visited[top_index]) {
		predecessor[top_index] = curr_index;
		if (dfs_util(top_index, amount_visted, visited, predecessor, gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc))
			return true;
	}
	
	// right
	int right_index = curr_index + 1;
	if (gr[curr_index][1] == 1.0 && !index_is_in_array(right_index, cat_loc, cats) && !visited[right_index]) {
		predecessor[right_index] = curr_index;
		if (dfs_util(right_index, amount_visted, visited, predecessor, gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc))
			return true;
	}
	
	// bottom
	int bottom_index = curr_index + size_X;
	if (gr[curr_index][2] == 1.0 && !index_is_in_array(bottom_index, cat_loc, cats) && !visited[bottom_index]) {
		predecessor[bottom_index] = curr_index;
		if (dfs_util(bottom_index, amount_visted, visited, predecessor, gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc))
			return true;
	}
	
	// left
	int left_index = curr_index - 1;
	if (gr[curr_index][3] == 1.0 && !index_is_in_array(left_index, cat_loc, cats) && !visited[left_index]) {
		predecessor[left_index] = curr_index;
		if (dfs_util(left_index, amount_visted, visited, predecessor, gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc))
			return true;
	}
	
	return false;
}

void dfs(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2])
{
	bool visited[graph_size]; // Initialize visited array, set all values to false
	initialize_visited(visited); // Set all values to false

	int predecessor[graph_size]; // Initialize predecessor array
	initialize_predecessor(predecessor); // Set all values to -1
	
	int amt_nodes_visited = 0;
	bool cheese_reached = false;
	int start_index = mouse_loc[0][0] + (mouse_loc[0][1]*size_Y); // Initial mouse index
	
	cheese_reached = dfs_util(start_index, &amt_nodes_visited, visited, predecessor, gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc);

	if (!cheese_reached) {
		// No path, welp the mouse is stuck.
        // Cheese is not reachable, just stay in the same position until the cat moves,
		// or the cat eats the mouse.
		stuck_mouse_set_path(path, mouse_loc);
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

void a_star(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]))
{
	struct PriorityQueue* pq = create_queue();
	
	int start_index = mouse_loc[0][0] + (mouse_loc[0][1]*size_Y); // Initial mouse index
	
	int actual_cost[graph_size]; // Array of the actual cost of reaching each node from the mouse location [g(n)]
	initialize_actual_cost(actual_cost); // Set all values to INT_MAX
	
	int predecessor[graph_size]; // Initialize predecessor array
	initialize_predecessor(predecessor); // Set all values to -1
	
	bool visited[graph_size]; // Initialize visited array, set all values to false
	initialize_visited(visited); // Set all values to false
	
	actual_cost[start_index] = 0;
	
	push(pq, start_index, actual_cost[start_index] + heuristic(start_index % size_X, start_index / size_Y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr));
	
	int amt_visited = 0;
	while (!is_empty(pq)) {
		int min_pos = get_min(pq);
		int curr_index = pq->nodes[min_pos].node;
		
		remove_node(pq, min_pos);
		visited[curr_index] = true;
		amt_visited++;
		visit_order[curr_index % size_X][curr_index / size_Y] = amt_visited;
		
		if (index_is_in_array(curr_index, cheese_loc, cheeses)) {
			// Cheese has been found!
			populate_path_array(curr_index, predecessor, path);
			return;
		}
		
		// Top
		int top_index = curr_index - size_X;
		if (gr[curr_index][0] == 1.0 && !index_is_in_array(top_index, cat_loc, cats) && !visited[top_index]) {
			if (actual_cost[curr_index] + 1 < actual_cost[top_index]) { // There's a shorter path
				actual_cost[top_index] = actual_cost[curr_index] + 1;
				predecessor[top_index] = curr_index;
				if (!is_in_queue(pq, top_index)) {
					// Add to pq
					push(pq, top_index, actual_cost[top_index] + heuristic(top_index % size_X, top_index / size_Y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr));
				} else {
					update_cost(pq, top_index, actual_cost[top_index] + heuristic(top_index % size_X, top_index / size_Y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr));
				}
			}
		}
		
		// Right
		int right_index = curr_index + 1;
		if (gr[curr_index][1] == 1.0 && !index_is_in_array(right_index, cat_loc, cats) && !visited[right_index]) {
			if (actual_cost[curr_index] + 1 < actual_cost[right_index]) { // There's a shorter path
				actual_cost[right_index] = actual_cost[curr_index] + 1;
				predecessor[right_index] = curr_index;
				if (!is_in_queue(pq, right_index)) {
					// Add to pq
					push(pq, right_index, actual_cost[right_index] + heuristic(right_index % size_X, right_index / size_Y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr));
				} else {
					update_cost(pq, right_index, actual_cost[right_index] + heuristic(right_index % size_X, right_index / size_Y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr));
				}
			}
		}
		
		// Bottom
		int bottom_index = curr_index + size_X;
		if (gr[curr_index][2] == 1.0 && !index_is_in_array(bottom_index, cat_loc, cats) && !visited[bottom_index]) {
			if (actual_cost[curr_index] + 1 < actual_cost[bottom_index]) { // There's a shorter path
				actual_cost[bottom_index] = actual_cost[curr_index] + 1;
				predecessor[bottom_index] = curr_index;
				if (!is_in_queue(pq, bottom_index)) {
					// Add to pq
					push(pq, bottom_index, actual_cost[bottom_index] + heuristic(bottom_index % size_X, bottom_index / size_Y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr));
				} else {
					update_cost(pq, bottom_index, actual_cost[bottom_index] + heuristic(bottom_index % size_X, bottom_index / size_Y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr));
				}
			}
		}
		
		// Left
		int left_index = curr_index - 1;
		if (gr[curr_index][3] == 1.0 && !index_is_in_array(left_index, cat_loc, cats) && !visited[left_index]) {
			if (actual_cost[curr_index] + 1 < actual_cost[left_index]) { // There's a shorter path
				actual_cost[left_index] = actual_cost[curr_index] + 1;
				predecessor[left_index] = curr_index;
				if (!is_in_queue(pq, left_index)) {
					// Add to pq
					push(pq, left_index, actual_cost[left_index] + heuristic(left_index % size_X, left_index / size_Y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr));
				} else {
					update_cost(pq, left_index, actual_cost[left_index] + heuristic(left_index % size_X, left_index / size_Y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr));
				}
			}
		}
	}
	
	// No path, welp the mouse is stuck.
    // Cheese is not reachable, just stay in the same position until the cat moves,
	// or the cat eats the mouse.
	stuck_mouse_set_path(path, mouse_loc);
	free(pq);
}

void search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]))
{
 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in AI_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:
	
	The game takes place on a grid of size 32x32, the file board_layout.h specifies the size and defines two
	constants 'size_X' and 'size_Y' for the horizontal and vertical size of the board, respectively. For our
	purposes, the grid of locations is represented by a graph with one node per grid location, so given
	the 32x32 cells, the graph has 1024 nodes. 

	To create a maze, we connect cell locations in the grid in such a way that a) there is a path from any
	grid location to any other grid location (i.e. there are no disconnected subsets of nodes in the graph),
	and b) there are loops.

	Since each node represents a grid location, each node can be connected to up to 4 neighbours in the
	top, right, bottom, and left directions respectively:

			node at (i,j-1)
				^
				|
	(node at i-1, j) <- node at (i,j) -> node at (i+1, j)
				|
				v
			node at (i,j+1)

	The graph is theredore stored as an adjacency list with size 1024 x 4, with one row per node in the
	graph, and 4 columns corresponding to the weight of an edge linking the node with each of its 4
	possible neighbours in the order towp, right, bottom, left (clockwise from top).

	Since all we care is whether nodes are connected. Weights will be either 0 or 1, if the weight is
	1, then the neighbouring nodes are connected, if the weight is 0, they are not. For example, if

	graph[i][0] = 0 
	graph[i][1] = 1
	graph[i][2] = 0
	graph[i][3] = 1

	then node i is connected to the right and left neighbours, but not to top or bottom.

	The index in the graph for the node corresponding to grid location (x,y) is

	index = x + (y*size_X) 		or in this case		index = x + (y*32)

	Conversely, if you have the index and want to figure out the grid location,

	x = index % size_X		or in this case		x = index % 32
	y = index / size_Y		or in this case		y = index / 32

	(all of the above are *integer* operations!)

	A path is a sequence of (x,y) grid locations. We store it using an array of size	
	1024 x 2 (since there are 1024 locations, this is the maximum length of any
	path that visits locations only once).

	Agent locations are coordinate pairs (x,y)	

   Arguments:
		gr[graph_size][4]   - This is an adjacency list for the maze
		path[graph_size][2] - An initially empty path for your code to fill.
				      In this case, empty means all entries are initially -1
		visit_order[size_X][size_Y]	- An array in which your code will store the
						  *order* in which grid locations were 
						  visited during search. For example, while
						  doing BFS, the initial location is the
						  start location, it's visit order is 1.
						  Then the search would expand the immediate
						  neighbours of the initial node in some order,
						  these would get a visit order of 2, 3, 4, and
						  5 respectively, and so on.
					
						  This array will be used to display the search
						  pattern generated by each search method.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- BFS
					mode = 1	- DFS
					mode = 2	- A*

		(*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[10][2], int cats, int cheeses)
				- This is a pointer to one of the heuristic functions you will implement, either H_cost()
				  or H_cost_nokitty(). The driver in AI_search_core_GL will pass the appropriate pointer
				  depending on what search the user wants to run.
			
				  If the mode is 0 or 1, this pointer is NULL

				  * How to call the heuristic function from within this function : *
					- Like any other function:
						h = heuristic( x, y, cat_loc, cheese_loc, mouse_loc, cats, cheeses);

   Return values:
		Your search code will directly update data passed-in as arguments:

		- path[graph_size][2]	: Your search code will update this array to contain the path from
					  the mouse to one of the cheese chunks. The order matters, so
					  path[0][:] must be the mouse's current location, path[1][:]
					  is the next move for the mouse. Each successive row will contain
					  the next move toward the cheese, and the path ends at a location
					  whose coordinates correspond to one of the cheese chunks.
					  Any entries beyond that must remain set to -1
		- visit_order[size_X][size_Y] 	:  Your search code will update this array to contain the
						   order in which each location in the grid was expanded
						   during search. This means, when that particular location
						   was checked for being a goal, and if not a goal, had its
						   neighbours added as candidates for future expansion (in
						   whatever order is dictated by the search mode).

						   Note that since there are 1024 locations, the values in 
						   this array must always be in [0, 1023]. The driver code
						   will then display search order as a yellow-colored
						   brightness map where nodes expanded earlier will look
						   brighter.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *
			
		That's that, now, implement your solution!
 */

 /********************************************************************************************************
 * 
 * TO DO:	Implement code to carry out the different types of search depending on the
 *		mode.
 *
 *		You can do this by writing code within this single function (and being clever about it!)
 *		Or, you can use this function as a wrapper that calls a different search function
 *		 (BFS, DFS, A*) depending on the mode. Note that in the latter case, you will have
 *	    	  to inform your A* function somehow of what heuristic it's supposed to use.
 *
 *		Visiting Order: When adding the neighbours of a node to your list of candidates for
 *				expansion, do so in the order top, right, bottom, left.
 *
 *		NOTE: Your search functions should be smart enough to not choose a path that goes
 *		      through a cat! this is easily done without any heuristics.
 * 
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/

 // Stub so that the code compiles/runs - The code below will be removed and replaced by your code!

 //path[0][0]=mouse_loc[0][0];
 //path[0][1]=mouse_loc[0][1];
 //path[1][0]=mouse_loc[0][0];
 //path[1][1]=mouse_loc[0][1];
 
 if (mode == 0) { // BFS
	bfs(gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc);
 } else if (mode == 1) { // DFS
	dfs(gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc);
 } else if (mode == 2) { // A*
    a_star(gr, path, visit_order, cat_loc, cats, cheese_loc, cheeses, mouse_loc, heuristic);
 }
 
 return;
}

int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
	/*
		This function computes and returns the heuristic cost for location x,y.
		As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
		The goal is cheese. Which cheese is up to you.
		Whatever you code here, your heuristic must be admissible.

		Input arguments:

			x,y - Location for which this function will compute a heuristic search cost
			cat_loc - Cat locations
			cheese_loc - Cheese locations
			mouse_loc - Mouse location
			cats - # of cats
			cheeses - # of cheeses
			gr - The graph's adjacency list for the maze

			These arguments are as described in the search() function above
	*/
	
	// This heuristic function h(n) will essentially be the shortest Manhattan distance from location x,y
	// to ANY cheese on the map
	
	if (cheeses == 0)
		return 0;
	
	int shortest = INT_MAX;
	for (int i = 0; i < cheeses; i++) {
		if (abs(x - cheese_loc[i][0]) + abs(y - cheese_loc[i][1]) < shortest) {
			shortest = abs(x - cheese_loc[i][0]) + abs(y - cheese_loc[i][1]);
		}
	}
	return shortest;
}

int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
	/*
		This function computes and returns the heuristic cost for location x,y.
		As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
		The goal is cheese. 

		However - this time you want your heuristic function to help the mouse avoid being eaten.
		Therefore: You have to somehow incorporate knowledge of the cats' locations into your
		heuristic cost estimate. How well you do this will determine how well your mouse behaves
		and how good it is at escaping kitties.

		This heuristic *does not have to* be admissible.

		Input arguments have the same meaning as in the H_cost() function above.
	*/
	
	// This heuristic function h(n) is the shortest Manhattan distance to any cheese on the map minus 2 times
	// the shortest Manhattan distance to any cat on the map. Though this makes it inadmissible since h(n) can be
	// negative.
	
	int shortest_dist_to_cheese = INT_MAX;
	for (int i = 0; i < cheeses; i++) {
		if (abs(x - cheese_loc[i][0]) + abs(y - cheese_loc[i][1]) < shortest_dist_to_cheese) {
			shortest_dist_to_cheese = abs(x - cheese_loc[i][0]) + abs(y - cheese_loc[i][1]);
		}
	}
	
	int shortest_dist_to_cat = INT_MAX;
	for (int i = 0; i < cats; i++) {
		if (abs(x - cat_loc[i][0]) + abs(y - cat_loc[i][1]) < shortest_dist_to_cat) {
			shortest_dist_to_cat = abs(x - cat_loc[i][0]) + abs(y - cat_loc[i][1]);
		}
	}
	
	return shortest_dist_to_cheese - 2*shortest_dist_to_cat;
}

