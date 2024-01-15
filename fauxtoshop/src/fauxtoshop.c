#include <stdio.h>
#include <stdlib.h>
 //οι παρακάτω σταθερές προκύπτουν από την εκφώνηση της άσκησης
#define FILES 2  // στο 2ο byte της κεφαλίδας(headers) περιέχεται το μέγεθος του αρχείου σε bytes με την μορφή little endian
#define START_PIXELS 10 //στο 10ο byte της κεφαλίδας(headers) περιέχεται το offset του πίνακα των pixel σε μορφή little endian
//δηλαδή το byte από το οποίο αρχίζουν τα pixels της εικόνας 

 
#define WI 18  //στο 18ο byte της κεφαλίδας (headers) περιέχεται το πλατος(width) της εικόνας σε pixels με την μορφή little endian

 
#define HI 22 //στο 22ο byte της κεφαλίδας (headers) περιέχεται το ύψος (heigh) της εικόνας σε pixels με την μορφή little endian
#define PHOTOS 34 //στο 34ο byte της κεφαλίδας (headers) περιέχεται το μέγεθος της εικόνας σε bytes με την μορφή little endian
#define BYTES 28//στο 28ο byte βρίσκεται ο αριθμός των bit που χρησιμοοιεί το αρχείο τύπου bmp για την αναπαράσταση χρώματος

