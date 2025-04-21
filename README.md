# 2D Maze Game in C++

A fun and challenging terminal-based 2D maze game built using **C++** and **Windows-specific console libraries**. The player navigates through levels collecting items, avoiding enemies, and racing to the exit with limited moves!

---

## Game Features

- 10x10 Dynamic Maze Generation
-  Collectibles: Gain points by collecting `*`
-  Enemies: Avoid `X` or the game ends!
-  Exit Portal: Reach the `E` to finish each level
-  Multiple Levels: Progressive difficulty up to **4 levels**
-  Move Limit: Each level gives you a limited number of moves
-  High Score System: Saves your best score to a file (`highscore.txt`) even after the game exits!

---

##  Technologies Used

- **C++** (Standard features + `<conio.h>`, `<windows.h>`)
- Console Graphics via ASCII
- File I/O for saving high scores
- Real-time keyboard input with `_getch()`

---

##  How to Run

>  **Windows only** (due to usage of `<conio.h>` and `<windows.h>`)

1. Clone or download this repository.
2. Compile using a C++ compiler like **g++** or **MSVC**.

### Example (Using g++):
```bash
g++ maze_game.cpp -o MazeGame
./MazeGame
