//
//  main.cpp
//  CancerCode
//
//  Created by Olivia Gallup on 01/12/2018.
//  Copyright © 2018 Olivia Gallup. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <algorithm>

using namespace std;

// Prototypes
class Cell;
class Cancer;


class Cell {
public:
    char live = 'O';
    char dead = ' ';
    char currAppearance;            // looks live or dead
    
    bool isAlive() {
        if(currAppearance == live) {
            return true;
        } else {
            return false;
        }
    }
    
    virtual char evolve(int neighborCount) {         // Dead cells
        if(neighborCount == 3) {            // dead cell has exactly 3 neighbors
            currAppearance = live;
        }
        // Live cells
        if(neighborCount >=4) {      // cell has 4 or more neighbors
            currAppearance = dead;
        }
        if(neighborCount <=1) {      // cell has one or fewer neighbors
            currAppearance = dead;
        }
        return currAppearance;
    }
    
    int getNeighborCount(vector<vector<bool> > boolBoard, int currentRow, int currentCol, int totalRows, int totalCols) {
        int neighborCount=0;
        
        for(int dx = -1; dx<2; dx++) {
            for(int dy = -1; dy<2; dy++) {
                if((dx||dy) != 0) {         // discounts the case where index is on the cell we are actually considering
                    
                    bool inBounds = ((currentRow+dx) >=0 && (currentRow+dx) < totalRows && (currentCol+dy) >=0 && (currentCol+dy) < totalCols);
                    if( inBounds ) {           // makes sure index doesn't overflow
                        if((boolBoard[currentRow+dx][currentCol+dy])) {          // check for live neighboring cells
                            neighborCount++;
                        }
                    }
                }
            }
        }
        return neighborCount;
    }
    
    // destr
};


class Cancer : public Cell {
public:
    
    virtual char evolve(int neighborCount) {         // Dead cells
        if(neighborCount == 3) {            // dead cell has exactly 3 neighbors
            currAppearance = live;
        }
        // Live cells
        if(neighborCount >=5) {      // cell has 4 or more neighbors
            currAppearance = dead;
        }
        if(neighborCount <=1) {      // cell has one or fewer neighbors
            currAppearance = dead;
        }
        return currAppearance;
    }
};

class Board {
    vector<vector<Cell*> > board;
    
public:
    int numberOfRows;
    int numberOfCols;
    int numLive;          // Tells you how many cells you start the board with (initiated in seed_cells)
    int time;
    int cellMode;
    
    // COPY CONSTRUCTOR
    //    Board(const Board &other)
    //    {
    //        cellMode=other.cellMode;
    //        time = other.time;
    //        numLive = other.numLive;
    //        numberOfCols = other.numberOfCols;
    //        numberOfRows = other.numberOfRows;
    //        board = other.board; //only shallow copy - want to re copy pointer values
    //
    //        for(int i=0; i<other.numberOfRows; i++) {
    //            for(int j=0; j<other.numberOfCols; j++) {
    //
    //            }
    //        }
    //    }
    
    
    Board(int rows, int cols) {     // By default a board must be created with rows / cols input
        numberOfRows = rows;
        numberOfCols = cols;
        
    }
    
    void seed_cells(int cellModeSelected, int confluence) {
        cellMode = cellModeSelected;
        if(cellMode ==1) {                                      // cells are normal
            for(int i=0; i<numberOfRows; i++) {
                vector<Cell*> temp;
                for(int j=0; j<numberOfCols; j++) {
                    Cell* newCellPointer = new Cell();
                    temp.push_back(newCellPointer);
                }
                board.push_back(temp);
            }
        }
        if(cellMode ==2) {                                      // cells are normal
            cout << "sickomode activated " << endl;
            for(int i=0; i<numberOfRows; i++) {
                vector<Cell*> temp;
                for(int j=0; j<numberOfCols; j++) {
                    Cell* newCellPointer = new Cancer();
                    newCellPointer->live = 'X';
                    temp.push_back(newCellPointer);
                }
                board.push_back(temp);
            }
        }
        
        // filling board with dead cells
        for(int i=0; i<numberOfRows; i++) {
            for(int j=0; j<numberOfCols; j++) {
                board[i][j]->currAppearance = board[i][j]->dead;
            }
        }
        
        // filling board at random with the desired number of cells, numLive
        numLive = (confluence*0.01)*numberOfRows*numberOfCols;          // how many cells are on board
        vector<int> XCoordsUsed;
        vector<int> YCoordsUsed;
        srand(1);
        int cellFillCount=0;
        while(cellFillCount < numLive) {
            int randCoordX = rand() % numberOfRows;         // Getting a random x and y coordinate
            int randCoordY = rand() % numberOfCols;
            XCoordsUsed.push_back(randCoordX);              // Vectors storing all the x and y's that we already used
            YCoordsUsed.push_back(randCoordY);
            
            bool isXUsed = (find(XCoordsUsed.begin(), XCoordsUsed.end(), randCoordX) == XCoordsUsed.end());
            bool isYUsed =(find(YCoordsUsed.begin(), YCoordsUsed.end(), randCoordY) == YCoordsUsed.end());
            if(!isXUsed || !isYUsed) {              // if the xy coordinate has not been used yet, make the cell alive
                if(board[randCoordX][randCoordY]->currAppearance == board[randCoordX][randCoordY]->dead) {
                    board[randCoordX][randCoordY]->currAppearance = board[randCoordX][randCoordY]->live;
                    cellFillCount++;
                }
            }
        }
        
    }
    
