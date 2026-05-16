#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

#define H 20
#define W 15

char board[H][W] = {};
char blocks[][4][4] = {
    {{' ','I',' ',' '},{' ','I',' ',' '},{' ','I',' ',' '},{' ','I',' ',' '}},
    {{' ',' ',' ',' '},{' ','O','O',' '},{' ','O','O',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ','T',' ',' '},{'T','T','T',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ','S','S',' '},{'S','S',' ',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{'Z','Z',' ',' '},{' ','Z','Z',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{'J',' ',' ',' '},{'J','J','J',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ',' ','L',' '},{'L','L','L',' '},{' ',' ',' ',' '}}
};

int score = 0;
int speed = 200;
int x = 4, y = 0, b = 1;
int next_b = 0; 

void gotoxy(int x, int y) {
    COORD c = {(short)x, (short)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void boardDelBlock() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (blocks[b][i][j] != ' ' && y + i < H)
                board[y + i][x + j] = ' ';
}

void block2Board() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (blocks[b][i][j] != ' ')
                board[y + i][x + j] = blocks[b][i][j];
}

void initBoard() {
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if ((i == H - 1) || (j == 0) || (j == W - 1)) board[i][j] = '#';
            else board[i][j] = ' ';
}

bool canMove(int dx, int dy) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (blocks[b][i][j] != ' ') {
                int tx = x + j + dx;
                int ty = y + i + dy;
                if (tx < 1 || tx >= W - 1 || ty >= H - 1) return false;
                if (board[ty][tx] != ' ') return false;
            }
    return true;
}

void draw() {
    gotoxy(0, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (board[i][j] == ' ') cout << "  ";
            else if (board[i][j] == '#') cout << "##";
            else cout << "[]";
        }
        cout << endl;
    }
    cout << "Score: " << score << "    " << endl;
    cout << "Speed: " << speed << "ms   " << endl;
}

void drawNextBlock() {
    int startX = W * 2 + 5; 
    int startY = 2;         

    gotoxy(startX, startY);
    cout << "NEXT BLOCK:";

    for (int i = 0; i < 4; i++) {
        gotoxy(startX, startY + 1 + i);
        for (int j = 0; j < 4; j++) {
            if (blocks[next_b][i][j] != ' ') {
                cout << "[]"; 
            } else {
                cout << "  "; 
            }
        }
    }
}

void rotateBlock() {
    char temp[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            temp[i][j] = blocks[b][i][j];

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            blocks[b][j][3 - i] = temp[i][j];

    if (!canMove(0, 0)) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                blocks[b][i][j] = temp[i][j];
    }
}

void removeLine() {
    for (int i = H - 2; i > 0; i--) {
        int count = 0;
        for (int j = 1; j < W - 1; j++) {
            if (board[i][j] != ' ') count++;
        }
        if (count == W - 2) { 
            for (int ii = i; ii > 0; ii--) {
                for (int jj = 1; jj < W - 1; jj++) {
                    board[ii][jj] = board[ii - 1][jj];
                }
            }
            score += 10;
            if (speed > 50) speed -= 10; // Tăng tốc độ khi xóa hàng
            i++;
        }
    }
}

void showGuide() {
    system("cls");
    cout << "===== HUONG DAN CHOI =====\n";
    cout << "A : Di chuyen sang trai\n";
    cout << "D : Di chuyen sang phai\n";
    cout << "W : Xoay khoi\n";
    cout << "X : Di chuyen nhanh hon\n";
    cout << "Q : Thoat\n";
    cout << "Nhan bat ky phim nao de quay lai...";
    _getch();
}

int main() {
    srand(time(0));
    b = rand() % 7;
    next_b = rand() % 7;

    showGuide(); // Hiển thị hướng dẫn trước khi vào game

    system("cls");
    initBoard();

    while (1) {
        boardDelBlock();

        if (_kbhit()) {
            char c = _getch();
            if (c == 'a' && canMove(-1, 0)) x--;
            if (c == 'd' && canMove(1, 0)) x++;
            if (c == 'x' && canMove(0, 1)) y++;
            if (c == 'w' || c == ' ') rotateBlock(); 
            if (c == 'q') break;
        }

        if (canMove(0, 1)) {
            y++;
        } else {
            block2Board();
            removeLine(); 
            
            // Cập nhật khối hiện tại từ khối tiếp theo
            x = 4; y = 0; 
            b = next_b;
            next_b = rand() % 7;

            if (!canMove(0, 0)) {
                system("cls");
                cout << "GAME OVER!" << endl;
                cout << "Final Score: " << score << endl;
                break;
            }
        }

        block2Board();
        draw();
        drawNextBlock();
        Sleep(speed); // Sử dụng biến speed để điều chỉnh tốc độ game
    }
    return 0;
}