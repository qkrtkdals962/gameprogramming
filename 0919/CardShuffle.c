#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct trump {
    int order;
    char shape[3];
    int number;
};

void make_card(struct trump m_card[]);
void display_card(struct trump m_card[]);
void shuffle_card(struct trump m_card[]);

int main(void) {
    struct trump card[52];
    make_card(card);
    shuffle_card(card);
    display_card(card);
    return 0;
}

void make_card(struct trump m_card[]) {
    int i, j, k = 0;
    // 유니코드 헥사 코드 대신 직접 문자열 사용
    char shape[4][3] = {"♠", "♥", "♦", "♣"};
    
    for (i = 0; i < 4; i++) {
        for (j = 1; j <= 13; j++) {
            m_card[k].order = k + 1;
            strcpy(m_card[k].shape, shape[i]);
            m_card[k].number = j;
            k++;
        }
    }
}

void display_card(struct trump m_card[]) {
    int i;
    for (i = 0; i < 52; i++) {
        printf("%2d:%s%2d ", m_card[i].order, m_card[i].shape, m_card[i].number);
        if ((i + 1) % 13 == 0) {
            printf("\n");
        }
    }
}

void shuffle_card(struct trump m_card[]) {
    int i, rnd;
    struct trump temp;
    srand(time(NULL));
    
    for (i = 51; i >= 0; i--) {
        rnd = rand() % (i + 1);
        temp = m_card[i];
        m_card[i] = m_card[rnd];
        m_card[rnd] = temp;
    }
}