#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
/*
   - W,H: 보드 크기(8x8)
   - SCALE_X/Y: 칸 사이 여백 스케일(콘솔 렌더 확대)
   - BASE_X/Y: 보드 시작 좌표(콘솔 오프셋)
   - board[y][x]: 0=빈칸, 1..=목표 숫자
   - expectedNum: 다음에 맞춰야 할 숫자(초기 1)
   - playerX: 현재 총구 열(1..W)
   - COLOR_*: 콘솔 색상 상수
*/
#define W 8
#define H 8
#define MIN_STAGE 1
#define MAX_STAGE 61

#define SCALE_X 2
#define SCALE_Y 2

#define BASE_X 2
#define BASE_Y 0

static const WORD COLOR_DEFAULT = 7;
static const WORD COLOR_GREEN   = FOREGROUND_GREEN  | FOREGROUND_INTENSITY;
static const WORD COLOR_RED     = FOREGROUND_RED    | FOREGROUND_INTENSITY;
static const WORD COLOR_CYAN    = FOREGROUND_GREEN  | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
static const WORD COLOR_YELLOW  = FOREGROUND_RED    | FOREGROUND_GREEN| FOREGROUND_INTENSITY;
static const WORD COLOR_WHITE   = FOREGROUND_RED    | FOREGROUND_GREEN| FOREGROUND_BLUE | FOREGROUND_INTENSITY;

static int board[H+1][W+1];   /* 0=빈칸, 1..=목표 숫자 */
static int expectedNum = 1;   /* 다음에 맞출 숫자 */
static int playerX = 1;       /* 총구 X(1..W) */

/* ====== 콘솔 유틸 ====== */
void gotoxy(int x, int y){ COORD p={x-1,y-1}; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p); }
void setColor(WORD a){ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a); }
void clearScr(void){ system("cls"); }

int read_key(void){
    if(!_kbhit()) return 0;
    {
        int ch=_getch();
        if(ch==0 || ch==224){
            int e=_getch();
            if(e==72) return 'U';
            if(e==80) return 'D';
            if(e==75) return 'L';
            if(e==77) return 'R';
            return 0;
        }
        return ch; /* ' ', Enter=13, ESC=27, q/Q 등 */
    }
}

int get_console_cols(void){
    CONSOLE_SCREEN_BUFFER_INFO info;
    if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
        return info.srWindow.Right - info.srWindow.Left + 1;
    return 120;
}

/* ====== 좌표 매핑 ====== */
static void cell_pos(int x, int y, int* px, int* py){
    *px = BASE_X + 2*((x-1)*SCALE_X + 1);
    *py = BASE_Y + 1 + ((y-1)*SCALE_Y + 1);
}

void get_grid_rect(int* left, int* top, int* right, int* bottom){
    int pxW, pyH; cell_pos(W, H, &pxW, &pyH);
    *left   = BASE_X;
    *top    = BASE_Y + 1;
    *right  = pxW + 1;
    *bottom = pyH + 1;
}

void get_hud_origin(int* hud_x, int* hud_y){
    int L,T,R,B; get_grid_rect(&L,&T,&R,&B);
    int cols = get_console_cols();
    int desired = R + 4;
    if(desired + 24 <= cols){ *hud_x = desired; *hud_y = BASE_Y + 2; }
    else                    { *hud_x = BASE_X + 2; *hud_y = B + 1; }
}

/* ====== ASCII 배너 ====== */
void splash_order_rush(void){
    clearScr();
    setColor(COLOR_CYAN);

    gotoxy(BASE_X+3, BASE_Y+2);  printf("  ____    _____    _____    _____	__ _____");
    gotoxy(BASE_X+3, BASE_Y+3);  printf(" / __ \\  |  __ \\  |  __ \\  / _____| |  __ \\");
    gotoxy(BASE_X+3, BASE_Y+4);  printf("| |  | | | |__) | | |  | | | |_____ | |__) |");
    gotoxy(BASE_X+3, BASE_Y+5);  printf("| |  | | |  _  /  | |  | | |  ____| |  _  /");
    gotoxy(BASE_X+3, BASE_Y+6);  printf("| |__| | | | \\ \\  | |__| | | |_____ | | \\ \\");
    gotoxy(BASE_X+3, BASE_Y+7);  printf(" \\____/  |_|  \\_\\ |_____/  \\______| |_|  \\_\\");
    
    gotoxy(BASE_X+3, BASE_Y+9);  printf(" _____   _    _   ______    _    _");
    gotoxy(BASE_X+3, BASE_Y+10); printf("|  __ \\ | |  | | /  ____|  | |  | |");
    gotoxy(BASE_X+3, BASE_Y+11); printf("| |__) || |  | | |  (___   | |__| |");
    gotoxy(BASE_X+3, BASE_Y+12); printf("|  _  / | |  | |  \\___  \\  |  __  |");
    gotoxy(BASE_X+3, BASE_Y+13); printf("| | \\ \\ | |__| |  ____)  | | |  | |");
    gotoxy(BASE_X+3, BASE_Y+14); printf("|_|  \\_\\ \\____/  |______/  |_|  |_|");

    setColor(COLOR_WHITE);
    gotoxy(BASE_X+10, BASE_Y+17); printf("아무 키나 누르세요...");
    setColor(COLOR_DEFAULT);
    _getch();
}



