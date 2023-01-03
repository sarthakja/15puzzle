#include<iostream>
#include<bits/stdc++.h>
using namespace std;
using namespace std::chrono;
//every state of the 15 puzzle is stored as a stuct
struct Node{
    string state;// the current state of the 15 puzzle represented as a string
    string actionTaken;// the stream of actions taken from initial state to current state
};

//This method calculates the no of misplaced tiles for a given state
long long int misplacedTilesCount(string currentStateString){
    istringstream currentStateStream(currentStateString);
    string currentNumber;
    long long int numberCount = 1;
    long long int noOfMisplacedTiles = 0;
    while(getline(currentStateStream, currentNumber, ' ')){
        long long int no = stoi(currentNumber);
        if(numberCount == 16 && no!=0){
            noOfMisplacedTiles++;
        }
        else if(no != numberCount){
            noOfMisplacedTiles++;
        }
        numberCount++;
    }
    return noOfMisplacedTiles;
}

//This method calculates the manhattan distance for a given state
long long int manhattanDistance(string currentStateString){
    istringstream currentStateStream(currentStateString);
    string currentNumber;
    long long int numberCount = 0;
    long long int manhattanValue = 0;
    while(getline(currentStateStream, currentNumber, ' ')){
        long long int no = stoi(currentNumber);
        long long int currentRow = (int)(numberCount)/4;
        long long int currentColumn = (int)(numberCount)%4;
        long long int correctRow,correctColumn;
        if(no != 0){
            correctRow = (long long int)(no-1)/4;
            correctColumn = (long long int)(no-1)%4;
            manhattanValue+=(abs(correctRow-currentRow)+abs(correctColumn-currentColumn));
        }
        numberCount++;
    }
    return manhattanValue;
}

string moves="";//This stores the moves from initial configuration to the final configuration
long long int nodesExpanded=0;// This is the total no of nodes expanded during the idastar search
auto timeTaken=0.0;// This is the total time taken to perform the idastar search
double memoryTaken = 0.0;// This is the max runtime memory for idastar
double recursionMemory = 0.0;// This is the memory for current recursion state
unordered_set<string> recursionStates; // This stores the states in the current recursion stack.
string goalState = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0";
string result = "Failure";
long long int maxDepth = 0;// This variable holds the maximum f(n) upto which idastar can go in the current iteration
long long int heuristic = 0;//0-> No of misplaced tiles, 1-> Manhattan distance

//This method performs astar search upto max f(n) value = maxDepth
long long int idastar(Node node, long long int gn){
    long long int hn;
    string nodeState = (node.state);
    if(heuristic == 0){
        hn = misplacedTilesCount(nodeState);
    }
    else{
        hn = manhattanDistance(nodeState);
    }
    long long int fn = gn + hn;
    long long int minCutoff = LLONG_MAX;
   // If the current recursion f(n) is greater than the maximum cutoff in the current iteration of idastar, then cutoff is reached.
    if(fn>maxDepth){
        result= "Cutoff";
        return fn;
    }

    recursionStates.insert(nodeState);
    // the recursionMemory is calculated as the sum of memory for recursion and the hashset(recursionStates) holding the states is current recursion
    recursionMemory+=((double)sizeof(node))/1024;
    recursionMemory+=((double)sizeof(nodeState))/1024;
    memoryTaken = max(memoryTaken,(recursionMemory));
    // if goal state is reached, then no need to perform idastar further
    if(nodeState == goalState){
        result = "Success";
        moves = node.actionTaken;
        return minCutoff;
    }
    nodesExpanded+=1;

    istringstream currentStateStream(nodeState);
    vector<string> currentStateArray;// currentStateArray holds the current state as a 1-D array of strings
    string currentNumber;
    //currentRow and currentColumn are the row and column of the blank tile in the current state
    long long int currentRow;
    long long int currentColumn;
    long long int currentIndex;
    //the while loop generates the currentStateArray from currentState string
    while(getline(currentStateStream, currentNumber, ' ')){
        currentStateArray.push_back(currentNumber);
        if(currentNumber == "0"){
            currentIndex = (long long int)(currentStateArray.size()-1);
            currentRow = (long long int)(currentIndex)/4;
            currentColumn = (long long int)(currentIndex)%4;
        }
    }
    vector<long long int> rowStates{0, 0, 1, -1};
    vector<long long int> colStates{1, -1, 0, 0};
    vector<char> move{'R', 'L', 'D', 'U'};
    long long int newRow;
    long long int newColumn;
    long long int newIndex;
    // this loop moves the blank tile in each of the four directions- left,right,up, and down and checks whether the resulting state is present in the current
    // recursion stack. If not, then the tile is moved in the direction and recursion proceeds in that direction
    for(long long int i =0;i<4;i++){
        newRow = currentRow + rowStates[i];
        newColumn = currentColumn + colStates[i];
        if(newRow >= 0 && newRow <= 3 && newColumn >=0 && newColumn <= 3){
            vector<string> newStateArray = currentStateArray;
            newIndex = (4*newRow) + newColumn;
            swap(newStateArray[currentIndex],newStateArray[newIndex]);
            string newStateString = "";
            for(long long int j = 0; j<newStateArray.size();j++){
                newStateString.append(newStateArray[j]+" ");
            }
            newStateString.erase(newStateString.size()-1,1);

            if(recursionStates.find(newStateString) == recursionStates.end()){
                Node newStateNode;
                newStateNode.state = newStateString;
                string actionString = node.actionTaken;
                actionString.push_back(move[i]);
                newStateNode.actionTaken = actionString;
                minCutoff = min(minCutoff, idastar(newStateNode,gn+1) );
                if(result == "Success"){
                    recursionStates.erase(nodeState);
                    recursionMemory-=((double)sizeof(node))/1024;
                    recursionMemory-=((double)sizeof(nodeState))/1024;
                    return minCutoff;
                }
            }

        }
    }
    recursionStates.erase(nodeState);
    recursionMemory-=((double)sizeof(node))/1024;
    recursionMemory-=((double)sizeof(nodeState))/1024;
    return minCutoff;
}

