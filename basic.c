/* 
 * File:   Lab 7, Part 2.c
 * Author: Nirmit Zinzuwadia
 *
 * Created on November, 11 2015, 11:18 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/resource.h>

// Method which prompts and get the information from user at the beginning.
void initialPropmts(int *boardDimension, char *computerColour) {
    printf("Enter the board dimension: ");
    scanf("%d", boardDimension);

    printf("\nComputer plays (B/W) : ");
    scanf(" %c", computerColour);
}

// Methods used to display the initial board. 
void initialBoard(char board[26][26], int dimension) {
    int row, col; // variables store row and col values
    int middle = dimension / 2; // determines the middle index of the board 

    for (row = 0; row < dimension; row++) {
        for (col = 0; col < dimension; col++) {
            // setting up the middle part of the board 
            if (row == middle - 1 && col == middle - 1) {
                board[row][col] = 'W';
            } else if (row == middle - 1 && col == middle) {
                board[row][col] = 'B';
            } else if (row == middle && col == middle - 1) {
                board[row][col] = 'B';
            } else if (row == middle && col == middle) {
                board[row][col] = 'W';
            } else {
                board[row][col] = 'U';
            }
        }
    }
}

// Method used to print board after each turn 
void printBoard(char board[26][26], int dimension) {
    int row, col, charHeader; // following variables are used in the for loops below. 
    char boardLocation = 'a'; // stores the starting value of the header. 

    printf("\n  ");

    // Printing the top character header for the board 
    for (charHeader = 0; charHeader < dimension; charHeader++) {
        printf("%c", boardLocation);
        boardLocation++;
    }

    // boardLocation is reinitialized to 'a' for the row labeling
    boardLocation = 'a';
    printf("\n");

    /* Following nested for loops print each index of the board array. It also 
        prints the characters on the side to determine location. */
    for (row = 0; row < dimension; row++) {
        printf("%c ", boardLocation);
        for (col = 0; col < dimension; col++) {
            printf("%c", board[row][col]);
        }
        printf("\n");
        boardLocation++;
    }
}

// Following method checks to see the if the board location is within the range. 
bool positionInBounds(int N, char row, char col) {
    // following variables store the int value of row and col 
    int rowNum = row;
    int colNum = col;
    // checking to see the index is found with the range of the board 
    if (rowNum >= 0 && rowNum < N && colNum >= 0 && colNum < N) {
        return true;
    } else {
        return false;
    }
}

// Method used to determine the opponent's color 
char oppositeColour(char colour) {
    char oppositeColour;
    if (colour == 'B') {
        oppositeColour = 'W';
    } else if (colour == 'W') {
        oppositeColour = 'B';
    }
    return oppositeColour;
}

// Following method checks to see if the indicated location is valid move. 
bool checkLegalInDirection(char board[26][26], int N, char row, char col, char colour, int deltaRow, int deltaCol) {
    char opponentColour; // stores the opponent's color ('B' / 'W')
    int newRow, newCol; // variables used to store the new location of row and col 
    bool countinue = true; // variable used to check if the for loops should continue to run or not.   
    bool exitStatus = false; // stores the return value of the function 
    bool positionStatus; // stores the return value of 'positionInBounds' function 
    int numRow = row - 'a', numCol = col - 'a'; // stores the numerical value of row and col

    // Determines the color of the opponent 
    opponentColour = oppositeColour(colour);

    // checking to see if the location is unoccupied and not the same index passed in the function.     
    if ((deltaRow != 0 || deltaCol != 0) && board[numRow][numCol] == 'U') {

        // checking to see if the index is with in the board range. 
        positionStatus = positionInBounds(N, (numRow + deltaRow), (numCol + deltaCol));

        // following if statement checks to see the index beside stores an opposite color
        if (positionStatus == true && board[numRow + deltaRow][numCol + deltaCol] == opponentColour) {
            newRow = numRow;
            newCol = numCol;
            /* following loop continues to check in the direction until it is
               out of bound or the same color if found. */
            while (countinue == true) {
                newRow = newRow + deltaRow; // stores the new location of the row index
                newCol = newCol + deltaCol; // stores the new location of the col index 

                // checking to see if the new position is within the range of the board. 
                positionStatus = positionInBounds(N, newRow, newCol);

                // if the following conditions are met; loop continues to run
                if (positionStatus == true && board[newRow][newCol] == opponentColour) {
                    countinue = true;
                } else {
                    countinue = false;
                }
            }

            // checking to see the index in the direction we are looking, has the same color 
            if (board[newRow][newCol] != 'U' && positionInBounds(N, newRow, newCol) == true) {
                exitStatus = true;
            } else {
                exitStatus = false;
            }
        }
    } else {
        exitStatus = false;
    }
    return exitStatus;
}

