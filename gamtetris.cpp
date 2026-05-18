#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <string>

using namespace std;

#define H 20
#define W 15
#define OFFSET_X 25

enum Color {
    CYAN = 11, YELLOW = 14, PURPLE = 13, GREEN = 10, RED = 12, BLUE = 9, WHITE = 15, GRAY = 8
};

char board[H][W] = {};
char blocks[][4][4] = {
    {{' ','1',' ',' '},{' ','1',' ',' '},{' ','1',' ',' '},{' ','1',' ',' '}},
    {{' ',' ',' ',' '},{' ','2','2',' '},{' ','2','2',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ','3',' ',' '},{'3','3','3',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ','4','4',' '},{'4','4',' ',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{'5','5',' ',' '},{' ','5','5',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{'6',' ',' ',' '},{'6','6','6',' '},{' ',' ',' ',' '}},
    {{' ',' ',' ',' '},{' ',' ','7',' '},{'7','7','7',' '},{' ',' ',' ',' '}}
};

int score = 0, speed = 200;
int x = 4, y = 0, b = 1, next_b = 0;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void applyBlockColor(char c) {
    switch (c) {
        case '1': setColor(CYAN); break;
        case '2': setColor(YELLOW); break;
        case '3': setColor(PURPLE); break;
        case '4': setColor(GREEN); break;
        case '5': setColor(RED); break;
        case '6': setColor(BLUE); break;
        case '7': setColor(WHITE); break;
        case '#': setColor(GRAY); break;
        default: setColor(WHITE); break;
    }
}

void gotoxy(int x, int y) {
    COORD c = {(short)x, (short)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void drawBigTitle() {
    int tx = OFFSET_X + 11;
    int ty = 2;
    int colors[] = {12, 14, 10, 11, 9, 13};
    string art[5][6] = {
        {"[][][] ", "[][][] ", "[][][] ", "[][][] ", " [] ", "[][][]"},
        {"  []   ", "[]     ", "  []   ", "[]  [] ", " [] ", "[]    "},
        {"  []   ", "[][]   ", "  []   ", "[][]   ", " [] ", " [][] "},
        {"  []   ", "[]     ", "  []   ", "[]  [] ", " [] ", "    []"},
        {"  []   ", "[][][] ", "  []   ", "[]  [] ", " [] ", "[][][]"}
    };

    setColor(7);
    gotoxy(tx - 4, ty - 1);
    cout << "===============================================";

    for (int i = 0; i < 5; i++) {
        gotoxy(tx, ty + i);
        for (int j = 0; j < 6; j++) {
            setColor(colors[j]);
            cout << art[i][j];
        }
    }

    setColor(7);
    gotoxy(tx - 4, ty + 5);
    cout << "===============================================";
    gotoxy(tx - 4, ty + 6);
    cout << "================= MENU GAME ===================";
    setColor(15);
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

void loadingScreen() {
    system("cls");
    int lx = OFFSET_X + 21;
    int ly = H / 2 + 2;
    setColor(CYAN);
    string msg = "    Loading...";
    for (int i = 0; i <= 100; i += 5) {
        gotoxy(lx, ly); cout << msg;
        gotoxy(lx - 2, ly + 2);
        cout << "[";
        int barWidth = 20;
        int pos = (i * barWidth) / 100;
        for (int j = 0; j < barWidth; ++j) {
            if (j < pos) cout << "=";
            else cout << " ";
        }
        cout << "] " << i << "%";
        Sleep(50);
    }
    setColor(WHITE);
    gotoxy(lx, ly + 4); cout << "    complete!";
    Sleep(500);
}

void showGuide() {
    system("cls");
    setColor(YELLOW);
    int gx = OFFSET_X + 11;
    gotoxy(gx, 8);  cout << "    === HUONG DAN ===";
    setColor(WHITE);
    gotoxy(gx, 10); cout << "      A: Sang trai";
    gotoxy(gx, 11); cout << "      D: Sang phai";
    gotoxy(gx, 12); cout << "      W: Xoay khoi";
    gotoxy(gx, 13); cout << "      X: Roi nhanh";
    gotoxy(gx, 16); cout << "Bam phim bat ky de quay lai...";
    _getch();
}

int menu() {
    int choice = 0;
    while (true) {
        system("cls");
        drawBigTitle();
        int mx = OFFSET_X + 21;

        gotoxy(mx, 10);
        if (choice == 0) { setColor(CYAN); cout << " > BAT DAU GAME <"; }
        else { setColor(WHITE); cout << "   BAT DAU GAME  "; }

        gotoxy(mx, 11);
        if (choice == 1) { setColor(CYAN); cout << "> HUONG DAN CHOI <"; }
        else { setColor(WHITE); cout << "  HUONG DAN CHOI  "; }

        gotoxy(mx, 12);
        if (choice == 2) { setColor(RED); cout << "  > THOAT GAME <"; }
        else { setColor(WHITE); cout << "    THOAT GAME  "; }

        char c = _getch();

        if (c == 'w' || c == 72) choice = (choice - 1 + 3) % 3;
        if (c == 's' || c == 80) choice = (choice + 1) % 3;
        if (c == 13) { setColor(WHITE); return choice; }
    }
}

void boardDelBlock() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (blocks[b][i][j] != ' ' && y + i < H) board[y + i][x + j] = ' ';
}

void block2Board() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (blocks[b][i][j] != ' ') board[y + i][x + j] = blocks[b][i][j];
}

void draw() {
    for (int i = 0; i < H; i++) {
        gotoxy(OFFSET_X, i);
        for (int j = 0; j < W; j++) {
            if (board[i][j] == ' ') cout << "  ";
            else {
                applyBlockColor(board[i][j]);
                if (board[i][j] == '#') cout << "##";
                else cout << "[]";
            }
        }
    }
    setColor(WHITE);
    gotoxy(OFFSET_X, H + 1); cout << "Score: " << score << "    ";
}

void drawNextBlock() {
    int startX = OFFSET_X + (W * 2) + 6;
    setColor(WHITE);
    gotoxy(startX, 2); cout << "NEXT:";
    for (int i = 0; i < 4; i++) {
        gotoxy(startX, 3 + i);
        for (int j = 0; j < 4; j++) {
            if (blocks[next_b][i][j] != ' ') {
                applyBlockColor(blocks[next_b][i][j]);
                cout << "[]";
            }
            else cout << "  ";
        }
    }
    setColor(WHITE);
}

void rotateBlock() {
    char temp[4][4];
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) temp[i][j] = blocks[b][i][j];
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) blocks[b][j][3 - i] = temp[i][j];
    if (!canMove(0, 0)) for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) blocks[b][i][j] = temp[i][j];
}