int main(){
    cout<<"Enter the initial board configuration. There should be ONLY a single space between each number"<<endl;
    string initialConfiguration;// intialConfiguration holds the initial configuration of the board
    // This while loop checks whether input size is correct
    while(true){
        getline(cin, initialConfiguration);
        istringstream checkValid(initialConfiguration);// checkValid is used to check whether the input in initialConfiguration is valid
        vector<string> inputArray;// Array of strings for initial board configuration. Each element of array is a number on the board
        string numberString;// A number on the board represented as a string
        while(getline(checkValid, numberString, ' ')){
            inputArray.push_back(numberString);
        }
        if(inputArray.size()!=16){
            cout<<"The board format entered is invalid. Please enter a valid initial configuration."<<endl;
            continue;
        }
        break;
    }
    long long int gn;

    //performing idastar search with misplaced tiles heuristic
    heuristic = 0;
    auto start = high_resolution_clock::now();// start is the initial snaphot before bfs starts
    //this while loop performs idastar- increase the f(n) to the min value of f(n) of the node that exceeded the cutoff in the current iteration
    //and checks whether the goal state is present upto the cutoff.
    maxDepth = misplacedTilesCount(initialConfiguration);
    while(true){
        Node startNode; // startNode stores the initial configuration
        startNode.state = initialConfiguration;
        startNode.actionTaken = "";
        result = "Failure";
        recursionMemory = 0.0;
        recursionStates.clear();
        gn = 0;

        maxDepth = idastar(startNode, gn);
        if(result!="Cutoff"){
            break;
        }
    }

    auto stop = high_resolution_clock::now();// stop is the final snaphot before bfs ends
    duration<double> time_span = duration_cast<duration<double>>(stop - start);// stop - start is the total time taken by bfs
    timeTaken = time_span.count();
    cout<<endl<<"For misplaced tiles heuristic:"<<endl;
    if(result == "Success"){
        cout<<"Moves: "<<moves<<endl;
        cout<<"Number of Nodes expanded: "<<nodesExpanded<<endl;
        cout<<"Time Taken: "<<timeTaken<<" seconds"<<endl;
        cout<<"Memory Used: "<<memoryTaken<<" kb"<<endl<<endl;
    }
    else{
        cout<<"The goal state could not be found"<<endl<<endl;
    }

    //performing idastar search with manhattan distance heuristic
    moves = "";
    nodesExpanded=0;
    memoryTaken = 0.0;
    heuristic = 1;
    auto start1 = high_resolution_clock::now();
    //this while loop performs idastar- increase the f(n) to the min value of f(n) of the node that exceeded the cutoff in the current iteration
    //and checks whether the goal state is present upto the cutoff.
    maxDepth = manhattanDistance(initialConfiguration);

    while(true){
        Node startNode1; // startNode stores the initial configuration
        startNode1.state = initialConfiguration;
        startNode1.actionTaken = "";
        result = "Failure";
        recursionMemory = 0.0;
        recursionStates.clear();
        gn = 0;

        maxDepth = idastar(startNode1, gn);
        if(result!="Cutoff"){
            break;
        }
    }
    
    auto stop1 = high_resolution_clock::now();
    duration<double> time_span1 = duration_cast<duration<double>>(stop1 - start1);
    auto timeTaken2 = time_span1.count();
    cout<<"For manhattan heuristic:"<<endl;
    if(result == "Success"){
        cout<<"Moves: "<<moves<<endl;
        cout<<"Number of Nodes expanded: "<<nodesExpanded<<endl;
        cout<<"Time Taken: "<<timeTaken2<<" seconds"<<endl;
        cout<<"Memory Used: "<<memoryTaken<<" kb";
    }
    else{
        cout<<"The goal state could not be found"<<endl;
    }
    return 0;
}