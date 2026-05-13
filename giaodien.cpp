void draw() {
    gotoxy(0, 0); 
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (board[i][j] == 0) {
                // SV3: In 2 dấu chấm để đồng bộ chiều rộng
                cout << ". "; 
            } else {
                // SV3: QUAN TRỌNG NHẤT - Nhân đôi ký tự để khối gạch vuông vức
                // Thay vì in 1 ký tự dẹt, ta in 2 ký tự sát nhau
                cout << "##"; 
            }
        }
        cout << endl;
    }
}