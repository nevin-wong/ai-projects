/*
	CSC D84 - Unit 3 - Reinforcement Learning
	
	This file contains stubs for implementing the Q-Learning method
	for reinforcement learning as discussed in lecture. You have to
	complete two versions of Q-Learning.
	
	* Standard Q-Learning, based on a full-state representation and
	  a large Q-Table
	* Feature based Q-Learning to handle problems too big to allow
	  for a full-state representation
	    
	Read the assignment handout carefully, then implement the
	required functions below. Sections where you have to add code
	are marked

	**************
	*** TO DO:
	**************

	If you add any helper functions, make sure you document them
	properly and indicate in the report.txt file what you added.
	
	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Jan. 16
*/

#include "QLearn.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

void QLearn_update(int s, int a, double r, int s_new, double *QTable)
{
 /*
   This function implementes the Q-Learning update as stated in Lecture. It 
   receives as input a <s,a,r,s'> tuple, and updates the Q-table accordingly.
   
   Your work here is to calculate the required update for the Q-table entry
   for state s, and apply it to the Q-table
     
   The update involves two constants, alpha and lambda, which are defined in QLearn.h - you should not 
   have to change their values. Use them as they are.
     
   Details on how states are used for indexing into the QTable are shown
   below, in the comments for QLearn_action. Be sure to read those as well!
 */
 
  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
   
   double Qsa = *(QTable+(4*s)+a);
   
   double bestQValueForSNew = -DBL_MAX;
   // Loop over all valid actions for s_new
   for (int i = 0; i < 4; i++) {
	   if (bestQValueForSNew < *(QTable+(4*s_new)+i)) {
		   bestQValueForSNew = *(QTable+(4*s_new)+i);
	   }
   }
   
   double value = alpha*(r + lambda*bestQValueForSNew - Qsa);
   
   // update Q(s, a)
   *(QTable+(4*s)+a) += value;
}

int QLearn_action(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, double *QTable, int size_X, int graph_size)
{
  /*
     This function decides the action the mouse will take. It receives as inputs
     - The graph - so you can check for walls! The mouse must never move through walls
     - The mouse position
     - The cat position
     - The cheese position
     - A 'pct' value in [0,1] indicating the amount of time the mouse uses the QTable to decide its action,
       for example, if pct=.25, then 25% of the time the mouse uses the QTable to choose its action,
       the remaining 75% of the time it chooses randomly among the available actions.
       
     Remember that the training process involves random exploration initially, but as training
     proceeds we use the QTable more and more, in order to improve our QTable values around promising
     actions.
     
     The value of pct is controlled by QLearn_core_GL, and increases with each round of training.
     
     This function *must return* an action index in [0,3] where
        0 - move up
        1 - move right
        2 - move down
        3 - move left
        
     QLearn_core_GL will print a warning if your action makes the mouse cross a wall, or if it makes
     the mouse leave the map - this should not happen. If you see a warning, fix the code in this
     function!
     
   The Q-table has been pre-allocated and initialized to 0. The Q-table has
   a size of
   
        graph_size^3 x 4
        
   This is because the table requires one entry for each possible state, and
   the state is comprised of the position of the mouse, cat, and cheese. 
   Since each of these agents can be in one of graph_size positions, all
   possible combinations yield graph_size^3 states.
   
   Now, for each state, the mouse has up to 4 possible moves (up, right,
   down, and left). We ignore here the fact that some moves are not possible
   from some states (due to walls) - it is up to the QLearn_action() function
   to make sure the mouse never crosses a wall. 
   
   So all in all, you have a big table.
        
   For example, on an 8x8 maze, the Q-table will have a size of
   
       64^3 x 4  entries
       
       with 
       
       size_X = 8		<--- size of one side of the maze
       graph_size = 64		<--- Total number of nodes in the graph
       
   Indexing within the Q-table works as follows:
   
     say the mouse is at   i,j
         the cat is at     k,l
         the cheese is at  m,n
         
     state = (i+(j*size_X)) + ((k+(l*size_X))*graph_size) + ((m+(n*size_X))*graph_size*graph_size)
     ** Make sure you undestand the state encoding above!
     
     Entries in the Q-table for this state are

     *(QTable+(4*state)+a)      <-- here a is the action in [0,3]
     
     (yes, it's a linear array, no shorcuts with brackets!)
     
     NOTE: There is only one cat and once cheese, so you only need to use cats[0][:] and cheeses[0][:]
   */
  
  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/  
	
	// Random number in [0, 1]
	double c = ((double) rand() / RAND_MAX);
	
	int s = getState(mouse_pos, cats, cheeses, size_X, graph_size);
	
	int mouseIndex = mouse_pos[0][0] + (mouse_pos[0][1]*size_X);
	
	int a; // action
	if (c <= pct) {
		// Use Qtable
		double highest = -DBL_MAX;
		for (int i = 0; i < 4; i++) {
			if (gr[mouseIndex][i] == 1.0) {
				if (highest < *(QTable+(4*s)+i)) {
					highest = *(QTable+(4*s)+i);
					a = i;
				}
			}
 		}
	} else {
		// Random action
		bool foundValidAction = false;
		while (!foundValidAction) {
			a = rand() % 4; // Random action, integer in [0, 3]
			if (gr[mouseIndex][a] == 1.0) {
				foundValidAction = true;
			}
		}
	}
	
	return(a);
}