void ascii_congratulations(void){
    clearScr();
    setColor(COLOR_GREEN);
    gotoxy(BASE_X+2, BASE_Y+3); printf("       ___                ___                  ____");
    gotoxy(BASE_X+2, BASE_Y+4); printf("      |   |              |   |                |    |");
    gotoxy(BASE_X+2, BASE_Y+5); printf(" _____|   |____     _____|   |____            |    |");
    gotoxy(BASE_X+2, BASE_Y+6); printf("|__________    |  |___________    |           |    |");
    gotoxy(BASE_X+2, BASE_Y+7); printf("          /   /              /   /            |    |");
    gotoxy(BASE_X+2, BASE_Y+8); printf("         /   /              /   /             |    |");
    gotoxy(BASE_X+2, BASE_Y+9); printf("        /    \\             /    \\             |    |");
    gotoxy(BASE_X+2, BASE_Y+10);printf("       /      \\           /      \\            |____|");
    gotoxy(BASE_X+2, BASE_Y+11);printf("      /   /\\   \\         /   /\\   \\            ____");
    gotoxy(BASE_X+2, BASE_Y+12);printf("     /___/  \\___\\       /___/  \\___\\          |____|");
    setColor(COLOR_DEFAULT);
    Sleep(1500);
}


void ascii_tryagain(void){
    clearScr();
    setColor(COLOR_YELLOW);
    gotoxy(BASE_X+4, BASE_Y+3);  printf("                                        ___________");
    gotoxy(BASE_X+4, BASE_Y+4);  printf("                                       /   _____   \\");
    gotoxy(BASE_X+4, BASE_Y+5);  printf("  __________        _________         /   /     \\   \\");
    gotoxy(BASE_X+4, BASE_Y+6);  printf(" |______    |      |  _______|        \\___\\     /   /");
    gotoxy(BASE_X+4, BASE_Y+7);  printf("  ______|   |      |  |______                  /   /");
    gotoxy(BASE_X+4, BASE_Y+8);  printf(" |   _______|      |   ______|                /   /");
    gotoxy(BASE_X+4, BASE_Y+9);  printf(" |  |_______       |  |______                 |___|");
    gotoxy(BASE_X+4, BASE_Y+10); printf(" |__________|      |_________|");
    gotoxy(BASE_X+4, BASE_Y+11); printf("                                              ____");
    gotoxy(BASE_X+4, BASE_Y+12); printf("                                             |____|");
    setColor(COLOR_DEFAULT);
    Sleep(1200);
}

void ascii_goodbye(void){
    clearScr();
    setColor(COLOR_RED);
    gotoxy(BASE_X+4, BASE_Y+4);  printf("  ___     ___     ___     ___");
    gotoxy(BASE_X+4, BASE_Y+5);  printf(" |  |     |  |   |  |     |  |       ~~~~~~~~      ~~~~~~~                   ");
    gotoxy(BASE_X+4, BASE_Y+6);  printf(" |  |     |  |   |  |     |  |      ~         ~    ~     ~            ");
    gotoxy(BASE_X+4, BASE_Y+7);  printf(" |  |_____|  |   |  |_____|  |     ~   ~~~~    ~~~~      ~                ");
    gotoxy(BASE_X+4, BASE_Y+8);  printf(" |   ______  |   |   _____   |     ~  ~    ~           ~           ");
    gotoxy(BASE_X+4, BASE_Y+9);  printf(" |  |_____|  |   |  |_____|  |     ~~~      ~~~~~~~~~~                   ");
    gotoxy(BASE_X+4, BASE_Y+10); printf(" |___________|   |___________|                                     ");
    setColor(COLOR_DEFAULT);
    Sleep(1200);
}

void ascii_lose(void){
    clearScr();
    setColor(COLOR_RED);
    gotoxy(BASE_X+4, BASE_Y+4);  printf(" _        ____    _____   ______");
    gotoxy(BASE_X+4, BASE_Y+5);  printf("| |      / __ \\  / ____| / _____| ");
    gotoxy(BASE_X+4, BASE_Y+6);  printf("| |     | |  | || (___   | |_____");
    gotoxy(BASE_X+4, BASE_Y+7);  printf("| |     | |  | | \\___ \\  |  ____|");
    gotoxy(BASE_X+4, BASE_Y+8);  printf("| |____ | |__| | ____) | | |_____ ");
    gotoxy(BASE_X+4, BASE_Y+9);  printf("|______| \\____/ |_____/  \\______| ");
    setColor(COLOR_DEFAULT);
    Sleep(1200);
}