// Methods used to flip the tiles once the player has made a valid move 
bool changeBoardInfo(char board[26][26], int N, char colour, char row, char col) {
    int deltaRow, deltaCol, newRow, newCol;
    int numRow = row - 'a', numCol = col - 'a'; // numerical values of row and col
    bool infoChanged = false, // return true if any tiles can be flipped or false
            legalMoveStatus; // stores the status of legality of a move. 

    // checking through all the rows and columns surrounded by a specific index
    for (deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for (deltaCol = -1; deltaCol <= 1; deltaCol++) {

            if (deltaRow != 0 || deltaCol != 0) {

                // checking to see the legality
                legalMoveStatus = checkLegalInDirection(board, N, row, col, colour, deltaRow, deltaCol);

                if (legalMoveStatus == true) {
                    // storing the new location of new row and column 
                    newRow = numRow + deltaRow;
                    newCol = numCol + deltaCol;
                    // flipping the ties in the direction
                    while (board[newRow][newCol] != colour) {
                        board[newRow][newCol] = colour;
                        infoChanged = true;
                        newRow = newRow + deltaRow;
                        newCol = newCol + deltaCol;
                    }
                }
            }
        }
    }
    return infoChanged;
}

/* Method used by compMove to check the maximum points that can be attained by 
   the opponent. The following method return the max possible points gained by
   human player as a result of computer move. */
int maxScorePossible(char tempBoard[26][26], int N, char colour) {
    // Following variables store the information about rows and columns
    int row, col, deltaRow, deltaCol, newRow, newCol;
    // Following variables store the scores. 
    int score = 0, previousHighScore = 0;
    bool legalMoveStatus; // variable store the status of legality 

    // scanning through each index. 
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            for (deltaRow = -1; deltaRow <= 1; deltaRow++) {
                for (deltaCol = -1; deltaCol <= 1; deltaCol++) {

                    legalMoveStatus = checkLegalInDirection(tempBoard, N, row + 'a', col + 'a', colour, deltaRow, deltaCol);
                    
                    // Calculating player's maximum score by scanning in that direction
                    if (legalMoveStatus == true) {
                        newRow = row + deltaRow;
                        newCol = col + deltaCol;
                        while (tempBoard[newRow][newCol] != colour) {
                            score++;
                            newRow += deltaRow;
                            newCol += deltaCol;
                        }
                        ///////////////////////printf("HIGH SCORE: %d, ROW: %d, COL, %d\n", score, row, col);
                    }
                }
            }

            // storing only the maximum points 
            if (score > previousHighScore) {
                previousHighScore = score;
            }
            score = 0;   //reinitialized for the next index.  
        }
    }
    return previousHighScore;
}

// Method used to change information on a new board(tempBoard)
void tempChangeBoard(char board[26][26], char tempBoard[26][26], int N, char colour, int cRow, int cCol) {
    int row, col;
    // copying the information from the original board 
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            tempBoard[row][col] = board[row][col];
        }
    }
    // making changes on the board to count the possible score
    changeBoardInfo(tempBoard, N, colour, cRow + 'a', cCol + 'a');
    tempBoard[cRow][cCol] = colour; // making a possible computer move
}

// Method used to check if the index is corner or not 
bool corner(int row, int col, int N) {
    bool exitCondition = false;
    // checking for possible index vales for rows and columns. 
    if ((row == 0 && col == 0) || (row == 0 && col == N - 1)
            || (row == N - 1 && col == 0) || (row == N - 1 && col == N - 1)) {
        exitCondition = true;
    }
    return exitCondition;
}

