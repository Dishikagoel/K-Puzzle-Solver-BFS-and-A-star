# K-Puzzle-Solver-BFS-and-A-star

The 8-puzzle is a simple sliding tile game where 8 tiles are jumbled in a 3 x 3 grid, and the player must slide tiles around to get the board into a “goal state”. The k-puzzle is just a generalization of the 8-puzzle game, where k = n*n-1 and n is the width and height of the board.

Here I have solved the K-puzzle in two following ways:

- Using A* (pronounced as A-star) algorithm - a graph traversal and path search algorithm that uses heuristic function h(n), and cost to reach the node n from the start state g(n). A* search algorithm finds the shortest path through the search space using the heuristic function. This search algorithm expands less search tree and provides optimal result faster, which makes it the most commonly used best-first-search algorithm in AI.
- Using breadth firt search algorithm - used to search a tree or graph data structure for a node that meets a set of criteria. It begins at the root of the tree or graph and investigates all nodes at the current depth level before moving on to nodes at the next depth level.