/* Try Again? / Quit 메뉴 (그대로) */
int ascii_try_again_or_quit(void){
    int sel=0;
    ascii_tryagain();
    for(;;){
        setColor(COLOR_WHITE);
        gotoxy(BASE_X+15,BASE_Y+12);  printf("TRY AGAIN or QUIT GAME?");
        gotoxy(BASE_X+9,BASE_Y+15); if(sel==0){ setColor(COLOR_GREEN); printf("[ TRY AGAIN ]"); setColor(COLOR_DEFAULT);} else printf("  TRY AGAIN  ");
        gotoxy(BASE_X+31,BASE_Y+15); if(sel==1){ setColor(COLOR_RED);   printf("[ QUIT GAME ]"); setColor(COLOR_DEFAULT);} else printf("  QUIT GAME  ");
        gotoxy(BASE_X+7,BASE_Y+18); setColor(COLOR_CYAN); printf("LEFT/RIGHT to choose, ENTER to confirm."); setColor(COLOR_DEFAULT);

        int k=read_key();
        if(k=='L') sel=0;
        if(k=='R') sel=1;
        if(k==13)  return sel==0 ? 1 : 0;
        if(k==27)  return 0;
        Sleep(40);
    }
}

/* ====== 단계 선택 ====== */
int stage_select(void){
    int n=1;
    for(;;){
        clearScr();
        setColor(COLOR_WHITE);
        gotoxy(BASE_X+8,BASE_Y+4); printf("1부터 순서대로 쏘면 됩니다!");
        gotoxy(BASE_X+8,BASE_Y+6); printf("오른쪽키, 왼쪽키를 사용해 움직일 수 있습니다.");
        gotoxy(BASE_X+8,BASE_Y+7); printf("ENTER를 누르면 시작합니다.");
        setColor(COLOR_RED);
        gotoxy(BASE_X+8,BASE_Y+9); printf("종료를 원하시면 esc를 누르시오.");
        setColor(COLOR_WHITE);
        gotoxy(BASE_X+8,BASE_Y+11); printf("Select Stage (1~61)");
        setColor(COLOR_YELLOW);
        gotoxy(BASE_X+16,BASE_Y+13); printf("Stage: %2d", n);
        setColor(COLOR_DEFAULT);

        int k=read_key();
        if(k=='L'){ if(--n<MIN_STAGE) n=MIN_STAGE; }
        if(k=='R'){ if(++n>MAX_STAGE) n=MAX_STAGE; }
        if(k==13) return n;
        if(k==27) return 0;
        Sleep(40);
    }
}

/* ====== 프레임 & 라벨 ====== */
void draw_static_frame(void){
    int L,T,R,B; get_grid_rect(&L,&T,&R,&B);

    clearScr();
    /* 상단 */
    gotoxy(L, T); printf("+"); for(int x=L+1; x<=R-1; ++x) printf("-"); printf("+");
    /* 본문 */
    for(int y=T+1; y<=B-1; ++y){
        gotoxy(L, y); printf("|");
        for(int x=L+1; x<=R-1; ++x) printf(" ");
        printf("|");
    }
    /* 하단 */
    gotoxy(L, B); printf("+"); for(int x=L+1; x<=R-1; ++x) printf("-"); printf("+");

    /* 내부 세로선: 1..W-1 (양끝 제외) */
    for(int col=1; col<=W-1; ++col){
        int px, py; cell_pos(col, 1, &px, &py);
        for(int y=T+1; y<=B-1; ++y){ gotoxy(px, y); printf("|"); }
    }
}

void draw_column_labels(void){
    int pxW, pyH; cell_pos(W, H, &pxW, &pyH);
    int labelY = pyH + 3;
    for(int x=1; x<=W; ++x){
        int px, py; cell_pos(x, H, &px, &py);
        gotoxy(px, labelY); printf("%d", x);
    }
}

/* ====== 셀/총구/HUD ====== */
void redraw_cell(int x, int y){
    int px, py; cell_pos(x,y,&px,&py);
    gotoxy(px-1, py);
    int v=board[y][x];
    if(v==0){ printf("  "); return; }
    if(v==expectedNum){ setColor(COLOR_GREEN); printf("%2d", v); setColor(COLOR_DEFAULT); }
    else              { setColor(COLOR_RED);   printf("%2d", v); setColor(COLOR_DEFAULT); }
}

void draw_all_cells(void){
    for(int y=1;y<=H;++y)
        for(int x=1;x<=W;++x)
            redraw_cell(x,y);
}