    int countCells(int rows, int cols) {
        int cellCount = 0;
        for(int i=0; i<rows; i++) {
            for(int j=0; j<cols; j++) {
                if(board[i][j]->currAppearance == board[i][j]->live) {
                    cellCount++;
                }
            }
        }
        return cellCount;
    }
    
    
    void display() {
        cout << "\ntime:\t\t\t\t" << time << endl;
        cout << "number of cells:\t" << numLive << endl;
        for( int i=0; i<numberOfRows; i++) {
            for( int j=0; j<numberOfCols; j++) {
                cout << (board[i][j]->currAppearance);
            }
            cout << endl;
        }
    }
    
    
    void next_state() {
        
        // Decide what to do with each cell
        vector<vector<bool> > currentAliveCells;         // represents a copy of the board only in terms of which cells are alive or dead
        
        // initializing board of cells in terms of bools (currentAliveCells)
        for(int i=0; i<numberOfRows; i++) {
            vector<bool> temp;
            for(int j=0; j<numberOfCols; j++) {
                if(board[i][j]->currAppearance == board[i][j]->live) {
                    temp.push_back(true);
                } else {
                    temp.push_back(false);
                }
            }
            currentAliveCells.push_back(temp);
        }
        
        
        // Updates board with the new
        for(int i=0; i<numberOfRows; i++) {
            for(int j=0; j<numberOfCols; j++) {
                int neighborCount = board[i][j]->getNeighborCount(currentAliveCells, i,j, numberOfRows, numberOfCols);
                board[i][j]->currAppearance = board[i][j]->evolve(neighborCount);
            }
        }
        
        // Update time and cell count
        time++;
        numLive = countCells(numberOfRows, numberOfCols);
    }
    
};




// Functions for the use of Classes


int getCellMode() {
    int cellMode;
    cout << "Select your cell type: (1) normal cells or (2) cancer cells" << endl;
    cout << ">";
    cin >> cellMode;
    while( (cellMode != 1) && (cellMode != 2)) {
        cout << ">";
        cin >> cellMode;
    }
    // TODO: switch cell type
    //    switch (cellChoice) {
    //        case 1: cellMode = 'O';
    //            break;
    //
    //        case 2: cellMode = 'X';
    //            break;
    //
    //        default: cellMode = 'p';
    //            break;
    //    }
    return cellMode;
}
int getConfluence() {
    int confluence;
    cout << "Select the confluence percentage (%)" << endl;
    cout << ">";
    cin >> confluence;
    while( (confluence <1) || (confluence > 100)) {
        cout << ">";
        cin >> confluence;
    }
    return confluence;
}




int main() {
    int cellMode =0;
    int confluence=0;
    int c=0;              // A character taken in cin
    cout << "Welcome to the cell simulator\n" << endl;
    
    // Menu option
    cellMode = getCellMode();
    confluence = getConfluence();

    Board board(20, 75);
    board.seed_cells(cellMode, confluence);

    while(c!='q') // keep running the simulator until 'q' is entered
    {
//        system("CLS"); // clear the screen (note: omit this when debugging)
        //    cout << "time: " << board.get_time() << endl;
        //    cout << "number of cells: " << board.get_num_cells() << endl;
        board.display(); // display the board
        c = cin.get();
        board.next_state(); // get the next state (note: the complex algorithm is in here)
    }
    return 0;
    
}
