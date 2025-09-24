# cse3150_week_4_lab

## Lab 4: Building the 2048 Game in C++

### Overview
In this lab, you will implement a simplified version of the game **2048**. You will start from provided starter code and incrementally add functionality until your program passes the given test suite.

By the end, you will:
- Understand how to manipulate **2D arrays** in C++.
- Use **Standard Template Library (STL)** tools such as `std::vector`, `std::stack`, and `<algorithm>`.
- Apply **iterators**, the `auto` keyword, and adapters effectively.
- Implement game mechanics such as **shifting, merging, and undo**.
- Verify your program using **automated tests**.

Your work must be committed to a file called `solution.cpp` in your GitHub repository `cse3150 lab 4`. Your submission is complete when all tests pass.

### Game Rules Recap
The game is played on a **4 × 4 board**:
- Each turn, the player chooses a direction: left (`a`), right (`d`), up (`w`), or down (`s`).
- Tiles slide in that direction (if possible). **Adjacent tiles of the same value merge into one tile of double the value.**
- After each move, a new tile (usually a `2`) spawns in an empty cell.
  - **Note:** If you attempt to slide in a direction but every cell is already slid that way, the move doesn’t count and no new tiles spawn. Moves only count when something changes on the board.
- The player can type `u` to **undo** the last move.
- The player can type `q` to **quit**.

### Starter Files
You are given two files:
- `starter.cpp` — Skeleton code for the game (already configured to write board state to CSV).
- `test_game.py` — Pytest tests that validate your implementation.

You will build your solution by editing and renaming `starter.cpp` into `solution.cpp`.

### Step 1: Representing the Board
The board is a **4 × 4 grid of integers**. In C++, this can be stored as:
```cpp
std::vector<std::vector<int>> board(4, std::vector<int>(4, 0));
```
**Key C++ tools for this step:**
- Use `std::vector` instead of raw arrays for flexibility.
- Access rows with `board[i]` and individual cells with `board[i][j]`.
- Use range-based for loops with `auto&` to traverse rows and cells.

> At the bottom of the `print_board` function there is a function call that writes the board to the CSV. This is already implemented for you and should not be changed.

### Step 2: Shifting and Compressing Tiles
To shift tiles left, right, up, or down, we first need to **compress them by removing zeros**.
- Recommended STL approach: use the `copy_if` function.
- Start with an empty vector `compressed`, and then use `copy_if` and a back inserter with a lambda to copy the row to your compressed vector.
- `copy_if` also allows filtering as you copy. Pass a lambda to filter out zeros.
```cpp
std::vector<int> compressed;
```
This uses:
- `std::copy_if` from `<algorithm>` to filter tiles.
- `std::back_inserter` from `<iterator>` to append efficiently.
- A lambda expression to keep only non-zero entries.

After compression, **pad with zeros** to return the row to length 4.

### Step 3: Merging Tiles
- When two adjacent tiles are equal:
  - Multiply the first value by two.
  - Set the second value to zero.
- Then call the **compress step** again to remove zeros created by merging.
- This ensures the **no double merge rule** is respected.

### Step 3: Moving the Tiles Left
- Start with move left (easiest).
- For each row in your board:
  1. Compress the row.
  2. Merge the row.
  3. Set the row to this new merged value.
- If any new row is different from the old row, the move was valid → return `true`.
- Otherwise, if the board didn’t change → return `false`.

> Try the game out now and try moving left.

### Step 4: Moving the Tiles Right
- Reverse the vector using `std::reverse`.
- Compress and merge the row.
- Reverse the row back to its original direction.
- Implement the rest of the logic similar to moving left.

> Test both left and right moves.

### Step 5: Spawning New Tiles
After each valid move:
- Collect empty positions in a vector of pairs.
- Use `std::mt19937` and `std::uniform_int_distribution` from `<random>` to select a random empty cell.
- Assign a `2` (90% of the time) or `4` (10%).

> This syntax-heavy step is a good place to use AI for help.

### Step 6: Moving the Tiles Up and Down
- Implement similar logic as moving left, but work with columns.
- Create a vector representing the column.
- Implement move up and move down.

> You should now be able to move in any direction!

### Step 7: Implementing Undo
- Use `std::stack` from `<stack>` to store previous board states.
- Before applying a move, push a copy of the board.
- On undo (`u`):
  - Restore from the stack if not empty.
  - Print the board and continue.

### Step 8: Implementing Score
- Modify the stub for the score function to take a template or reference class `Board`.
- Calculate the score by adding up every tile on the board.

### Step 9: Putting It All Together
In `main()`:
1. Print the current board and write it to CSV.
2. Read a character command from input.
3. Use a switch statement for actions (`a`, `d`, `w`, `s`, `u`, `q`).
4. For moves:
   - Push the board to history.
   - Compress, merge, compress again.
   - Spawn a new tile.
5. For undo: restore from the stack.
6. For quit: break the loop.

### Testing Your Code
Run the provided tests with:
```bash
pytest test_game.py
```
- Your code is correct when all tests pass.
- Push your final version to GitHub as `solution.cpp` under the repo `cse3150 week 4 lab`.