void draw_gun(int x){
    int px, py; cell_pos(x,H,&px,&py);
    gotoxy(px-1, py+1); printf("/^\\");
}
void erase_gun(int x){
    int px, py; cell_pos(x,H,&px,&py);
    gotoxy(px-1, py+1); printf("   ");
}

void update_hud_at(int sx, int sy, int stage, int nextNum, int bullets, int msRemain){
    gotoxy(sx, sy+0); printf("Stage : %-2d  ", stage);
    gotoxy(sx, sy+1); printf("Next  : %-2d  ", nextNum);
    gotoxy(sx, sy+2); printf("bullet  : %-3d ", bullets);
    gotoxy(sx, sy+3); printf("Time  : %02d.%01d s ", msRemain/1000, (msRemain%1000)/100);
    gotoxy(sx, sy+5); setColor(COLOR_CYAN); printf("Move: 왼쪽키/오른쪽키    ");
    gotoxy(sx, sy+6); printf("Fire: SPACE         ");
    gotoxy(sx, sy+7); printf("Quit: Q             "); setColor(COLOR_DEFAULT);
}

/* ====== 수용량 가드 ====== */
static int min_rows_needed(int remaining){
    return (remaining + W - 1) / W;
}

/* ====== 배치 ====== */
void init_targets_monotone(int goals, int stage){
    for(int y=1;y<=H;y++) for(int x=1;x<=W;x++) board[y][x]=0;
    expectedNum=1;

    int prevY = H - (rand()%3); if(prevY<1) prevY = H;

    for(int num=1; num<=goals; ++num){
        int remaining = goals - (num - 1);
        int floorY = min_rows_needed(remaining);
        if(floorY > prevY) floorY = prevY;

        int yMin=floorY, yMax=prevY; if(yMax<yMin) yMax=yMin;
        int y = yMin + rand() % (yMax - yMin + 1);

        int x, tries=0;
        do{
            x = 1 + rand()%W;
            tries++;
            if(tries>64){
                if(y>yMin) y--;
                tries=0;
            }
        }while(board[y][x]!=0);

        board[y][x]=num;
        prevY=y;
    }

    playerX=1;
}

/* ====== 사격 ====== */
void fire_shot(void){
        if(board[y][playerX]!=0){
            int v=board[y][playerX];
            if(v==expectedNum){
                int px,py;
                board[y][playerX]=0;
                cell_pos(playerX,y,&px,&py);
                gotoxy(px-1,py); setColor(COLOR_YELLOW); printf("**"); setColor(COLOR_DEFAULT);
                Sleep(60);
                redraw_cell(playerX,y);
                expectedNum++;
            }
            return;
        }
    }
}

/* ====== 스테이지 컨트롤 ====== */
int run_stage(int n){
    int goals   = n + 3;//목표물 
    int bullets = n + 4;//총알 
    int limit   = 5000 + (3*(n-1)) * 1000;//시간 

    int HUD_X, HUD_Y;
    DWORD t0;
    int lastHudTick=-1;

    draw_static_frame();
    init_targets_monotone(goals, n);
    draw_all_cells();
    draw_gun(playerX);
    draw_column_labels();

    get_hud_origin(&HUD_X, &HUD_Y);

    t0=GetTickCount();
    for(;;){
        int elapsed=(int)(GetTickCount()-t0);
        int remain = limit - elapsed; if(remain<0) remain=0;

        /* HUD 0.1초마다 갱신 */
        int hudTick = remain/100;
        if(hudTick!=lastHudTick){
            update_hud_at(HUD_X, HUD_Y, n, expectedNum, bullets, remain);
            lastHudTick=hudTick;
        }

        /* 종료 조건: 성공 / 실패 구분 */
        if(expectedNum>goals){                 /* 성공 */
            ascii_congratulations();
            return 1;
        }
        if(remain==0 || bullets==0){           /* 실패: 시간 또는 탄약 소진 */
            return 0;
        }

        /* 입력 */
        int k=read_key();
        if(!k){ Sleep(5); continue; }
        if(k=='q'||k=='Q'){ ascii_goodbye(); exit(0); }
        if(k=='L' && playerX>1){ erase_gun(playerX); playerX--; draw_gun(playerX); }
        if(k=='R' && playerX<W){ erase_gun(playerX); playerX++; draw_gun(playerX); }
        if(k==' '){
            bullets--;
            fire_shot();
        }
    }
}

/* ====== main ====== */
int main(void){
    srand((unsigned)time(NULL));
    splash_order_rush();

    for(;;){
        int stg=stage_select();
        if(stg==0){ ascii_goodbye(); return 0; }

        int win = run_stage(stg);

        if(!win){
            ascii_lose();
        }

        // 메뉴는 항상 메인에서 단 한 번만
        if(!ascii_try_again_or_quit()){ ascii_goodbye(); break; }
    }
    return 0;
}

