void draw() {
    gotoxy(0, 0); 
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (board[i][j] == 0) {
                cout << ". "; 
            } else {
                
                cout << "##"; 
            }
        }
        cout << endl;
    }
}