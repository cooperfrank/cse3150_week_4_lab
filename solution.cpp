#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

void write_board_csv(const vector<vector<int>>& board, bool first) {
    ios_base::openmode mode = ios::app;
    if (first) mode = ios::trunc;
    ofstream fout("game_output.csv", mode);
    if (!fout) return;
    for (int r=0;r<4;r++){
        for (int c=0;c<4;c++){
            fout<<board[r][c];
            if (!(r==3 && c==3)) fout<<",";
        }
    }
    fout<<"\n";
}

void print_board(const vector<vector<int>>& board, bool first) {
    // TODO: implement print_board here
    for (auto& row : board) {
        for (auto& cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }

    write_board_csv(board, first);
}

void spawn_tile(vector<vector<int>>& board) {
    vector<pair<int,int>> empty_cells;

    // Collect all empty positions
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (board[r][c] == 0) {
                empty_cells.push_back({r, c});
            }
        }
    }

    // If there are no empty cells, do nothing
    if (empty_cells.empty()) return;

    // Seed random generator (do this once in main() ideally)
    // srand(time(nullptr)); // <-- call this once at program start

    // Pick a random empty cell
    int idx = rand() % empty_cells.size();
    int r = empty_cells[idx].first;
    int c = empty_cells[idx].second;

    // Spawn 2 (90%) or 4 (10%)
    board[r][c] = (rand() % 10 == 0) ? 4 : 2;
}

// TODO: Compress a row, remove zeroes, and then pad with zeroes at the end
std::vector<int> compress_row(const std::vector<int>& row) {
    std::vector<int> compressed;

    std::copy_if(
        row.begin(),
        row.end(),
        std::back_inserter(compressed),
        [](int value) { return value != 0; }
    );

    compressed.resize(4, 0);

    return compressed;
}

// TODO: Merge a row (assumes the row is already compressed)
std::vector<int> merge_row(std::vector<int> row) {
    for (int i = 0; i < 3; i++) {
        if (row[i] == row[i+1]) {
            row[i] *= 2;
            row[i+1] = 0;
        }
    }

    row = compress_row(row);

    return row;
}



// TODO: use copy_if and iterators
bool move_left(vector<vector<int>>& board) {
    bool changed = false;

    for (auto& row : board) {
        auto original = row;

        row = compress_row(row);
        row = merge_row(row);

        if (row != original) {
            changed = true;
        }
    }

    return changed;
}


// TODO: use reverse iterators
bool move_right(vector<vector<int>>& board) {
    bool changed = false;

    for (auto& row : board) {
        auto original = row;

        std::reverse(row.begin(), row.end());

        row = compress_row(row);
        row = merge_row(row);

        std::reverse(row.begin(), row.end());

        if (row != original) {
            changed = true;
        }
    }

    return changed;
}

// TODO: use column traversal
bool move_up(vector<vector<int>>& board) {
    bool changed = false;

    for (int i = 0; i < 4; i++) {
        std::vector<int> col(4);

        for (int j = 0; j < 4; j++) {
            col[j] = board[j][i];
        }

        auto original = col;

        col = compress_row(col);
        col = merge_row(col);

        for (int j = 0; j < 4; j++) {
            board[j][i] = col[j];
        }

        if (col != original) {
            changed = true;
        }
    }


    return changed;
}


// TODO: use column traversal with reverse
bool move_down(vector<vector<int>>& board) {
    bool changed = false;

    for (int i = 0; i < 4; i++) {
        std::vector<int> col(4);

        for (int j = 0; j < 4; j++) {
            col[j] = board[j][i];
        }

        auto original = col;

        std::reverse(col.begin(), col.end());

        col = compress_row(col);
        col = merge_row(col);

        std::reverse(col.begin(), col.end());

        for (int j = 0; j < 4; j++) {
            board[j][i] = col[j];
        }

        if (col != original) {
            changed = true;
        }
    }


    return changed;
}

int main(){
    srand(time(nullptr));
    vector<vector<int>> board(4, vector<int>(4,0));
    spawn_tile(board);
    spawn_tile(board);

    stack<vector<vector<int>>> history;
    bool first=true;

    while(true){
        print_board(board, first);
        first=false;
        cout<<"Move (w=up, a=left, s=down, d=right), u=undo, q=quit: ";
        char cmd;
        if (!(cin>>cmd)) break;
        if (cmd=='q') break;

        if (cmd=='u') {
            // TODO: get the history and print the board and continue
            if (!history.empty()) {
                board = history.top();
                history.pop();
                print_board(board, false);
            }
        }

        vector<vector<int>> prev = board;
        bool moved=false;
        if (cmd=='a') moved=move_left(board);
        else if (cmd=='d') moved=move_right(board);
        else if (cmd=='w') moved=move_up(board);
        else if (cmd=='s') moved=move_down(board);

        if (moved) {
            // TODO: Store the previous state here!
            history.push(prev);
            spawn_tile(board);
        }
    }
    return 0;
}
