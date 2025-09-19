#include <stdio.h>
#include <math.h>
#include <windows.h>

int calc_frequency(int octave, int inx);

int main(void)
{
    int index[]={0, 2, 4, 5, 7, 9, 11, 12}; // 도레미파솔라시도
    int freq[8];
    int i;

    // 옥타브 4의 도~도 주파수 계산
    for(i=0;i<8;i++)
        freq[i]=calc_frequency(4, index[i]);

    int melody[] = {
        0,0,4,4,5,5,4,
        3,3,2,2,1,1,0,
        4,4,3,3,2,2,1,
        4,4,3,3,2,2,1
    };
    // 각 음 길이(ms). 400=4분음표, 필요하면 800=2분음표
    int duration[] = {
        400,400,400,400,400,400,800,
        400,400,400,400,400,400,800,
        400,400,400,400,400,400,800,
        400,400,400,400,400,400,800
    };
    int length = sizeof(melody)/sizeof(melody[0]);

    for(i=0;i<length;i++) {
        Beep(freq[melody[i]], duration[i]);
        Sleep(100); // 음과 음 사이 간격
    }

    return 0;
}

int calc_frequency(int octave, int inx)
{
    double do_scale=32.7032;
    double ratio=pow(2., 1.0/12.0);
    double temp=do_scale*pow(2, octave-1);
    int i;
    for(i=0;i<inx;i++) {
        temp*=ratio;
    }
    return (int)(temp+0.5);
}