double QLearn_reward(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function computes and returns a reward for the state represented by the input mouse, cat, and
    cheese position. 
    
    You can make this function as simple or as complex as you like. But it should return positive values
    for states that are favorable to the mouse, and negative values for states that are bad for the 
    mouse.
    
    I am providing you with the graph, in case you want to do some processing on the maze in order to
    decide the reward. 
        
    This function should return a maximim/minimum reward when the mouse eats/gets eaten respectively.      
   */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/ 
    double good = 900000000;
	double bad = -100000000;
	
	// Mouse eats cheese - good!
	for (int i = 0; i < 5; i++) {
		if (mouse_pos[0][0] == cheeses[i][0] && mouse_pos[0][1] == cheeses[i][1])
			return good;
	}
	
	// Cat eats mouse - bad!
	for (int i = 0; i < 5; i++) {
		if (mouse_pos[0][0] == cats[i][0] && mouse_pos[0][1] == cats[i][1])
			return bad;
	}
	
	// Even just ending the fcn here does very well for 8x8
	/** "It's a simple spell, but quite unbreakable." -Doctor Strange, 2018 **/
	
	int shortestDistToCat = INT_MAX;
	for (int i = 0; i < 5; i++) {
		if (!(cats[i][0] == -1 && cats[i][1] == -1)) {
			int distance = abs(mouse_pos[0][0] - cats[i][0]) + abs(mouse_pos[0][1] - cats[i][1]);
			if (shortestDistToCat > distance)
				shortestDistToCat = distance;
		}
	}
	
	int shortestDistToCheese = INT_MAX;
	for (int i = 0; i < 5; i++) {
		if (!(cheeses[i][0] == -1 && cheeses[i][1] == -1)) {
			int distance = abs(mouse_pos[0][0] - cheeses[i][0]) + abs(mouse_pos[0][1] - cheeses[i][1]);
			if (shortestDistToCheese > distance)
				shortestDistToCheese = distance;
		}
	}

	// Set weights for the distance to cat and cheese
	double closeToCheeseWt = 12;
	double farFromCatWt = 3;
		
	// Calculate reward based on distances
	return farFromCatWt*shortestDistToCat - closeToCheeseWt*shortestDistToCheese;
}

