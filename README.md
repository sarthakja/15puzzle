# 15puzzle
This repository solves the 15 puzzle problem using uninformed and informed search strategies.  
BFS and idfs are the uninformed searh strategies used.  
Astar and idastar are the informed search strategies used. Both astar and idastar run with manhattan and misplaced tiles heuristic.  
  
To run the code, first run the following command:  
"g++ -std=c++11 <file name>.cpp"  
This will generate the executable file. Now run the command:  
".\a.exe"  
  
On running the command, the following prompt will appear:  
"Enter the initial board configuration. There should be ONLY a single space between each number"  
Enter the initial configuration of the board as a sequence of numbers with a 0 for the blank tile. Make sure there is ONLY one single space between each number.  
  
All the search strategies output the following 4 information:  
1. The moves required to solve the puzzle
2. The number of nodes explored before the final goal state is reached
3. Total time taken to solve the puzzle
4. Runtime memory used to solve the puzzle