// Methods used for computer moves every turn. 
void computerMove(char board[26][26], int N, char colour) {
    char tempBoard[26][26]; // creating another board to check one step ahead
    // following variables are used to store the rows and columns 
    int row, col, deltaRow, deltaCol, newRow, newCol;
    // following variables store the information about the score         
    int highestScoreRow = 0, highestScoreCol = 0, previousHighScore = 0,
            previousScoreDiff = 0, worstScoreDifference = 10, score = 0,
            maxPossiblePoints = 0;
    // following variables store the status of each stage of the moves. 
    bool legalMoveStatus, movePossible, greater = false, same = false, doneChecking = false;

    // scanning through each index to see a possible place to put the tile
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            for (deltaRow = -1; deltaRow <= 1; deltaRow++) {
                for (deltaCol = -1; deltaCol <= 1; deltaCol++) {

                    // checking for legality 
                    legalMoveStatus = checkLegalInDirection(board, N, row + 'a', 
                            col + 'a', colour, deltaRow, deltaCol);

                    /* checking if corner is not available to put the tile, and 
                       and it is legal move, counting the possible score in that 
                     * direction */
                    if (legalMoveStatus == true && !corner(row, col, N)) {
                        newRow = row + deltaRow;
                        newCol = col + deltaCol;
                        while (board[newRow][newCol] != colour) {
                            score++;
                            newRow += deltaRow;
                            newCol += deltaCol;
                        }
                       ////////////////////////////////////// printf("%d,     %d\n", row, col);
                        // creating a new board to make changes to it 
                        tempChangeBoard(board, tempBoard, N, colour, row, col);
                        // calculating the max possible points gained by putting in that index 
                        maxPossiblePoints = maxScorePossible(tempBoard, N, oppositeColour(colour));
                        movePossible = true;
                    } else if (legalMoveStatus) {  // capturing corner!
                        highestScoreRow = row;
                        highestScoreCol = col;
                        doneChecking = true;
                        ////////////////////////////////////////// printf("CORNER");
                    }
                }
            }

            if (!doneChecking) {
                // Appropriate index is chosen as computer score is greater
                if (movePossible && score > maxPossiblePoints && (score - maxPossiblePoints) >= previousScoreDiff) {
                    highestScoreRow = row;
                    highestScoreCol = col;
                    previousScoreDiff = score - maxPossiblePoints;
                    greater = true;
                    /////////////////////////////printf("1) HighROW: %d, HighCOL: %d\n", highestScoreRow, highestScoreCol);
                // Appropriate index is chosen as computer score results in equal benefit
                } else if (score == maxPossiblePoints && !greater && movePossible) {
                    highestScoreRow = row;
                    highestScoreCol = col;
                    same = true;
                    ////////////////////////////// printf("same %d 2) HighROW: %d, HighCOL: %d\n", same, highestScoreRow, highestScoreCol);
                // Any possible valid index is chosen
                } else if (!same && movePossible && !greater && score < maxPossiblePoints && (maxPossiblePoints - score) <= worstScoreDifference) {
                    highestScoreRow = row;
                    highestScoreCol = col;
                    worstScoreDifference = maxPossiblePoints - score;
                    ////////////////////////////////printf("3) HighROW: %d, HighCOL: %d\n", highestScoreRow, highestScoreCol);
                }
            }
            // restating the statuses for next alliteration 
            score = 0;
            movePossible = false;
        }
    }
    // printing the move made and changing the information of the current board 
    printf("Computer places %c at %c%c.\n\n", colour, (highestScoreRow + 'a'), (highestScoreCol + 'a'));
    changeBoardInfo(board, N, colour, (highestScoreRow + 'a'), (highestScoreCol + 'a'));
    board[highestScoreRow][highestScoreCol] = colour;
}

