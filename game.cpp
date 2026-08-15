#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <memory>
using namespace std;

enum PieceType { EMPTY, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
enum Color { WHITE, BLACK };

class ChessPiece {
public:
    PieceType type;
    Color color;
    bool hasMoved;

    ChessPiece(PieceType t = EMPTY, Color c = WHITE, bool moved = false)
        : type(t), color(c), hasMoved(moved) {}

    bool isEmpty() const { return type == EMPTY; }

    char getSymbol() const {
        if (type == EMPTY) return ' ';
        char symbols[] = {' ', 'P', 'N', 'B', 'R', 'Q', 'K'};
        char sym = symbols[type];
        return color == WHITE ? sym : tolower(sym);
    }

    string toString() const {
        if (type == EMPTY) return "   ";
        char symbols[] = {' ', 'P', 'N', 'B', 'R', 'Q', 'K'};
        return string(1, (color == WHITE ? 'W' : 'B')) + symbols[type];
    }
};

class ChessBoard {
private:
    vector<vector<ChessPiece>> board;
    Color currentTurn;
    bool whiteKingSafe, blackKingSafe;

    void initBoard() {
        board.assign(8, vector<ChessPiece>(8, ChessPiece(EMPTY, WHITE)));

        auto placePiece = [this](int row, int col, PieceType type, Color color) {
            board[row][col] = ChessPiece(type, color);
        };

        // Set up pawns
        for (int col = 0; col < 8; col++) {
            placePiece(1, col, PAWN, WHITE);
            placePiece(6, col, PAWN, BLACK);
        }

        // Set up pieces
        for (int row : {0, 7}) {
            Color color = (row == 0) ? WHITE : BLACK;
            placePiece(row, 0, ROOK, color);
            placePiece(row, 1, KNIGHT, color);
            placePiece(row, 2, BISHOP, color);
            placePiece(row, 3, QUEEN, color);
            placePiece(row, 4, KING, color);
            placePiece(row, 5, BISHOP, color);
            placePiece(row, 6, KNIGHT, color);
            placePiece(row, 7, ROOK, color);
        }

        currentTurn = WHITE;
    }

    bool isValidPosition(int row, int col) const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    bool isPathClear(int fromRow, int fromCol, int toRow, int toCol) const {
        int rowStep = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0;
        int colStep = (toCol > fromCol) ? 1 : (toCol < fromCol) ? -1 : 0;

        int row = fromRow + rowStep;
        int col = fromCol + colStep;
        while (row != toRow || col != toCol) {
            if (!board[row][col].isEmpty()) return false;
            row += rowStep;
            col += colStep;
        }
        return true;
    }

    bool isKingInCheck(Color color) const {
        int kingRow = -1, kingCol = -1;
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if (board[r][c].type == KING && board[r][c].color == color) {
                    kingRow = r;
                    kingCol = c;
                    break;
                }
            }
            if (kingRow != -1) break;
        }