void removeLine() {
    for (int i = H - 2; i > 0; i--) {
        int count = 0;
        for (int j = 1; j < W - 1; j++) if (board[i][j] != ' ') count++;
        if (count == W - 2) {
            for (int ii = i; ii > 0; ii--) for (int jj = 1; jj < W - 1; jj++) board[ii][jj] = board[ii - 1][jj];
            score += 10;
            if (speed > 50) speed -= 5;
            i++;
        }
    }
}

int main() {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);

    srand(time(0));

    start_menu:
    int choice = menu();
    if (choice == 1) {
        showGuide();
        goto start_menu;
    }
    if (choice == 2) { // Xử lý khi chọn Thoát
        system("cls");
        setColor(YELLOW);
        gotoxy(OFFSET_X + 5, H / 2);
        cout << "CAM ON BAN DA CHOI! TAM BIET!";
        Sleep(1000);
        return 0;
    }

    loadingScreen();
    system("cls");
    initBoard();
    score = 0; speed = 200;
    b = rand() % 7; next_b = rand() % 7;
    x = 4; y = 0;

    clock_t lastTime = clock();

    while (1) {
        if (_kbhit()) {
            boardDelBlock();
            char c = _getch();
            if (c == 'a' && canMove(-1, 0)) x--;
            if (c == 'd' && canMove(1, 0)) x++;
            if (c == 'x' && canMove(0, 1)) y++;
            if (c == 'w' || c == ' ') rotateBlock();
            if (c == 'q') goto start_menu; // Nhấn Q để quay lại menu

            block2Board();
            draw();
        }

        if (clock() - lastTime >= speed) {
            boardDelBlock();
            if (canMove(0, 1)) {
                y++;
                block2Board();
            } else {
                block2Board();
                removeLine();
                x = 4; y = 0;
                b = next_b; next_b = rand() % 7;
                while(_kbhit()) _getch();

                if (!canMove(0, 0)) {
                    system("cls");
                    int goX = OFFSET_X + 10;
                    int goY = H / 2 - 2;

                    int gameOverChoice = 0;
                    while (true) {

                        gotoxy(goX, goY);
                        setColor(RED); cout << "      GAME OVER!      ";

                        gotoxy(goX, goY + 2);
                        setColor(WHITE); cout << "  Tong diem cua ban: ";
                        setColor(YELLOW); cout << score;


                        gotoxy(goX + 3, goY + 4);
                        if (gameOverChoice == 0) { setColor(CYAN); cout << "  > CHOI LAI < "; }
                        else { setColor(WHITE); cout << "    CHOI LAI   "; }

                        gotoxy(goX + 3, goY + 5);
                        if (gameOverChoice == 1) { setColor(RED); cout << " > THOAT GAME < "; }
                        else { setColor(WHITE); cout << "   THOAT GAME   "; }


                        char key = _getch();
                        if (key == 'w' || key == 72) gameOverChoice = 0;
                        if (key == 's' || key == 80) gameOverChoice = 1;
                        if (key == 13) break;
                    }

                    if (gameOverChoice == 0) {
                        goto start_menu;
                    } else {
                        system("cls");
                        setColor(YELLOW);
                        gotoxy(OFFSET_X + 5, H / 2);
                        cout << "CAM ON BAN DA CHOI! TAM BIET!";
                        Sleep(1000);
                        return 0;
                    }
                }
            }
            draw();
            drawNextBlock();
            lastTime = clock();
        }
        Sleep(1);
    }
    return 0;
}