// Methods that returns the validity of a human move
bool humanMove(char board[26][26], int N, char colour, char row, char col) {
    bool validity = false;
    // checking for validity of a move 
    validity = changeBoardInfo(board, N, colour, row, col);

    if (validity == true) {
        board[row - 'a'][col - 'a'] = colour; // making a move 
    } else {
        validity = false;
    }

    return validity;
}

// Methods used to check if any moves are left for the player. 

bool availableMove(char board[26][26], int N, char colour) {
    int row, col, deltaRow, deltaCol;
    bool legalMoveStatus, exitCondition = false;

    // scanning through each index to check for available moves 
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            for (deltaRow = -1; deltaRow <= 1; deltaRow++) {
                for (deltaCol = -1; deltaCol <= 1; deltaCol++) {

                    legalMoveStatus = checkLegalInDirection(board, N, row + 'a', col + 'a', colour, deltaRow, deltaCol);

                    if (legalMoveStatus == true) {
                        exitCondition = true;
                    }
                }
            }
        }
    }
    return exitCondition;
}

// Method used to declare winner and count score. 
void declareWinner(char board[26][26], int N) {
    int row, col, blackTiles = 0, whiteTiles = 0;

    // scanning through each index and counting each colored tile
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            if (board[row][col] == 'B') {
                blackTiles++;
            } else if (board[row][col] == 'W') {
                whiteTiles++;
            }
        }
    }
    // appropriate result is printed 
    if (blackTiles > whiteTiles) {
        printf("B player wins.");
    } else if (whiteTiles > blackTiles) {
        printf("W player wins.");
    } else {
        printf("Draw!");
    }
}


/*
 * Start of C-Program - "main" method which is run right after compilation 
 * Program: Player is able to play reversi game against a smart computer.    
 */

int main(void) {
    // variables to get the user input 
    int boardDimension; char row, col;      
    char board[26][26]; // array to store the board information 
    char computerColour = 'W', playerColour = 'B'; // storing each player's color 
    // Following variables are used to store the status at each important events 
    bool computerFirst = false, validity = true;
    bool compAvailableMove = true, playerAvailableMove = true;

    initialPropmts(&boardDimension, &computerColour); // printing initial prompts
    initialBoard(board, boardDimension); // setting initial board 
    printBoard(board, boardDimension); // printing the board 

    // deciding the color for each player
    if (computerColour == 'B') {
        playerColour = 'W';
        computerFirst = true;
    }

    // Alliterating through loop until no valid move
    while (validity != false) {

        /*computer makes it's move and changes the board information. If 
          no moves are left, the compAvailableMove is set to false. */
        if (computerFirst == true) {
            if (availableMove(board, boardDimension, computerColour)) {
                computerMove(board, boardDimension, computerColour);
                printBoard(board, boardDimension);
                compAvailableMove = true;
            } else {
                compAvailableMove = false;
            }
        } else {
            /* player enters the move. Following that, validity is checked and if 
             * met, board information is changed. If no moves are left, then 
             * playerAvailableMove is set to false. */
            if (availableMove(board, boardDimension, playerColour)) {
                printf("Enter move for colour %c (RowCol): ", playerColour);
                scanf(" %c%c", &row, &col);
                validity = humanMove(board, boardDimension, playerColour, row, col);
                playerAvailableMove = true;
                // if the non-valid move is entered, game is ended. 
                if (validity == false) {
                    printf("Invalid Move.\n\n");
                    printf("%c player wins.\n\n", computerColour);
                } else {
                    printBoard(board, boardDimension); // printing the board
                }
            } else {
                playerAvailableMove = false;
            }
        }

        // Appropriate prompts are printed based on availability of each player's move
        if (!(compAvailableMove || playerAvailableMove)) {
            declareWinner(board, boardDimension);
            validity = false;
        } else if (!compAvailableMove && availableMove(board, boardDimension, playerColour)) {
            printf("%c player has no valid move.\n\n", computerColour);
        } else if (!playerAvailableMove && availableMove(board, boardDimension, computerColour)) {
            printf("%c player has no valid move.\n\n", playerColour);
        }

        // Alternating turn 
        computerFirst = !computerFirst;

    }
    return (EXIT_SUCCESS);
}