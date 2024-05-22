#include <iostream>
#include <vector>
#include <limits>

class TicTacToe {
public:
    TicTacToe(int size, int winCondition, int maxDepth)
        : boardSize(size), winCondition(winCondition), maxDepth(maxDepth), board(size, std::vector<char>(size, ' ')) {}

    void playGame() {
        bool isPlayerTurn = true;
        int totalMoves = 0;
        while (true) {
            printBoard();
            if (isPlayerTurn) {
                int row, col;
                std::cout << "Gracz X, wybierz wiersz i kolumne (1-" << boardSize << "): ";
                std::cin >> row >> col;
                row--;
                col--;
                if (row < 0 || row >= boardSize || col < 0 || col >= boardSize || board[row][col] != ' ') {
                    std::cout << "Niepoprawny ruch, sprobuj ponownie." << std::endl;
                    continue;
                }
                board[row][col] = 'X';
            } else {
                std::cout << "Komputer wykonuje ruch (O)..." << std::endl;
                auto move = findBestMove();
                board[move.first][move.second] = 'O';
            }
            totalMoves++;
            char currentPlayer = isPlayerTurn ? 'X' : 'O';
            if (checkWin(currentPlayer)) {
                printBoard();
                std::cout << "Gracz " << currentPlayer << " wygrywa!" << std::endl;
                break;
            }
            if (totalMoves == boardSize * boardSize) {
                printBoard();
                std::cout << "Remis!" << std::endl;
                break;
            }
            isPlayerTurn = !isPlayerTurn;
        }
    }

private:
    int boardSize;
    int winCondition;
    int maxDepth;
    std::vector<std::vector<char>> board;

    void printBoard() {
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                std::cout << board[i][j];
                if (j < boardSize - 1) std::cout << "|";
            }
            std::cout << std::endl;
            if (i < boardSize - 1) {
                for (int j = 0; j < boardSize; j++) {
                    std::cout << "-";
                    if (j < boardSize - 1) std::cout << "+";
                }
                std::cout << std::endl;
            }
        }
    }

    bool checkWin(char player) {
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if ((checkDirection(i, j, player, 1, 0)) || // Horizontal
                    (checkDirection(i, j, player, 0, 1)) || // Vertical
                    (checkDirection(i, j, player, 1, 1)) || // Diagonal 
                    (checkDirection(i, j, player, 1, -1))) { // Diagonal 
                    return true;
                }
            }
        }
        return false;
    }

    bool checkDirection(int row, int col, char player, int dRow, int dCol) {
        int count = 0;
        for (int i = 0; i < winCondition; i++) {
            int r = row + i * dRow;
            int c = col + i * dCol;
            if (r >= 0 && r < boardSize && c >= 0 && c < boardSize && board[r][c] == player) {
                count++;
                if (count == winCondition) return true;
            } else {
                break;
            }
        }
        return false;
    }

    int minimax(int depth, bool isMax) {
        if (checkWin('O')) return 10 - depth;
        if (checkWin('X')) return depth - 10;
        if (isBoardFull() || depth >= maxDepth) return 0;

        if (isMax) {
            int best = std::numeric_limits<int>::min();
            for (int i = 0; i < boardSize; i++) {
                for (int j = 0; j < boardSize; j++) {
                    if (board[i][j] == ' ') {
                        board[i][j] = 'O';
                        best = std::max(best, minimax(depth + 1, !isMax));
                        board[i][j] = ' ';
                    }
                }
            }
            return best;
        } else {
            int best = std::numeric_limits<int>::max();
            for (int i = 0; i < boardSize; i++) {
                for (int j = 0; j < boardSize; j++) {
                    if (board[i][j] == ' ') {
                        board[i][j] = 'X';
                        best = std::min(best, minimax(depth + 1, !isMax));
                        board[i][j] = ' ';
                    }
                }
            }
            return best;
        }
    }

    std::pair<int, int> findBestMove() {
        int bestVal = std::numeric_limits<int>::min();
        std::pair<int, int> bestMove = {-1, -1};
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';
                    int moveVal = minimax(0, false);
                    board[i][j] = ' ';
                    if (moveVal > bestVal) {
                        bestMove = {i, j};
                        bestVal = moveVal;
                    }
                }
            }
        }
        return bestMove;
    }

    bool isBoardFull() {
        for (const auto& row : board) {
            for (char cell : row) {
                if (cell == ' ') {
                    return false;
                }
            }
        }
        return true;
    }
};

int main() {
    int size, winCondition, maxDepth;
    std::cout << "Podaj rozmiar planszy: ";
    std::cin >> size;
    std::cout << "Podaj ilosc znakow w rzedzie potrzebnych do wygranej: ";
    std::cin >> winCondition;
    std::cout << "Podaj maksymalna glebokosc przeszukiwania: ";
    std::cin >> maxDepth;

    TicTacToe game(size, winCondition, maxDepth);
    game.playGame();

    return 0;
}