void feat_QLearn_update(double gr[max_graph_size][4],double weights[25], double reward, int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function performs the Q-learning adjustment to all the weights associated with your
    features. Unlike standard Q-learning, you don't receive a <s,a,r,s'> tuple, instead,
    you receive the current state (mouse, cats, and cheese potisions), and the reward 
    associated with this action (this is called immediately after the mouse makes a move,
    so implicit in this is the mouse having selected some action)
    
    Your code must then evaluate the update and apply it to the weights in the weight array.    
   */
  
   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/        
   
   double features[25];
   
   evaluateFeatures(gr, features, mouse_pos, cats, cheeses, size_X, graph_size);
   
   double Qs, Qs_new;
   int maxA;
   
   Qs = Qsa(weights, features);
   maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &Qs_new, &maxA);
   
   for (int i = 0; i < numFeatures; i++) {
	   weights[i] += alpha*(reward + lambda*(Qs_new) - Qs)*(features[i]);
   }
}

int feat_QLearn_action(double gr[max_graph_size][4],double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, int size_X, int graph_size)
{
  /*
    Similar to its counterpart for standard Q-learning, this function returns the index of the next
    action to be taken by the mouse.
    
    Once more, the 'pct' value controls the percent of time that the function chooses an optimal
    action given the current policy.
    
    E.g. if 'pct' is .15, then 15% of the time the function uses the current weights and chooses
    the optimal action. The remaining 85% of the time, a random action is chosen.
    
    As before, the mouse must never select an action that causes it to walk through walls or leave
    the maze.    
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/        
   
   	// Random number in [0, 1]
	double c = ((double) rand() / RAND_MAX);

	int mouseIndex = mouse_pos[0][0] + (mouse_pos[0][1]*size_X);
	
	int a; // action
	bool foundValidAction = false;
	if (c <= pct) {
		// Use Q-table
		double maxU;
		while (!foundValidAction) {
			maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &maxU, &a);
			if (gr[mouseIndex][a] == 1.0) {
				foundValidAction = true;
			}
		}
	} else {
		// Random action
		while (!foundValidAction) {
			a = rand() % 4; // Random action, integer in [0, 3]
			if (gr[mouseIndex][a] == 1.0) {
				foundValidAction = true;
			}
		}
	}
	
	return(a);
}

void evaluateFeatures(double gr[max_graph_size][4],double features[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
   This function evaluates all the features you defined for the game configuration given by the input
   mouse, cats, and cheese positions. You are free to define up to 25 features. This function will
   evaluate each, and return all the feature values in the features[] array.
   
   Take some time to think about what features would be useful to have, the better your features, the
   smarter your mouse!
   
   Note that instead of passing down the number of cats and the number of cheese chunks (too many parms!)
   the arrays themselves will tell you what are valid cat/cheese locations.
   
   You can have up to 5 cats and up to 5 cheese chunks, and array entries for the remaining cats/cheese
   will have a value of -1 - check this when evaluating your features!
  */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/      
	
	int numCats = 0;
	int numCheeses = 0;
	for (int i = 0; i < 5; i++) {
		if (cats[i][0] != -1 && cats[i][1] != -1) {
			numCats++;
		}
		if (cheeses[i][0] != -1 && cheeses[i][1] != -1) {
			numCheeses++;
		}
	}
	
	features[0] = distClosestCat(gr, mouse_pos, cats, numCats, size_X);
	features[1] = distClosestCheese(gr, mouse_pos, cheeses, numCheeses, size_X);
	features[2] = avgManhtnDistCats(mouse_pos, cats, numCats, size_X);
	features[3] = avgManhtnDistCheeses(mouse_pos, cheeses, numCheeses, size_X);
	features[4] = numAvailableMoves(gr, mouse_pos, size_X);
	features[5] = (double) numCats / 5; // Limit to range [0, 1]
	features[6] = (double) numCheeses / 5; // Limit to range [0, 1]
	features[7] = distToCenter(mouse_pos, size_X);
}

