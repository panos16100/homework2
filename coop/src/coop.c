#include <stdio.h>
#include <math.h>
#define MAX pow(10,6)



int main(void){
    char me='D';
    char player;
    int rounds=0;
    putchar(me);
    putchar('\n');
    fflush(stdout);
    player=getchar();
    while((player!=EOF)&&(rounds<MAX)){
        rounds++;
        if((player!='C')&&(player!='D')){
            rounds--;
            player=getchar();
            continue;
        }
        if(rounds<MAX){
          if(!(rounds%10)){
            putchar('C');
            putchar('\n');
            fflush(stdout);
          }
          else{
            putchar(me);
            putchar('\n');
            fflush(stdout);
          }
        player=getchar();}
    }
    return 0;
}