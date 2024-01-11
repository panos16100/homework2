#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc,char *argv[]){
    if(argc!=3){
        printf("sorry but you have to check it again\n");
        return 1;
    }
    FILE *filetoread1=fopen(argv[1],"rb");
    FILE *filetoread2=fopen(argv[2],"rb");
    if (!filetoread1){
        printf("file1 does not exists\n");
        return 1;
    }
    else if(!filetoread2){
        printf("file2 does not exists\n");
        return 1;
    }
    char *buffer1=malloc(100001*sizeof(char));
    char *buffer2=malloc(100001*sizeof(char));
    size_t diabasa1=fread(buffer1,sizeof(char),100000,filetoread1);
    size_t diabasa2=fread(buffer2,sizeof(char),100000,filetoread2);
    buffer1[diabasa1]='\0';
    buffer2[diabasa2]='\0';
    size_t i,j,k;
    long int count,max=0;
    char *array=malloc(100001*sizeof(char));
    char *teliko=malloc(1*sizeof(char));
    
    for(i=0;i<diabasa1;i++){
       if((buffer1[i]!='A')&&(buffer1[i]!='C')&&(buffer1[i]!='T')&&(buffer1[i]!='G')){
        
        continue;
       }
       k=i;
       count=0;
        for(j=0;(j<diabasa2) &&(buffer1[k]!='\0');j++){
            if((buffer2[j]!='A')&&(buffer2[j]!='C')&&(buffer2[j]!='T')&&(buffer2[j]!='G')){
                
                continue;
            }
            if(buffer1[k]==buffer2[j]){
                array[count]=buffer2[j];
                count++;
                k++;
            }else{
                if(count>max){
                    max=count;
                    free(teliko);
                    array[max]='\0';
                    teliko=malloc((max+1)*sizeof(char));
                    strcpy(teliko,array);
                }
                free(array);
                array=malloc(100001*sizeof(char));
                count=0;
                k=i;
            }


        }
        if(count>max){
            max=count;
            free(teliko);
            array[max]='\0';
            teliko=malloc((max+1)*sizeof(char));
            strcpy(teliko,array);
        }
        free(array);
        array=malloc(100001*sizeof(char));

    }
    free(array);
    printf("%s\n",teliko);
    free(teliko);
    free(buffer1);
    free(buffer2);
    fclose(filetoread1);
    fclose(filetoread2);
    return 0;

    
}
