#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100000
#define MAX_PLUS 100001



int main(int argc,char *argv[]){
    if(argc!=3){
        printf("sorry but you have to check it again\n");
        return 1;
    }
    FILE *filetoread1=fopen(argv[1],"rb");
    FILE *filetoread2=fopen(argv[2],"rb");
    if((!filetoread1)||(!filetoread2)){
        printf("files does not exists\n");
        return 1;
    }
    char array1[MAX_PLUS];
    char array2[MAX_PLUS];
    size_t diabasa1=fread(array1,sizeof(char),MAX,filetoread1);
    size_t diabasa2=fread(array2,sizeof(char),MAX,filetoread2);
    array1[diabasa1]='\0';
    array2[diabasa2]='\0';
    
    size_t i,j,k;
    long int count,max=0;
    char teliko[MAX_PLUS];
    char *array=malloc(MAX_PLUS*sizeof(char));
    for(i=0;i<diabasa1;i++){
        k=i;
        count=0;
        for(j=0;j<diabasa2 && array1[k]!='\0';j++){
            if(array1[k]==array2[j]){
                array[count]=array2[j];
                count++;
                k++;
            }
            else{
                if(count>max){
                    max=count;
                    int h;
                    for(h=0;h<=max;h++){
                        teliko[h]='\0';
                    }
                    
                    array[max]='\0';
                    strcpy(teliko,array);
        
                }
                free(array);
                array=malloc(MAX_PLUS*sizeof(char));
                count=0;
                k=i;
            }
            
                    
                
        
        }
        if(count>max){
            max=count;
            int h;
            for(h=0;h<=max;h++){
                teliko[h]='\0';
            }
            
            
            array[max]='\0';
            strcpy(teliko,array);
            free(array);
            array=malloc(MAX_PLUS*sizeof(char));
        }
        
    }
    free(array);
    printf("%s\n",teliko);
    
    
    
    fclose(filetoread1);
    fclose(filetoread2);
    return 0;
}
   