# 15puzzle C++
This repository solves the 15 puzzle problem using uninformed and informed search strategies.  
BFS and idfs are the uninformed seach strategies used.  
Astar and idastar are the informed search strategies used. Both astar and idastar run with manhattan and misplaced tiles heuristic.  
  
To run the code, first run the following command:  
"g++ -std=c++11 <file name>.cpp"  
This will generate the executable file. Now run the command:  
".\a.exe"  
  
On running the command, the following prompt will appear:  
"Enter the initial board configuration. There should be ONLY a single space between each number"  
Enter the initial configuration of the board as a sequence of numbers with a 0 for the blank tile. Make sure there is ONLY one single space between each number.  
For example, for the following initial configuration:  
![alt text](https://github.com/sarthakja/15puzzle/blob/main/15puzzle_example_initial_state.PNG?raw=true)  
the input to the prompt should be:  
1 0 2 4 5 7 3 8 9 6 11 12 13 10 19 15  
All the search strategies output the following 4 values:  
1. The moves required to solve the puzzle
2. The number of nodes explored before the final goal state is reached
3. Total time taken to solve the puzzle
4. Runtime memory used to solve the puzzle
