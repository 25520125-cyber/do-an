#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>

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

///////////////////////////////////////////////////// PHẦN XOAY BLOCK
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

///////////////////////////////////////////////////// PHẦN XỬ LÝ XÓA HÀNG VÀ TĂNG TỐC
void removeLine() {
    for (int i = H - 2; i > 0; i--) {
        int count = 0;
        for (int j = 1; j < W - 1; j++) {
            if (board[i][j] != ' ') count++;
        }
        if (count == W - 2) { // Nếu hàng đầy
            for (int ii = i; ii > 0; ii--) {
                for (int jj = 1; jj < W - 1; jj++) {
                    board[ii][jj] = board[ii - 1][jj];
                }
            }
            // Cộng điểm
            score += 10;
            // Rẽ nhánh tăng tốc độ: Mỗi lần xóa hàng giảm 10ms delay (nhanh dần)
            if (speed > 50) speed -= 10;
            i++;
        }
    }
}

int main() {
    srand(time(0));
    b = rand() % 7;
    system("cls");
    initBoard();

    while (1) {
        boardDelBlock();

        if (kbhit()) {
            char c = getch();
            if (c == 'a' && canMove(-1, 0)) x--;
            if (c == 'd' && canMove(1, 0)) x++;
            if (c == 'x' && canMove(0, 1)) y++;
            if (c == 'w') rotateBlock(); // Sử dụng W để xoay block
            if (c == 'q') break;
        }

        if (canMove(0, 1)) {
            y++;
        } else {
            block2Board();
            removeLine(); // Gọi hàm xóa hàng và tăng tốc độ khi gạch tiếp đất
            x = 4; y = 0; b = rand() % 7;
            if (!canMove(0, 0)) {
                system("cls");
                cout << "GAME OVER!" << endl;
                cout << "Final Score: " << score << endl;
                break;
            }
        }

        block2Board();
        draw();
        _sleep(speed); // Tốc độ rơi dựa trên biến speed
    }
    return 0;
}
