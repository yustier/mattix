#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VERSION_MAJOR 0
#define VERSION_MINOR 13

#define BLANK 0
#define TPIECE 4096 // DO NOT USE 4096 ANYWHERE IN THE CODE

// タイトル画面を表示する
int title(void){

    printf("\n+----------------------+\n"
             "|        Mattix        |\n"
             "|    Made by Airoku    |\n"
             "+----------------------+\n"
             "  Version: %d.%d\n\n", VERSION_MAJOR, VERSION_MINOR);

    return 0;
}

// int型の配列をシャッフルする
int shuffleIntArray(int array[], int size){

    int i, rnd, tmp;

    srand((unsigned int)time(NULL));
    for(i = size - 1; i > 1; i--){
        rnd = rand() % i;
        tmp = array[i];
        array[i] = array[rnd];
        array[rnd] = tmp;
    }

    return 0;
}

// ボードを初期化する
int initiateBoard(int mode, int board[][6], int set[], int side){

    int i, j, k=0;

    for(i=0; i<side; i++){
        for(j=0; j<side; j++){
            if(mode==1)board[i][j] = set[k]; // モード1: 与えられた駒のセットで初期化
            else if(mode==0)board[i][j] = BLANK; // モード2: すべて空白で初期化
            else return -1; // 異常終了(-1: モードが不適切)
            k++;
        }
    }

    return 0;
}

// 取り駒を検出する
int findTpiece(int mode, int board[][6], int side){
    int roworcolumn, i, j;
    for(i=0; i<side; i++){
        for(j=0; j<side; j++){
            if(mode==1){ // 行を検出
                if(board[i][j]==TPIECE)roworcolumn = i;
            }
            else if(mode==2){ // 列を検出
                if(board[i][j]==TPIECE)roworcolumn = j;
            }
            else return -1; // 異常終了(-1: モードが不適切)
        } //行を検出
    }
    return roworcolumn;
}

// ボードを表示
int printboard(int board[][6], int side){

    int i, j;

    printf("\n  ");
    for(i=1; i<=side; i++)printf("  %d ", i);
    printf("\n  ");
    for(i=1; i<=side; i++)printf("+---");
    printf("+\n");
    for(i=0; i<side; i++){
        printf("%d ", i+1);
        for(j=0; j<side; j++){
            if(board[i][j]==TPIECE)printf("| * ");
            else if(board[i][j]==BLANK)printf("|   ");
            else printf("|%3d", board[i][j]);
        }
        printf("|\n  ");
        for(j=1; j<=side; j++)printf("+---");
        printf("+\n");
    }
    printf("\n");

    return 0;
}

// 置ける場所があるか調べる
int checknoplace(int player, int board[][6], int trow, int tcolumn, int side){

    int i, value=0, count=0;

    for(i=0; i<=side; i++){
        if(player==1 && (board[trow][i]!=0 || board[trow][i]!=TPIECE))count++;
        if(player==2 && (board[i][tcolumn]!=0 || board[trow][tcolumn]!=TPIECE))count++;
    }
    if(count==0)value=1;

    return value;
}

// 範囲外をはじいてムーブ入力
int inputmove(int side, int player){

    int input;

    while(1){
        if(player==1)printf("Player 1 Next Move (column) ?: ");
        else if(player==2)printf("Player 2 Next Move (row) ?: ");
        else return -1; // 異常終了(-1:プレイヤー番号がおかしい)
        if(scanf("%d%*c", &input) != 1){
            scanf("%*s");
            printf("Invalid input. Use integers from 1 to %d.\n", side);
            continue;
        }
        if(1 <= input && input <= side)break;
        else printf("Out of bound. Input an integer from 1 to %d.\n", side);
    }

    return input;
}

