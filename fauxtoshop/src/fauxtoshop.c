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
    /* με την χρήση της συνάρτησης fread η οποία ορίζεται στο αρχείο κεφαλίδας stdio.h
    περνάμε τα δεδομένα/bytes από το ανοιχτό αρχείο - εικόνα (stdin-δείκτης σε
    ανοιχτό αρχείο) στον στατικό πίνακα χαρακτήρων headrers. */
    int  read=fread(headers,sizeof(char),54,stdin);
    /*η fread επιστρέφει τον αριθμό των δεδομένων που διάβασε. Έτσι δημιουργούμε την
    ακέραια μεταβλητή read που περιέχει αυτόν τον αριθμό */
    if(read<54){
        /*Σύμφωνα με την εκφώνηση τα bytes όσο αφορά το headers πρέπει να είναι >=54.
        Διαφορετικά το πρόγραμμα επιστρέφει με κωδικό εξόδου το 1 εκτυπωνοντας ανάλογο μήνυμα */
    
     fprintf(stderr,"sorry but the are not enough headers bytes in the file\n");
        exit(1);
    }
    /*Για να θεωρείται ένα αρχείο τύπου bmp πρέπει να ξεκινά με τους χαρακτήρες B, M.
    Διαφορετικά όπως ισχύει στην συγκεκριμένη περίπτωση επιστρέφει εμφανίζοντας κατάλληλο μήνυμα με
    exit code 1*/
    if((headers[0]!='B')||(headers[1]!='M')){
        fprintf(stderr,"the file is not BMP type\n");
        exit(1); 
    }
    int width,heigh;
    /* επειδή ο το όνομα του πίνακα  headers είναι δείκτης στο πρώτο στοιχείο και επειδή 
    είναι τυπου char τον μετατρέπουμε σε δείκτη τύπου Int καθώς χρειαζόμαστε 4 bytes για 
    την αναπαράσταση των ακέραιων μεγεθών */
    int file_size=*(unsigned int *)(headers+FILES); 
    int pixel_start=*(unsigned int *)(headers+START_PIXELS);
    width=*(unsigned int *)(headers+WI);
    heigh=*(unsigned int *)(headers+HI);
     /*η ακέραια μεταβλητή diafora μας δείχνει πόσα δεδομένα/bytes υπάρχουν ανάμεσα 
     στο pixel_start και τα headers bytes*/
    /*η fread αντιγράφει τα  diafora δεδομενα/bytes  από το stdin στον δυναμικό 
    πίνακα otherdata*/
    
    int diafora=pixel_start-54;
    otherdata=malloc(diafora*sizeof(char));
    fread(otherdata,sizeof(char),diafora,stdin);
    int photo_size=*(unsigned int *)(headers+PHOTOS);
    pixel_data=malloc(photo_size*sizeof(char));
    /*στον δυναμικό πίνακα pixel _data αντίγραφονται τα δεδομένα/bytes μαζί με το 
    padding της εικονας(bytes εικόνας παρέχονται από την μεταβλητή photo _size*/

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
    
