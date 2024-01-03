#include <iostream>
#include <vector>
#include <chrono>

bool HAS_CONFLICTS;
int queensCount;
int* queens;
int* rows;
int* diagonal1;
int* diagonal2;


void printBoard() {
    for (int i = 0; i < queensCount; i++) {
        for (int j = 0; j < queensCount; j++) {
            if (queens[j] == i)
                std::cout << "* ";
            else std::cout << "_ ";
        }
        std::cout << std::endl;
    }
}

void insertQueens() {
    int column = 1;
    for (int row = 0; row < queensCount; row++) {
        queens[column] = row;
        rows[row]++;
        diagonal1[column - row + queensCount - 1]++;
        diagonal2[column + row]++;
        column += 2;
        if (column >= queensCount)
            column = 0;
    }
}

void updateState(int row, int column) {
    int previousRow = queens[column];
    rows[previousRow]--;
    diagonal1[column - row + queensCount - 1]++;
    diagonal2[column + row]++;
    queens[column] = row;
    rows[row]++;
    diagonal1[column - row + queensCount - 1]++;
    diagonal2[column + row]++;
}

int maxConflictColumn() {
    int max = -1;
    std::vector<int> maxConflictColumns;
    int currentRow;
    int currentConflict;
    for (int currentColumn = 0; currentColumn < queensCount; currentColumn++) {
        currentRow = queens[currentColumn];
        currentConflict = rows[currentRow]
            + diagonal1[currentColumn - currentRow + queensCount - 1]
            + diagonal2[currentColumn + currentRow]
            - 3;
        if (currentConflict == max)
            maxConflictColumns.push_back(currentColumn);
        else if (currentConflict > max) {
            max = currentConflict;
            maxConflictColumns.clear();
            maxConflictColumns.push_back(currentColumn);
        }
    }
    if (max == 0)
        HAS_CONFLICTS = false;
    return maxConflictColumns[rand() % maxConflictColumns.size()];
}

int minConflictRow(int column) {
    int min = queensCount + 1;
    std::vector<int> minConflictRows;
    int currentConflict;
    for (int currentRow = 0; currentRow < queensCount; currentRow++) {
        if (queens[column] == currentRow)
            currentConflict = rows[currentRow]
            + diagonal1[column - currentRow + queensCount - 1]
            + diagonal2[column + currentRow]
            - 3;
        else
            currentConflict = rows[currentRow]
            + diagonal1[column - currentRow + queensCount]
            + diagonal2[column + currentRow];

        if (currentConflict == min)
            minConflictRows.push_back(currentRow);
        else if (currentConflict < min) {
            min = currentConflict;
            minConflictRows.clear();
            minConflictRows.push_back(currentRow);
        }
    }

    return minConflictRows[rand() % minConflictRows.size()];
}

void solve() {
    int iterator = 0;
    int k = 1;
    int column, row;

    while (iterator++ <= k * queensCount) {
        column = maxConflictColumn();
        if (!HAS_CONFLICTS)
            break;
        row = minConflictRow(column);
        updateState(row, column);
    }

    if (HAS_CONFLICTS)
        solve();
}

int main() {
    std::cin >> queensCount;
    queens = new int[queensCount];
    rows = new int[queensCount] {0};
    diagonal1 = new int[2 * queensCount - 1] { 0 };
    diagonal2 = new int[2 * queensCount - 1] { 0 };

    HAS_CONFLICTS = true;

    auto start = std::chrono::system_clock::now();



    insertQueens();
    solve();

    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> durationInSeconds = end - start;

    if(queensCount<100)
        printBoard();

    std::cout << "Time for finding path: " << durationInSeconds.count() << std::endl;


    delete[] queens;
    delete[] rows;
    delete[] diagonal1;
    delete[] diagonal2;

    return 0;
}
