// Task 2: Conway's Game of Life
// User chooses initial state (preset or custom cells), number of generations, and watches evolution.
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;

const int ROWS = 12, COLS = 24;
using Grid = vector<vector<bool>>;

void display(const Grid &g, int gen) {
    cout << "Generation " << gen << "\n+" << string(COLS, '-') << "+\n";
    for (auto &row : g) {
        cout << "|";
        for (bool c : row) cout << (c ? '#' : ' ');
        cout << "|\n";
    }
    cout << "+" << string(COLS, '-') << "+\n";
}

int neighbors(const Grid &g, int r, int c) {
    int n = 0;
    for (int dr = -1; dr <= 1; dr++)
        for (int dc = -1; dc <= 1; dc++) {
            if (!dr && !dc) continue;
            int rr = (r + dr + ROWS) % ROWS, cc = (c + dc + COLS) % COLS; // wrap-around edges
            n += g[rr][cc];
        }
    return n;
}

Grid step(const Grid &g) {
    Grid next(ROWS, vector<bool>(COLS, false));
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++) {
            int n = neighbors(g, r, c);
            next[r][c] = g[r][c] ? (n == 2 || n == 3) : (n == 3);
        }
    return next;
}

void place(Grid &g, const vector<pair<int,int>> &cells, int r0, int c0) {
    for (auto &p : cells) g[(r0 + p.first) % ROWS][(c0 + p.second) % COLS] = true;
}

int main() {
    Grid g(ROWS, vector<bool>(COLS, false));
    cout << "=== CONWAY'S GAME OF LIFE ===\nGrid: " << ROWS << "x" << COLS << " (edges wrap)\n"
         << "Initial state:\n 1. Glider\n 2. Blinker\n 3. Block + Blinker\n 4. Custom cells\nChoice: ";
    int ch; cin >> ch;
    if (ch == 1) place(g, {{0,1},{1,2},{2,0},{2,1},{2,2}}, 1, 1);
    else if (ch == 2) place(g, {{0,0},{0,1},{0,2}}, 5, 10);
    else if (ch == 3) { place(g, {{0,0},{0,1},{1,0},{1,1}}, 2, 3); place(g, {{0,0},{0,1},{0,2}}, 7, 14); }
    else {
        int k; cout << "How many live cells? "; cin >> k;
        cout << "Enter " << k << " cells as: row col (0-" << ROWS - 1 << ", 0-" << COLS - 1 << ")\n";
        while (k--) { int r, c; cin >> r >> c; if (r >= 0 && r < ROWS && c >= 0 && c < COLS) g[r][c] = true; }
    }
    int gens, delay;
    cout << "Number of generations to run: "; cin >> gens;
    cout << "Delay between frames in ms (0 for none): "; cin >> delay;

    cout << "\n";
    display(g, 0);
    for (int i = 1; i <= gens; i++) {
        g = step(g);
        if (delay > 0) this_thread::sleep_for(chrono::milliseconds(delay));
        cout << "\n";
        display(g, i);
    }
    cout << "Simulation complete.\n";
}
