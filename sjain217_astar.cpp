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
int nodesExpanded=0;// This is the total no of nodes expanded during the astar search
auto timeTaken=0.0;// This is the total time taken to perform the astar search
double memoryTaken = 0.0;// This is the runtime memory for astar
string goalState = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0";
int heuristic = 0;//0-> No of misplaced tiles, 1-> Manhattan distance

//This method calculates the no of misplaced tiles for a given state
int misplacedTilesCount(string currentStateString){
    istringstream currentStateStream(currentStateString);
    string currentNumber;
    int numberCount = 1;
    int noOfMisplacedTiles = 0;
    while(getline(currentStateStream, currentNumber, ' ')){
        int no = stoi(currentNumber);
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

//This method decides the order of states in the frontier according to their evaluation function
bool comp(pair<long long, Node> x, pair<long long, Node> y){
    return ((x.first)<=(y.first));
}

//This method performs the astar search to find the solution starting from an initial state
void astar(Node node){
    string nodeState = (node.state);
    // if the initial state itself is the goal state then no need to perform bfs
    if(nodeState == goalState){
        return;
    }
    unordered_map<string,pair<long long,int> > reachedSet; //reachedSet holds the states that have already been seen by astar algorithm
    if(heuristic == 0){
        reachedSet[nodeState] = make_pair(misplacedTilesCount(nodeState),0);//0->Not expanded, 1->Expanded
    }
    else{
        reachedSet[nodeState] = make_pair(manhattanDistance(nodeState),0);//0->Not expanded, 1->Expanded
    }

    memoryTaken+=( (double)sizeof(node.state) +(double)sizeof(pair<long long,int>) )/1024;
    multiset<pair<long long, Node>, decltype(comp)* > frontier{comp}; // frontier is a priority queue that dictates the next node to be chosen for expansion
    if(heuristic == 0){
        frontier.insert(make_pair(misplacedTilesCount(nodeState), node));
    }
    else{
        frontier.insert(make_pair(manhattanDistance(nodeState), node));
    }

    //This while loop performs the astar search: choosing a node from the frontier based on the value of evaluation function and then expanding the node
    while(!frontier.empty()){
        Node currentNode;
        auto it = frontier.begin();
        pair<long long, Node> frontPair = (*it);
        currentNode = frontPair.second;
        frontier.erase(it);
        string currentState = currentNode.state;
        string currentAction = currentNode.actionTaken;
        if(currentState == goalState){
            moves = currentNode.actionTaken;
            return;
        }

        if( ((reachedSet[currentState]).second) == 1 ){
            continue;
        }
        nodesExpanded+=1;
        ((reachedSet[currentState]).second) = 1;
        istringstream currentStateStream(currentState);
        vector<string> currentStateArray;// currentStateArray holds the current state as an array of strings
        string currentNumber;
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
        //this loop moves the blank tile in each of the four directions- left,right,up, and down and checks whether the resulting state is seen or not before.
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

                int hn;
                if(heuristic == 0){
                    hn = misplacedTilesCount(newStateString);
                }
                else{
                    hn = manhattanDistance(newStateString);
                }
                long long int gn =  currentAction.length()+1;
                long long int fn = gn+(long long)hn;
                //this if checks whether newStateString is seen before or not. If not or the node has a lower value of the evaluation function
                //, it is added to the frontier and to the reached set
                if(reachedSet.find(newStateString) == reachedSet.end() || ((reachedSet[newStateString]).first) >fn  ){
                    reachedSet[newStateString]= make_pair(fn,0);
                    Node newStateNode;
                    newStateNode.state = newStateString;
                    string actionString = currentNode.actionTaken;
                    actionString.push_back(move[i]);
                    newStateNode.actionTaken = actionString;
                    memoryTaken+=((double)sizeof(newStateNode))/1024;
                    memoryTaken+=((double)sizeof((newStateNode.state)) + (double)sizeof(pair<long long,int>) )/1024;
                    frontier.insert(make_pair(fn, newStateNode));
                }
            }
        }
    }
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
        }
        break;
    }
    //performing astar search with misplaced tiles heuristic
    heuristic = 0;
    Node startNode; // startNode stores the initial configuration
    startNode.state = initialConfiguration;
    startNode.actionTaken = "";
    memoryTaken+=((double)sizeof(startNode))/1024;// memory storage value is incremented whenever a new node corresponding to a new puzzle state is created
    auto start = high_resolution_clock::now();// start is the initial snaphot before bfs starts
    astar(startNode);
    auto stop = high_resolution_clock::now();// stop is the final snaphot before bfs ends
    duration<double> time_span = duration_cast<duration<double>>(stop - start);// stop - start is the total time taken by bfs
    timeTaken = time_span.count();
    cout<<endl;
    cout<<"For misplaced tiles heuristic:"<<endl;
    cout<<"Moves: "<<moves<<endl;
    cout<<"Number of Nodes expanded: "<<nodesExpanded<<endl;
    cout<<"Time Taken: "<<timeTaken<<" seconds"<<endl;
    cout<<"Memory Used: "<<memoryTaken<<" kb"<<endl<<endl;

    //performing astar search with manhattan distance heuristic
    moves = "";
    nodesExpanded=0;
    //timeTaken=0.0;
    memoryTaken = 0.0;
    heuristic = 1;
    Node startNode1; // startNode1 stores the initial configuration
    startNode1.state = initialConfiguration;
    startNode1.actionTaken = "";
    memoryTaken+=((double)sizeof(startNode1))/1024;
    auto start1 = high_resolution_clock::now();
    astar(startNode1);
    auto stop1 = high_resolution_clock::now();
    duration<double> time_span1 = duration_cast<duration<double>>(stop1 - start1);
    auto timeTaken2 = time_span1.count();
    cout<<"For manhattan heuristic:"<<endl;
    cout<<"Moves: "<<moves<<endl;
    cout<<"Number of Nodes expanded: "<<nodesExpanded<<endl;
    cout<<"Time Taken: "<<timeTaken2<<" seconds"<<endl;
    cout<<"Memory Used: "<<memoryTaken<<" kb";
    return 0;
}