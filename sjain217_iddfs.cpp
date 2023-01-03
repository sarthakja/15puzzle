#include<iostream>
#include<bits/stdc++.h>
using namespace std;
using namespace std::chrono;
//every state of the 15 puzzle is stored as a stuct
struct Node{
    string state;// the current state of the 15 puzzle represented as a string
    string actionTaken;// the stream of actions taken from initial state to current state
};

string moves="";//This stores the moves from initial configuration to the final configuration
int nodesExpanded=0;// This is the total no of nodes expanded during the bfs search
auto timeTaken=0.0;// This is the total time taken to perform the bfs search
double memoryTaken = 0.0;// This is the max runtime memory for idfs
double recursionMemory = 0.0;// This is the memory for current recursion state
unordered_set<string> recursionStates; // This stores the states in the current recursion stack.
string goalState = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0";
string result = "Failure";
int maxDepth = 0;// This variable holds the maximum depth upto which idfs can go in the current iteration

void idfs(Node node, int depth){
   // If the current recursion depth is greater than the maximum depth permitted in the current iteration of idfs, then cutoff is reached.
    if(depth>maxDepth){
        result= "Cutoff";
        return;
    }
    string nodeState = (node.state);
    recursionStates.insert(nodeState);
    // the recursionMemory is calculated as the sum of memory for recursion and the hashset(recursionStates) holding the states is current recursion
    recursionMemory+=((double)sizeof(node))/1024;
    recursionMemory+=((double)sizeof(nodeState))/1024;
    memoryTaken = max(memoryTaken,(recursionMemory));
    // if goal state is reached, then no need to perform idfs further
    if(nodeState == goalState){
        result = "Success";
        moves = node.actionTaken;
        return;
    }
    nodesExpanded+=1;

    istringstream currentStateStream(nodeState);
    vector<string> currentStateArray;// currentStateArray holds the current state as a 1-D array of strings
    string currentNumber;
    //currentRow and currentColumn are the row and column of the blank tile in the current state
    int currentRow;
    int currentColumn;
    int currentIndex;
    //the while loop generates the currentStateArray from currentState string
    while(getline(currentStateStream, currentNumber, ' ')){
        currentStateArray.push_back(currentNumber);
        if(currentNumber == "0"){
            currentIndex = (int)(currentStateArray.size()-1);
            currentRow = (int)(currentIndex)/4;
            currentColumn = (int)(currentIndex)%4;
        }
    }
    vector<int> rowStates{0, 0, 1, -1};
    vector<int> colStates{1, -1, 0, 0};
    vector<char> move{'R', 'L', 'D', 'U'};
    int newRow;
    int newColumn;
    int newIndex;
    // this loop moves the blank tile in each of the four directions- left,right,up, and down and checks whether the resulting state is present in the current
    // recursion stack. If not, then the tile is moved in the direction and recursion proceeds in that direction
    for(int i =0;i<4;i++){
        newRow = currentRow + rowStates[i];
        newColumn = currentColumn + colStates[i];
        if(newRow >= 0 && newRow <= 3 && newColumn >=0 && newColumn <= 3){
            vector<string> newStateArray = currentStateArray;
            newIndex = (4*newRow) + newColumn;
            swap(newStateArray[currentIndex],newStateArray[newIndex]);
            string newStateString = "";
            for(int j = 0; j<newStateArray.size();j++){
                newStateString.append(newStateArray[j]+" ");
            }
            newStateString.erase(newStateString.size()-1,1);

            if(recursionStates.find(newStateString) == recursionStates.end()){
                Node newStateNode;
                newStateNode.state = newStateString;
                string actionString = node.actionTaken;
                actionString.push_back(move[i]);
                newStateNode.actionTaken = actionString;
                idfs(newStateNode,depth+1);
                if(result == "Success"){
                    recursionStates.erase(nodeState);
                    recursionMemory-=((double)sizeof(node))/1024;
                    recursionMemory-=((double)sizeof(nodeState))/1024;
                    return;
                }
            }

        }
    }
    recursionStates.erase(nodeState);
    recursionMemory-=((double)sizeof(node))/1024;
    recursionMemory-=((double)sizeof(nodeState))/1024;
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

    auto start = high_resolution_clock::now();// start is the initial snaphot before bfs starts
    int depth;
    //this while loop performs idfs- increase the depth by 1 in each iteration and checks whether the goal state is present upto the depth.
    while(true){
        Node startNode; // startNode stores the initial configuration
        startNode.state = initialConfiguration;
        startNode.actionTaken = "";
        result = "Failure";
        recursionMemory = 0.0;
        depth = 0;
        idfs(startNode, depth);
        if(result!="Cutoff"){
            break;
        }
        maxDepth+=1;
    }

    auto stop = high_resolution_clock::now();// stop is the final snaphot before bfs ends
    duration<double> time_span = duration_cast<duration<double>>(stop - start);// stop - start is the total time taken by bfs
    timeTaken = time_span.count();
    if(result == "Success"){
        cout<<"Moves: "<<moves<<endl;
        cout<<"Number of Nodes expanded: "<<nodesExpanded<<endl;
        cout<<"Time Taken: "<<timeTaken<<" seconds"<<endl;
        cout<<"Memory Used: "<<memoryTaken<<" kb";
    }
    else{
        cout<<"The goal state could not be found";
    }
    return 0;
}