        Color opponent = (color == WHITE) ? BLACK : WHITE;
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if (board[r][c].color == opponent) {
                    if (isLegalMoveWithoutKingCheck(r, c, kingRow, kingCol, color)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool isLegalMoveWithoutKingCheck(int fromRow, int fromCol, int toRow, int toCol, Color color) const {
        if (!isValidPosition(fromRow, fromCol) || !isValidPosition(toRow, toCol)) return false;
        ChessPiece piece = board[fromRow][fromCol];
        if (piece.isEmpty() || piece.color != color) return false;
        if (board[toRow][toCol].color == color) return false;

        int dr = toRow - fromRow;
        int dc = toCol - fromCol;
        int adr = abs(dr), adc = abs(dc);

        switch (piece.type) {
            case PAWN: {
                int dir = (color == WHITE) ? 1 : -1;
                if (dr == dir && dc == 0 && board[toRow][toCol].isEmpty()) return true;
                if (dr == 2 * dir && dc == 0 && board[fromRow + dir][fromCol].isEmpty() && board[toRow][toCol].isEmpty() && !piece.hasMoved) return true;
                if (dr == dir && adc == 1 && !board[toRow][toCol].isEmpty()) return true;
                return false;
            }
            case KNIGHT:
                return (adr == 2 && adc == 1) || (adr == 1 && adc == 2);
            case BISHOP:
                return adr == adc && isPathClear(fromRow, fromCol, toRow, toCol);
            case ROOK:
                return (adr == 0 || adc == 0) && isPathClear(fromRow, fromCol, toRow, toCol);
            case QUEEN:
                return (adr == adc || adr == 0 || adc == 0) && isPathClear(fromRow, fromCol, toRow, toCol);
            case KING:
                return (adr <= 1 && adc <= 1) && (adr + adc > 0);
            default:
                return false;
        }
    }

public:
    ChessBoard() {
        initBoard();
    }

    void display() const {
        cout << "  a b c d e f g h\n";
        cout << "  -----------------\n";
        for (int row = 7; row >= 0; row--) {
            cout << row + 1 << "|";
            for (int col = 0; col < 8; col++) {
                char symbol = board[row][col].getSymbol();
                if (board[row][col].isEmpty())
                    cout << (((row + col) % 2 == 0) ? " █" : " ░");
                else
                    cout << " " << symbol;
                if (col == 7) cout << " |";
            }
            cout << row + 1 << "\n";
        }
        cout << "  -----------------\n";
        cout << "  a b c d e f g h\n";
        cout << "Turn: " << (currentTurn == WHITE ? "White" : "Black") << "\n";
    }

    bool makeMove(const string& from, const string& to) {
        int fromRow = from[1] - '1';
        int fromCol = from[0] - 'a';
        int toRow = to[1] - '1';
        int toCol = to[0] - 'a';

        if (!isValidPosition(fromRow, fromCol) || !isValidPosition(toRow, toCol)) {
            cout << "Invalid position!\n";
            return false;
        }

        ChessPiece piece = board[fromRow][fromCol];
        if (piece.isEmpty() || piece.color != currentTurn) {
            cout << "Not your piece!\n";
            return false;
        }

        if (!isLegalMoveWithoutKingCheck(fromRow, fromCol, toRow, toCol, currentTurn)) {
            cout << "Illegal move!\n";
            return false;
        }

        // Simulate move to check for self-check
        ChessPiece captured = board[toRow][toCol];
        board[toRow][toCol] = piece;
        board[fromRow][fromCol] = ChessPiece(EMPTY, WHITE);

        if (isKingInCheck(currentTurn)) {
            board[fromRow][fromCol] = piece;
            board[toRow][toCol] = captured;
            cout << "Move exposes king to check!\n";
            return false;
        }

        // Check for pawn promotion
        if (piece.type == PAWN && (toRow == 7 || toRow == 0)) {
            char promote;
            cout << "Promote to (Q/R/B/N): ";
            cin >> promote;
            promote = toupper(promote);
            PieceType newType;
            switch (promote) {
                case 'Q': newType = QUEEN; break;
                case 'R': newType = ROOK; break;
                case 'B': newType = BISHOP; break;
                case 'N': newType = KNIGHT; break;
                default: newType = QUEEN;
            }
            board[toRow][toCol] = ChessPiece(newType, currentTurn, true);
        }

        // Update turn
        currentTurn = (currentTurn == WHITE) ? BLACK : WHITE;
        return true;
    }

    bool isGameOver()  {
        return isCheckmate(WHITE) || isCheckmate(BLACK);
    }

    bool isCheckmate(Color color) {
        if (!isKingInCheck(color)) return false;
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if (board[r][c].color == color) {
                    for (int tr = 0; tr < 8; tr++) {
                        for (int tc = 0; tc < 8; tc++) {
                            if (isLegalMoveWithoutKingCheck(r, c, tr, tc, color)) {
                                ChessPiece captured = board[tr][tc];
                                ChessPiece piece = board[r][c];
                                board[tr][tc] = piece;
                                board[r][c] = ChessPiece(EMPTY, WHITE);
                                bool stillInCheck = isKingInCheck(color);
                                board[r][c] = piece;
                                board[tr][tc] = captured;
                                if (!stillInCheck) return false;
                            }
                        }
                    }
                }
            }
        }
        return true;
    }

    Color getTurn() const { return currentTurn; }
};

int main() {
    ChessBoard board;
    string from, to;

    cout << "=== Console Chess ===\n";
    cout << "Format: a1 a2\n\n";

    while (!board.isGameOver()) {
        board.display();
        cout << "Enter move: ";
        cin >> from >> to;
        if (board.makeMove(from, to)) {
            cout << "Move successful!\n";
        }
    }

    board.display();
    cout << "Checkmate! " << (board.getTurn() == WHITE ? "Black" : "White") << " wins!\n";

    return 0;
}
