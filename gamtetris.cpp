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

// Hàm khởi tạo màu 
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
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

// Vẽ khung
void draw() {
    gotoxy(0, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (board[i][j] == ' ') {
                cout << "  ";
            }
            else if (board[i][j] == '#') {
                setColor(7);
                cout << "[]";
            }
            else {
                    // Thiết lập màu cho các khối
                 if (board[i][j] == 'I')
                    setColor(3); // Xanh cyan đậm

                 else if (board[i][j] == 'O')
                    setColor(14); // Vàng sáng

                 else if (board[i][j] == 'T')
                    setColor(13); // Hồng/Tím sáng

                 else if (board[i][j] == 'S')
                    setColor(10); // Xanh lá sáng

                 else if (board[i][j] == 'Z')
                    setColor(12); // Đỏ sáng

                 else if (board[i][j] == 'J')
                    setColor(9); // Xanh dương sáng

                 else if (board[i][j] == 'L')
                    setColor(6); // Vàng Đậm

                 cout << "[]";

                 setColor(7);
            }
        }
        cout << endl;
    }
    cout << "Score: " << score << "    " << endl;
    cout << "Speed: " << speed << "ms   " << endl;
}

// Hiển thị khối kế tiếp
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

// Xoay block
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

// Xóa hàng và tăng tốc 
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

// Hướng dẫn chơi
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

// Giao diện của trò chơi
void menu() {
    system("cls");

    // Các lựa chọn trong trò chơi
    cout << "================================================\n";

    // Đặt màu cho tựa game
    setColor(11);
    cout << "    TTTTTT ";

    setColor(14);
    cout << "EEEEEE ";

    setColor(13);
    cout << "TTTTTT ";

    setColor(12);
    cout << "RRRRR ";

    setColor(10);
    cout << " IIIIII ";

    setColor(9);
    cout << "SSSSS\n";


    setColor(11);
    cout << "      TT   ";

    setColor(14);
    cout << "EE     ";

    setColor(13);
    cout << "  TT   ";

    setColor(12);
    cout << "RR  RR ";

    setColor(10);
    cout << "  II   ";

    setColor(9);
    cout << "SS    \n";


    setColor(11);
    cout << "      TT   ";

    setColor(14);
    cout << "EEEEE  ";

    setColor(13);
    cout << "  TT   ";

    setColor(12);
    cout << "RRRRR  ";

    setColor(10);
    cout << "  II   ";

    setColor(9);
    cout << " SSS  \n";


    setColor(11);
    cout << "      TT   ";

    setColor(14);
    cout << "EE     ";

    setColor(13);
    cout << "  TT   ";

    setColor(12);
    cout << "RR RR  ";

    setColor(10);
    cout << "  II   ";

    setColor(9);
    cout << "   SS\n";


    setColor(11);
    cout << "      TT   ";

    setColor(14);
    cout << "EEEEEE ";

    setColor(13);
    cout << "  TT   ";

    setColor(12);
    cout << "RR  RR ";

    setColor(10);
    cout << "IIIIII ";

    setColor(9);
    cout << "SSSSS\n";

    setColor(7);

    cout << "================================================\n";

    cout << "\n";

    cout << "===================== MENU =====================\n";
    cout << "1. Choi game\n";
    cout << "2. Huong dan choi game\n";
    cout << "3. Thoat game\n";

    char c = _getch();

    if (c == '2') {
        showGuide();
        menu();
    }

    if (c == '3') {
        exit(0);
    }
}


// Hàm dùng khi thua
void gameOver() {

    system("cls");

    cout << "==================== GAME OVER ==================\n";

    cout << "Final Score: " << score << "\n";
    cout << "1. Choi lai\n";
    cout << "2. Thoat game\n";

    char c = _getch();

    if (c == '1') {

        score = 0;
        speed = 200;
        x = 4;
        y = 0;

        initBoard();

        return;
    }

    exit(0);
}

int main() {
    srand(time(0));
    b = rand() % 7;
    next_b = rand() % 7;

    menu();

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
               gameOver();
            }
        }

        block2Board();
        draw();
        drawNextBlock();
        Sleep(speed); // Sử dụng biến speed để điều chỉnh tốc độ game
    }
    return 0;
}