double Qsa(double weights[25], double features[25])
{
  /*
    Compute and return the Qsa value given the input features and current weights
   */

  /***********************************************************************************************
  * TO DO: Complete this function
  ***********************************************************************************************/  
	
	double Qsa = 0;
	for (int i = 0; i < numFeatures; i++) {
		Qsa += weights[i]*features[i];
	}
	
	return Qsa;
}

void maxQsa(double gr[max_graph_size][4],double weights[25],int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size, double *maxU, int *maxA)
{
 /*
   Given the state represented by the input positions for mouse, cats, and cheese, this function evaluates
   the Q-value at all possible neighbour states and returns the max. The maximum value is returned in maxU
   and the index of the action corresponding to this value is returned in maxA.
   
   You should make sure the function does not evaluate moves that would make the mouse walk through a
   wall. 
  */
 
   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/  		
	int mouseIndex = mouse_pos[0][0] + (mouse_pos[0][1]*size_X);
	
	double features[25];
	int moved_mouse_pos[1][2];
	
	double highest = -DBL_MAX;
	int a;
	for (int i = 0; i < 4; i++) {
		if (gr[mouseIndex][i] == 1.0) {
			int mouseMovedIndex = directionToNewIndex(i, mouseIndex, size_X);
			moved_mouse_pos[0][0] = mouseMovedIndex % size_X; // x
			moved_mouse_pos[0][1] = mouseMovedIndex / size_X; // y
			evaluateFeatures(gr, features, moved_mouse_pos, cats, cheeses, size_X, graph_size);
			double qValue = Qsa(weights, features);
			if (highest < qValue) {
				highest = qValue;
				a = i;
			}
		}
	}
	
	*maxU = highest;
	*maxA = a;
	
	return;
}

/***************************************************************************************************
 *  Add any functions needed to compute your features below 
 *                 ---->  THIS BOX <-----
 * *************************************************************************************************/