// ゲーム
int game(int type, int side, int size){

    int bset[16]={1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, TPIECE}, 
        aset[36]={1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, TPIECE, 
                  1, 2, 3, 8, 9, 9, 10, -1, -1, -2, -2, -3, -3, -4, -5, -6, -7, -8, -9, -10}, 
        board[6][6], move, trow, tcolumn, score1=0, score2=0, turn=0; // ゲームに関する変数
    int i; // 処理に関する変数

    initiateBoard(0, board, aset, 6); // ボードを空白で初期化

    // ゲームの初期化
    if(type==1){ // ベーシックゲーム
        printf("[ BASIC GAME ]\n\n");
        shuffleIntArray(bset, size); // 駒をシャッフル
        initiateBoard(1, board, bset, side); // ボードを初期化
    }
    else if(type==2){ // アドバンスゲーム
        printf("[ ADVANCED GAME ]\n\n");
        shuffleIntArray(aset, size); // 駒をシャッフル
        initiateBoard(1, board, aset, side); // ボードを初期化
    }
    else return -1; // 異常終了(-1:ゲームの種類がおかしい)
    // 取り駒を初期化
    trow = findTpiece(1, board, side);
    tcolumn = findTpiece(2, board, side);

    // ゲーム中
    for(i=1; 1; i = i % 2 + 1){
        turn++;
        printf("\nTurn    : %3d\nPlayer 1: %3d pt\nPlayer 2: %3d pt\n", turn, score1, score2); // 現在の状況を表示
        printboard(board, side); // ボードを表示
        while(1){ // 次のムーブを入力
            if(checknoplace(i, board, trow, tcolumn, side)==1)break; // 置ける場所がない時はプロンプトを表示しない
            move = inputmove(side, i);
            if(i==1 && move != tcolumn + 1){
                if(board[trow][move-1]!=0)break;
                printf("You cannot move to the place without any piece.\n");
                continue;
            }
            else if(i==2 && move != trow + 1){
                if(board[move-1][tcolumn]!=0)break;
                printf("You cannot move to the place without any piece.\n");
                continue;
            }
            printf("You cannot move to the same column or row as now.\n");
        }
        if(checknoplace(i, board, trow, tcolumn, side)==1){
            if(checknoplace(i % 2 + 1, board, trow, tcolumn, side)==1){ // 両方のプレイヤーが動かせない
                printf("Both players have no possible moves.\n");
                break;
            }
            else{ // 今のプレイヤーが動かせない(パス)
                printf("Player %d has no possible moves.\n", i);
                continue;
            }
        }
        board[trow][tcolumn] = BLANK; // 取り駒を消す
        if(i==1){
            tcolumn = move - 1; //取り駒の位置を変更
            score1 += board[trow][tcolumn]; // スコアを計算
        }
        else if(i==2){
            trow = move - 1;
            score2 += board[trow][tcolumn];
        }
        board[trow][tcolumn] = TPIECE; // 取り駒を移動
        if(turn + 1 >= size)break; // ゲーム終了判定
    }

    // ゲーム清算と勝敗判定
    printf("Game Over\n\nPlayer 1: %3d pt\nPlayer 2: %3d pt\n", score1, score2);
    if(score1 > score2)printf("The winner is player 1.\n");
    else if(score1 < score2)printf("The winner is player 2.\n");
    else printf("The game is draw.\n");
    printf("Press Enter to return to main menu.");
    scanf("%*[^\n]%*c");

    return 0;
}

// 作者について
int about(void){

    printf("\n+---------------+\n"
             "|     About     |\n"
             "+---------------+\n\n");
    printf(" Made by Airoku\n More info on https://bit.ly/airokusite\n");
    printf("\nMattix is a board game that was born in Israel in the 1970s and became a\n"
             "million seller worldwide.  This computer game is based on the one sold by\n"
             "Gakken.\n");
    printf("\nInstructions:\n"
             "Maybe I will write it later. Until then please google it by yourself.\n");
    printf("\nPress Enter to return to main menu.");
    scanf("%*[^\n]%*c");

    return 0;
}

// ゲームを終了する
int quit(void){

    printf("Thank you for playing.\n\n");

    return 0;
}

int main(void){

    int maincmd;

    title();
    while(1){
        printf(" 1. BASIC GAME\n 2. ADVANCED GAME\n 3. ABOUT\n 4. QUIT\n\nCommand?: ");
        if(scanf("%d%*c", &maincmd) != 1){
            scanf("%*s");
            printf("Invalid command. Use integers from 1 to 4.\n\n");
            continue;
        }
        if(maincmd==1)game(1, 4, 16);
        else if(maincmd==2)game(2, 6, 36);
        else if(maincmd==3)about();
        else if(maincmd==4)break;
        else{
            printf("Out of bound. Use integers from 1 to 4.\n\n");
            continue;
        }
        title();
    }
    quit();

    return 0;
}

///////////////////////////////////
// (C) 2020 Airoku
///////////////////////////////////
