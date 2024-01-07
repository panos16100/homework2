#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX pow(10,6)
#define C 67
#define D 68

int main(void){
    int me=D;
    int player;
    putchar(me); 
    fflush(stdout);
    printf("\n"); 
    player=getchar(); 
    int rounds=1;
    while((player != EOF)&&(rounds<MAX)){
    if(player!=C && player!=D && player!='\n'){
      printf("sorry but you have to check it again");
      printf("%d\n",rounds);
      player=getchar();
      continue;
    }
     if(rounds%3==0){
        if(player==D){
          putchar(C);
          putchar('\n');
       
        }
        else{
          putchar(D);
          putchar('\n');
    
        }
     }
     else{
      me=player;
      putchar(me);
      putchar('\n');

     }
     fflush(stdout);
     player=getchar();
     rounds++;
    }
    return 0;
}