int directionToNewIndex(int direction, int oldIndex, int size_X) {
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

int getState(int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size) {
	int i, j, k, l, m, n;
	
	i = mouse_pos[0][0];
	j = mouse_pos[0][1];
	k = cats[0][0];
	l = cats[0][1];
	m = cheeses[0][0];
	n = cheeses[0][1];
	
	return (i+(j*size_X)) + ((k+(l*size_X))*graph_size) + ((m+(n*size_X))*graph_size*graph_size);
}

/** Re-worked A1 code (BFS) **/

// Helper function to initialize visited[] array
void initialize_visited(bool visited[]) {
    for (int i = 0; i < max_graph_size; i++) {
        visited[i] = false;
    }
}

// Helper function to initialize predecessor[] array
void initialize_predecessor(int predecessor[]) {
    for (int i = 0; i < max_graph_size; i++) {
        predecessor[i] = -1;
    }
}

bool index_is_in_array(int index, int arr[10][2], int amount, int size_X) {	
	// Used to check if an index's grid location is one of a cat/cheese
	int x = index % size_X;
	int y = index / size_X;
	
	for (int i = 0; i < amount; i++) {
		if (x == arr[i][0] && y == arr[i][1]) {
			return true;
		}
	}
	return false;
}

int calculateCostPath(int curr_index, int predecessor[max_graph_size]) {
	// Follow predecessors back to the to get the length of the path
	int curr = curr_index;
	int path_length = 0;
	while (predecessor[curr] != -1) {
		curr = predecessor[curr];
		path_length++;
	}
	return path_length;
}

int shortestPathLengthBFS(int targets[5][2], int numTargets, double gr[max_graph_size][4], int mouse_loc[1][2], int size_X) {
	bool visited[max_graph_size]; // Initialize visited array, set all values to false
	initialize_visited(visited); // Set all values to false
	
	int predecessor[max_graph_size]; // Initialize predecessor array
	initialize_predecessor(predecessor); // Set all values to -1
	
	int start_index = mouse_loc[0][0] + (mouse_loc[0][1]*size_X);
	visited[start_index] = true;
		
	int queue[max_graph_size];
	int front = 0;
	int rear = 0;
	
	queue[rear] = start_index;
	rear++;
	
	while (front != rear) {
		int curr_index = queue[front];	
		front++;
		
		if (index_is_in_array(curr_index, targets, numTargets, size_X)) {
			return calculateCostPath(curr_index, predecessor);
		}
		
		int new_index;
		for (int i = 0; i < 4; i++) {
			new_index = directionToNewIndex(i, curr_index, size_X);
			if (gr[curr_index][i] == 1.0 && !visited[new_index]) {
				visited[new_index] = true;
				predecessor[new_index] = curr_index;
				queue[rear] = new_index;
				rear++;
			}
		}
	}
	
	// No path, welp the mouse is stuck, just return worst value
	return size_X*2;
}

/****/

double distClosestObject(double gr[max_graph_size][4], int mouse_pos[1][2], int objs[5][2], int numObjs, int size_X) {
	int shortest = INT_MAX;
	for (int i = 0; i < numObjs; i++) {
		int distance = abs(mouse_pos[0][0] - objs[i][0]) + abs(mouse_pos[0][1] - objs[i][1]);
		if (shortest > distance) {
			shortest = distance;
		}
	}
	
	// If we are very close, Manhattan distance may not be an accurate indicator of whether
	// this distance is beneficial for the mouse. Use BFS to get the actual shortest path length.
	if (shortest <= 3) {
		shortest = shortestPathLengthBFS(objs, numObjs, gr, mouse_pos, size_X);
	}
	
	double maxDistance = (2*((double) size_X));
	return (double) shortest / maxDistance; // limit range to [0, 1]
}

double distClosestCat(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int numCats, int size_X) {
	return distClosestObject(gr, mouse_pos, cats, numCats, size_X);
}

double distClosestCheese(double gr[max_graph_size][4], int mouse_pos[1][2], int cheeses[5][2], int numCheeses, int size_X) {
	return distClosestObject(gr, mouse_pos, cheeses, numCheeses, size_X);
}

double avgManhtnDist(int mouse_pos[1][2], int objs[5][2], int numObjs, int size_X) {
	int sum = 0;
	int total = 0;
	for (int i = 0; i < numObjs; i++) {
		int distance = abs(mouse_pos[0][0] - objs[i][0]) + abs(mouse_pos[0][1] - objs[i][1]);
		sum += distance;
		total++;
	}
	double avg = (double) sum / (double) total;
	double maxAvg = (2*((double) size_X));
	return (avg / maxAvg); // limit range to [0, 1]
}

double avgManhtnDistCats(int mouse_pos[1][2], int cats[5][2], int numCats, int size_X) {
	return avgManhtnDist(mouse_pos, cats, numCats, size_X);
}

double avgManhtnDistCheeses(int mouse_pos[1][2], int cheeses[5][2], int numCheeses, int size_X) {
	return avgManhtnDist(mouse_pos, cheeses, numCheeses, size_X);
}

double numAvailableMoves(double gr[max_graph_size][4], int mouse_pos[1][2], int size_X) {
	int num = 0;
	int mouseIndex = mouse_pos[0][0] + (mouse_pos[0][1]*size_X);
	for (int i = 0; i < 4; i++) {
		if (gr[mouseIndex][i] == 1.0) {
			num++;
		}
 	}
	return (double) num / 4; // limit range to [0, 1]
}

double distToCenter(int mouse_pos[1][2], int size_X) {
	double dist = abs(mouse_pos[0][0] - (double) size_X / 2) + abs(mouse_pos[0][1] - (double) size_X / 2);
	double maxDistance = (double) size_X; // (Max is when mouse is in a corner)
	return dist / maxDistance; // limit range to [0, 1]
}