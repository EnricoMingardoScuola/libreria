#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define STRING_LEN 50
#define LIBRARY_LEN 50

typedef struct{
    char title[STRING_LEN];
    char author[STRING_LEN];
    int year;
    double price;
}Book;

const char *RetriveStringFromFile(FILE *filestream,char separator){
    char *string;
    string = malloc(STRING_LEN);
    char character = 0;
    int numberofchars = 0;
    while(character != separator && character != '\n' && character != EOF)
    {
        character = fgetc(filestream);
        if(character != separator && character != '\n' && character != EOF){
            strncat(string,&character,1);
            numberofchars++;
        }
    }
    string[numberofchars] = '\0';
    return string;
}

void PrintBook(Book b1){
    printf("Title: %s, ", b1.title);
    printf("Author: %s, ", b1.author);
    printf("Year of publication: %d, ", b1.year);
    printf("Price: %.2lf\n", b1.price);
}

int main(int argc,char **argv){
    Book *library[LIBRARY_LEN];
    int libraryIndex = 0;
    if(argc!=2){
        printf("Parametri a linea di comando errati; format corretto: ./libreria.exe nomeFileOrigineDati.estensione\n");
        return -1;
    }
    FILE *originData = fopen(argv[1],"r");
    if(originData == NULL){
        printf("Error while opening the file\n");
        exit(EXIT_FAILURE);
    }
    while(!feof(originData)){
        library[libraryIndex] = malloc(sizeof(Book));
        strcpy(library[libraryIndex]->title, RetriveStringFromFile(originData,','));
        strcpy(library[libraryIndex]->author, RetriveStringFromFile(originData,','));
        library[libraryIndex]->year = atoi(RetriveStringFromFile(originData,','));
        library[libraryIndex]->price = atof(RetriveStringFromFile(originData,','));
        libraryIndex++;
    }
    fclose(originData);
    for(int i=0; i<libraryIndex; i++){
        PrintBook(*library[i]);
    }
    return 1;
}