int main(void){
    char headers[54];
    char *otherdata;
    char *pixel_data;

    int  read=fread(headers,sizeof(char),54,stdin);
     //η fread επιστρέφει τον αριθμό των δεδομένων που διάβασε. Έτσι δημιουργούμε την ακέραια μεταβλητή read που περιέχει αυτόν τον αριθμό

    if(read<54){

        exit(1);
 //Σύμφωνα με την εκφώνηση τα bytes όσο αφορά το headers πρέπει να είναι >=54. Διαφορετικά το πρόγραμμα επιστρέφει με κωδικό
//εξόδου το 1
 //εκτενέστερος έλεγχος για το πλήθος των κεφαλίδων/headers γίνεται στην συνέχεια του κώδικα

    }
 
    if((headers[0]!='B')||(headers[1]!='M')){
        exit(1); 
//Για να θεωρείται ένα αρχείο τύπου bmp πρέπει να ξεκινά με τους χαρακτήρες B, M.Διαφορετικά όπως ισχύει στην συγκεκριμένη
//περίπτωση επιστρέφει  με exit code 1
    }
    int width,heigh;
//επειδή ο το όνομα του πίνακα  headers είναι δείκτης στο πρώτο στοιχείο και επειδή είναι τυπου char τον μετατρέπουμε σε δείκτη
//τύπου int καθώς χρειαζόμαστε 4 bytes για  την αναπαράσταση των ακέραιων μεγεθών που απαιτούνται για την εκπόνηση της άσκησης 
    
    //int file_size=*(unsigned int *)(headers+FILES); 
//δεν χρησιμοποιείται κάπου στον κώδικα η ακέραια μεταβλητή file_size.Απλά επειδή η εκφώνηση της άσκησης μας παρέχει την
//συγκεκριμένη πληροφορία την βάλαμε έστω και σαν σχόλιο
    int pixel_start=*(unsigned int *)(headers+START_PIXELS);
      if(pixel_start<54){
        exit(1);
        //εκτενέστερος έλεγχος του πλήθους της κεφαλίδας για το οποίο έγινε αναφορά στην αρχή του προγράμματός μας

    }
    width=*(unsigned int *)(headers+WI);//με την ακέραια μεταβλητή width δηλώνεται το πλάτος της εικόνας
    heigh=*(unsigned int *)(headers+HI);//με την ακέραια μεταβλητή heigh δηλώνεται το ύψος της εικόνας

    int bits=*(unsigned int *)(headers+BYTES);
//μονοχρωματικά BMP αρχεία δεν είναι δεκτά.Αυτό σημαίνει ότι κάθε αρχείο τύπου BMP πρέπει να χρησιμοποιεί 24-bits για την
//αναπαράσταση χρώματος
    if(bits != 24){
         exit(1);
        //μονοχρωματικό αρχείο BMP

    }
 //η ακέραια μεταβλητή diafora μας δείχνει πόσα δεδομένα/bytes υπάρχουν ανάμεσα στο pixel_start και τα headers bytes

    int diafora=pixel_start-54;
    otherdata=malloc(diafora*sizeof(char));

    int read2=fread(otherdata,sizeof(char),diafora,stdin);
//η fread αντιγράφει τα  diafora δεδομενα/bytes  από το stdin στον δυναμικό  πίνακα otherdata,ενώ αν αποτύχει για οποινδήποτε λόγο
//όπως γίνεται στον ακριβώς επόμενο έλεγχο με την χρήση της μεταβλητής read2 το πρόγραμμα τερματίζει με exit code το 1,απελευθερώνοντας
//μνήμη για τον πίνακα otherdata
    if (read2 != diafora) {

        free(otherdata);

         exit(1);}
    int photo_size=*(unsigned int *)(headers+PHOTOS);
    pixel_data=malloc(photo_size*sizeof(char));

   int read3= fread(pixel_data,sizeof(char),photo_size,stdin);
//στον μονοδιάστατο δυναμικό πίνακα pixel _data αντίγραφονται μέσω επίσης της συνάρτησης fread τα δεδομένα/bytes μαζί με το
//padding της εικονας(bytes εικόνας παρέχονται από την μεταβλητή photo _size.Aν αποτύχει στο διάβασμα όπως φαίνεται παρακάτω το
//πρόγραμμα επιστρέφει με κωδικό εξόδου το 1,απελευθερώνοντας μνήμη για τους δυναμικούς πίνακες που είχαν δημιιουργηθεί μέχρι στιγμής
    if (read3 != photo_size) {

        free(otherdata);

        free(pixel_data);

        exit(1);

    }
    //στον τρισδιάστατο δυναμικό πίνακα image_array θα περάσω τα pixels της εικόνας χωρίς τα padding bytes

    char ***image_array=malloc(heigh*sizeof(char **));
    int i,j,k;
    for(i=0;i<heigh;i++){
        image_array[i]=malloc(width*sizeof(char*));
        for(j=0;j<width;j++){
            image_array[i][j]=malloc(3*sizeof(char));
        }
    }
    int count=0;//χρησιμοποιώ την ακέραια μεταβλητή count για να έχω πρόσβαση στα δεδομένα του μονοσιάστατου πίνακα pixel_data
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
    //αλλάζω τις διαστάσεις προκειμένου να ξεκινήσω την περιστροφή της εικόνας κατά 90 μοίρες

    int new_heigh=width;
    int new_width=heigh;
     //Ο δυναμικός τρισδυάστατος πίνακας rotate_array θα περιέχει τα στοιχεία του πίνακα image_array περιστρεφόμενα κατά 90 μοίρες

    char ***rotate_array=malloc(new_heigh*sizeof(char **));
    for(i=0;i<new_heigh;i++){
        rotate_array[i]=malloc(new_width*sizeof(char *));
        for(j=0;j<new_width;j++){
            rotate_array[i][j]=malloc(3*sizeof(char));
        }
    }

    for(i=0;i<heigh;i++){
        for(j=0;j<width;j++){
            for(k=0;k<3;k++){
                rotate_array[width-1-j][i][k]=image_array[i][j][k];
            }
// αν παρατηρήσουμε τον αρχικό πίνακα και τον τον επιθυμητό
//βγαίνει αυτή η συνθήκη ενδιάμεσα στα κελιά των δύο πινάκων 

        }
    }
    int padding=(new_width*3)%4;//υπολογισμός του padding
    //αλλαγή στα δεδομένα των headers εξαιτίας της περιστροφής

    *(unsigned int *)(headers +WI) = new_width;
    *(unsigned int *)(headers + HI) = new_heigh;
    *(unsigned int *)(headers + FILES) = 54 + diafora + (new_width * 3 * new_heigh) + (padding * new_heigh);
    *(unsigned int *)(headers +PHOTOS) = (new_width * 3 * new_heigh) + (padding * new_heigh * 3);
    
    fwrite(headers,sizeof(char),54,stdout);//εκτυπώνω τα headers αλλαγμένα στο αρχείο εξόδου BMP(stdout-δείκτης σε ανοιχτό αρχείο)

    fwrite(otherdata,sizeof(char),diafora,stdout);//επιστρέφω τα δεδομένα του πίνακα otherdata στο stdout
    for(i=0;i<new_heigh;i++){
        for(j=0;j<new_width;j++){
            for(k=0;k<3;k++){
                putchar(rotate_array[i][j][k]);//εκτυπώνω στο stdout τα pixel  και στο τέλος κάθε γραμμής τα padding bytes
            }
        }
        for(k=0;k<padding;k++){
            putchar('\0');
        }
    }
    //αποδέσμευση μνήμης για τους δυναμικόυς πίνακες rotate_array,image_array,pixel_data και otherdata.

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
    return 0;//αν όλα πάνε καλά το πρόγραμμα επιστρέφει με exit code το 0
}
    

