#include <stdio.h>
#include <stdlib.h>
//οι παραπάνω σταθερές προκύπτουν από την εκφώνηση της άσκησης 
#define FILES 2 /* στο 2ο byte της κεφαλίδας(headers) περιέχεται το μέγεθος του αρχείου
σε bytes με την μορφή little endian*/
#define START_PIXELS 10 /*στο 10ο byte της κεφαλίδας(headers) περιέχεται το offset του
    πίνακα των pixel σε μορφή little endian, δηλαδή το byte από το οποίο αρχίζουν τα
pixels της εικόνας */


 
#define WI 18 /*στο 18ο byte τηε κεφαλίδας (headers) περιέχεται το πλατος(width) σε   

    pixels με την μορφή little endian*/


 
#define HI 22 /*στο 22ο byte της κεφαλίδας (headers) περιέχεται το ύψος (heigh) σε
    pixels με την μορφή little endian*/
#define PHOTOS 34 /*στο 34ο byte της κεφαλίδας (headers) περιέχεται το μέγεθος της εικόνας 
    σε bytes με την μορφή little endian*/



int main(void){
    char headers[54];
    char *otherdata;
    char *pixel_data;
    int  read=fread(headers,sizeof(char),54,stdin);
    if(read<54){
        fprintf(stderr,"sorry but the are not enough headers bytes in the file\n");
        exit(1);
    }
    if((headers[0]!='B')||(headers[1]!='M')){
        fprintf(stderr,"the file is not BMP type\n");
        exit(1); 
    }
    int width,heigh;
    int file_size=*(unsigned int *)(headers+FILES); 
    int pixel_start=*(unsigned int *)(headers+START_PIXELS);
    width=*(unsigned int *)(headers+WI);
    heigh=*(unsigned int *)(headers+HI);
    int diafora=pixel_start-54;
    otherdata=malloc(diafora*sizeof(char));
    fread(otherdata,sizeof(char),diafora,stdin);
    int photo_size=*(unsigned int *)(headers+PHOTOS);
    pixel_data=malloc(photo_size*sizeof(char));
    fread(pixel_data,sizeof(char),photo_size,stdin);
    //array for pixel only bytes without padding
    char ***image_array=malloc(heigh*sizeof(char **));
    int i,j,k;
    for(i=0;i<heigh;i++){
        image_array[i]=malloc(width*sizeof(char*));
        for(j=0;j<width;j++){
            image_array[i][j]=malloc(3*sizeof(char));
        }
    }
    int count=0;
    for(i=0;i<heigh;i++){
        for(j=0;j<width;j++){
            for(k=0;k<3;k++){
                if(pixel_data[count]=='\0'){
                    count++;
                    continue;
                }
                image_array[i][j][k]=pixel_data[count];
                count++;
                
            }

        }
    }
    int new_heigh=width;
    int new_width=heigh;
    char ***rotate_array=malloc(new_heigh*sizeof(char **));
    for(i=0;i<new_heigh;i++){
        rotate_array[i]=malloc(new_width*sizeof(char *));
        for(j=0;j<new_width;j++){
            rotate_array[i][j]=malloc(3*sizeof(char));
        }
    }
    //rotate
    for(i=0;i<heigh;i++){
        for(j=0;j<width;j++){
            for(k=0;k<3;k++){
                rotate_array[width-1-j][i][k]=image_array[i][j][k];
            }
        }
    }
    int padding=(new_width*3)%4;
    *(unsigned int *)(headers +WI) = new_width;
    *(unsigned int *)(headers + HI) = new_heigh;
    *(unsigned int *)(headers + FILES) = 54 + diafora + (new_width * 3 * new_heigh) + (padding * new_heigh);
    *(unsigned int *)(headers +PHOTOS) = (new_width * 3 * new_heigh) + (padding * new_heigh * 3);
    fwrite(headers,sizeof(char),54,stdout);
    fwrite(otherdata,sizeof(char),diafora,stdout);
    for(i=0;i<new_heigh;i++){
        for(j=0;j<new_width;j++){
            for(k=0;k<3;k++){
                putchar(rotate_array[i][j][k]);
            }
        }
        for(k=0;k<padding;k++){
            putchar('\0');
        }
    }
    for (int i = 0; i < new_heigh; i++) {
        for (int j = 0; j < new_width; j++) {
            free(rotate_array[i][j]);
        }
        free(rotate_array[i]);
    }
    free(rotate_array);
    for (int i = 0; i < heigh; i++) {
        for (int j = 0; j < width; j++) {
            free(image_array[i][j]);
        }
        free(image_array[i]);
    }
    free(image_array);
    free(otherdata);
    free(pixel_data);
    return 0;
}
    
