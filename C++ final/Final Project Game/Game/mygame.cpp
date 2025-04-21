#include <iostream>
#include <vector>
#include <fstream>
#include <conio.h>   // For _getch() - Windows specific
#include <cstdlib>  // For rand(), srand(), system()
#include <ctime>  // For time()
#include <windows.h>  // For Set Console Text Attribute, COORD, Set Console Cursor Position, Sleep() - Windows specific
#include <cctype>  // For tolower()

using namespace std;


// --- Constants ---
const char WALL = 219;  // Character for walls (solid block)
const char PATH = ' ';
const char PLAYER = 'P';
const char EXIT = 'E';
const char COLLECTIBLE = '*';
const char ENEMY = 'X';
const int MAZE_SIZE = 10;
const int MAX_MOVES = 30;
const int MAX_LEVEL = 4;
const string SCORE_FILE = "highscore.txt"; // File to store the highest score


// --- Console Helper Functions ---
void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void goToTopLeft()
{
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// --- MazeGame Class Definition ---
class MazeGame
{
private:
    vector<vector<char>> maze;
    pair<int, int> playerPos;
    int score;
    int movesLeft;
    int level;
    int highestScore;

public:    //constructor
    MazeGame() : level(1), score(0), highestScore(0), movesLeft(MAX_MOVES), playerPos({1, 1})
    {
        srand(time(0));
        loadHighScore(); // Load from file at start
    }

    void generateMaze();
    void displayMaze();
    int playLevel();
    void showMenu();
    void showGameOverMessage();
    void showGameWonMessage();

    // --- File I/O Functions ---
    void loadHighScore()
    {
        ifstream inFile(SCORE_FILE);
        if (inFile >> highestScore)
        {
            // Loaded successfully
        }
        else
        {
            highestScore = 0;
        }
        inFile.close();
    }

    void saveHighScore()
    {
        ofstream outFile(SCORE_FILE);
        if (outFile)
        {
            outFile << highestScore;
        }
        outFile.close();
    }
};

void MazeGame::generateMaze()  // Initialize maze with paths
{
    maze = vector<vector<char>>(MAZE_SIZE, vector<char>(MAZE_SIZE, PATH));
    // Create outer walls
    for (int i = 0; i < MAZE_SIZE; i++)
    {
        maze[0][i] = WALL;
        maze[MAZE_SIZE - 1][i] = WALL;
        maze[i][0] = WALL;
        maze[i][MAZE_SIZE - 1] = WALL;
    }
 // Define start and exit positions
    pair<int, int> start = {1, 1};
    pair<int, int> exitPos = {MAZE_SIZE - 2, MAZE_SIZE - 2};

    // Place internal walls randomly
    int numWalls = 10 + level;
    for (int i = 0; i < numWalls; i++)
    {
        int x, y;
        do
        {  // Generate random coordinates within the inner maze area

            x = rand() % (MAZE_SIZE - 2) + 1;
            y = rand() % (MAZE_SIZE - 2) + 1;
        } while (make_pair(y, x) == start || make_pair(y, x) == exitPos || maze[y][x] != PATH);
        maze[y][x] = WALL;
    }

    // Place collectibles randomly
    int numCollectibles = 3 + level;
    for (int i = 0; i < numCollectibles; i++)
    {
        int x, y;
        do
        {
            x = rand() % (MAZE_SIZE - 2) + 1;
            y = rand() % (MAZE_SIZE - 2) + 1;
        } while (make_pair(y, x) == start || make_pair(y, x) == exitPos || maze[y][x] != PATH);
        maze[y][x] = COLLECTIBLE;
    }

    // Place enemies randomly
    int numEnemies = 1 + level / 2;
    for (int i = 0; i < numEnemies; i++)
    {
        int x, y;
        do
        {
            x = rand() % (MAZE_SIZE - 2) + 1;
            y = rand() % (MAZE_SIZE - 2) + 1;
        } while (make_pair(y, x) == start || make_pair(y, x) == exitPos || maze[y][x] != PATH);
        maze[y][x] = ENEMY;
    }

    playerPos = start;
    maze[start.first][start.second] = PATH;
    maze[exitPos.first][exitPos.second] = EXIT;  // Place to exit
}

void MazeGame::displayMaze()
{
    goToTopLeft();
    setColor(15);
    cout << "Score: " << score << " | Moves Left: " << movesLeft << " | Highest Score: " << highestScore << "\n";
    cout << "Level: " << level << "/" << MAX_LEVEL << "\n";
    cout << "----------------------\n";  //seperator

    for (int y = 0; y < MAZE_SIZE; y++)
    {
        for (int x = 0; x < MAZE_SIZE; x++)
        {
            if (playerPos.first == y && playerPos.second == x)  // Check if the current cell is the player's position
            {
                setColor(11);
                cout << PLAYER;
            }
            else
            {
                char cell = maze[y][x];
                switch (cell)
                {
                case WALL:  // Alternate colors for walls for texture

                    setColor((x + y) % 2 == 0 ? 9 : 1);  // Bright Blue / Dark Blue

                    cout << WALL;
                    break;
                case EXIT:
                    setColor(10);  // Bright Green for Exit
                    cout << EXIT;
                    break;
                case COLLECTIBLE:
                    setColor(14);  // Yellow for Collectible
                    cout << COLLECTIBLE;
                    break;
                case ENEMY:
                    setColor(12);  // Red for Enemy
                    cout << ENEMY;
                    break;
                case PATH:
                default:
                    setColor(7);  // Gray for Path
                    cout << PATH;
                    break;
                }
            }
            cout << " ";   // Add space for better visual separation
        }
        cout << '\n'; // Newline
    }
    setColor(15);
    cout << "----------------------\n";
    cout << "Use W,A,S,D. Q to Quit Level.\n";
}

int MazeGame::playLevel()
{
    generateMaze();
    movesLeft = MAX_MOVES + (level - 1) * 2; // More moves for higher levels


    while (movesLeft > 0)
    {
        displayMaze();
        char move = _getch();
        move = tolower(move);  // Convert to lowercase for case-insensitivity
        if (move == 'q')
        {
            goToTopLeft();
            cout << "\nQuitting level...\n";
            Sleep(1000);  // Pause briefly
            saveHighScore(); // Save on quit
            return 0;
        }

        pair<int, int> newPos = playerPos;
        if (move == 'w') newPos.first--;
        else if (move == 's') newPos.first++;
        else if (move == 'a') newPos.second--;
        else if (move == 'd') newPos.second++;
        else continue;

        if (newPos.first < 0 || newPos.first >= MAZE_SIZE || newPos.second < 0 || newPos.second >= MAZE_SIZE)
        {
            continue;  // Invalid move, skip rest of the loop

        }

        char destinationCell = maze[newPos.first][newPos.second];

      // Collision Check: Wall

        if (destinationCell == WALL) continue;

        if (destinationCell == ENEMY)
        {
            playerPos = newPos;
            displayMaze();
            setColor(12);
            cout << "\nOops! You got caught by an enemy! Game Over!" << endl;
            setColor(15);
            _getch();
            saveHighScore(); // Save on game over
            return 0;  // Return 0 for game over

        }

        if (destinationCell == COLLECTIBLE)
        {
            score += 10;
            if (score > highestScore) highestScore = score; // Update highest score if needed

            maze[newPos.first][newPos.second] = PATH;
        }

        if (destinationCell == EXIT)
        {
            playerPos = newPos;
            displayMaze();
            setColor(10);
            cout << "\nCongrats! Level " << level << " Completed!" << endl;
            setColor(15);
            if (score > highestScore) highestScore = score;
            saveHighScore(); // Save after successful level
            _getch();

            if (level == MAX_LEVEL)  // Check if this was the last level

            {
                return 2;
            }
            else
            {
                level++;
                return 1;
            }
        }
         // If the move is valid (not wall, not enemy, not exit yet) update position

        playerPos = newPos;
        movesLeft--;
    }
     // If the loop finishes, the player ran out of moves

    displayMaze();  // Show final state

    setColor(12);
    cout << "\nOut of moves! Game Over!" << endl;
    setColor(15);
    _getch();
    saveHighScore(); // Save on game over
    return 0;
}

void MazeGame::showMenu()
{
    system("cls");   // Clear the console screen (Windows specific)

    setColor(11);
    cout << " Welcome to the Amazing 2D Maze Game! \n";
    setColor(14);
    cout << "=======================================\n";
    setColor(15);
    cout << " Collect '*' for points, avoid 'X' enemies!\n";
    cout << " Reach the exit 'E'. Max Level: " << MAX_LEVEL << "\n";
    cout << "---------------------------------------\n";
    cout << " Controls: Use W A S D to move.\n";
    cout << "           Use Q to Quit current level.\n";
    cout << " Press 'P' to Play\n";
    cout << " Press 'E' to Exit\n";
    cout << "=======================================\n";
    setColor(15);

    char choice;
    do
    {
        choice = _getch();  // Get character input directly

        choice = tolower(choice);
    } while (choice != 'p' && choice != 'e');

    if (choice == 'e')
    {
        setColor(10);
        cout << "\nThanks for playing! See you next time! " << endl;
        setColor(15);
        saveHighScore(); // Save before exiting
        exit(0);   // Terminate the program
    }
// If 'p' was pressed, reset game state for a new game
    score = 0;
    level = 1;
}   // highestScore is intentionally NOT reset here, it persists across games in one run


void MazeGame::showGameOverMessage()
{
    system("cls");  // Clear screen

    setColor(12);
    cout << "\n=====================================" << endl;
    cout << "             GAME OVER!              " << endl;
    cout << "=====================================" << endl;
    setColor(15);
    cout << "\n    Your final score: " << score << endl;
    cout << "    Highest score achieved: " << highestScore << endl;
    cout << "\nBetter luck next time!" << endl;
    cout << "\nPress any key return to menu." << endl;
    _getch();
}

void MazeGame::showGameWonMessage()
{
    system("cls");
    setColor(10);
    cout << "\n\n*************************************" << endl;
    cout << "*      CONGRATULATIONS! YOU WON!    *" << endl;
    cout << "*   You completed all " << MAX_LEVEL << " levels!   *" << endl;
    cout << "*************************************" << endl;
    setColor(14);
    cout << "\n   Your final score: " << score << endl;
    cout << "   Highest score during game: " << highestScore << endl;
    setColor(11);
    cout << "\n   Well done, Maze Master! Thanks for playing! " << endl;
    setColor(15);
    cout << "\nPress any key return to menu." << endl;
    _getch();
}

// --- Main Function ---
int main()
{
    MazeGame game;

    while (true)
    {
        game.showMenu();
        while (true)  // Main application loop (allows playing multiple games)

        {
            int levelResult = game.playLevel();   // Play one level

            if (levelResult == 1)  // Level completed, continue to the next level

            continue;

            else if (levelResult == 2)
            {
                game.showGameWonMessage();
                break;
            }
            else
            {
                game.showGameOverMessage();
                break;   // Exit the inner loop (back to main menu)

            }
        }
    }
     // This return statement might not be reached due to the infinite outer loop
    // unless an exit() call happens (like in showMenu).
    return 0;
}
