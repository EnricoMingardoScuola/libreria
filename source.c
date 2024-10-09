#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define STRING_LEN 50
#define LIBRARY_LEN 50
#define GENRE_LEN 30
#define OPTIONS_LEN 3

typedef struct{
    char title[STRING_LEN];
    char author[STRING_LEN];
    int year;
    double price;
    char genre[STRING_LEN];
}Book;

const char *RetrieveStringFromFile(FILE *filestream,char separator){ //ritorna una stringa da un file fino a quando non trova lo specificato separatore
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
    printf("Price: %.2lf, ", b1.price);
    printf("Genre: %s\n",b1.genre);
}

char *uniqueString(char *strArray[], int arrLen, char *string){ //controlla se una stringa è unica all'interno di un array di stringhe
    char *result;
    for(int i=0; i<arrLen; i++){
            if(!strcmp(string,strArray[i]))
                return NULL;
    }
    result = malloc(strlen(string)+1);
    strcpy(result,string);
    return result;
}

void PrintOptions(char **options,int len){
    for(int i=0; i<len; i++)
        printf("[%d]: %s\n",i+1,options[i]);
}

void PrintBooksGenre(Book **library,int len,char *genre){ //printa tutti i libri di un certo genere
    for(int i=0; i<len;i++){
        if(!strcmp(library[i]->genre,genre))
            PrintBook(*library[i]);
    }
}

Book *SearchBook(Book **library,int len, char *title){ //cerca un libro in base al titolo
    for(int i=0; i<len; i++){
        if(!strcmp(library[i]->title,title)){
            return library[i];
        }
    }
    return NULL;
}

int main(int argc,char **argv){
    Book *library[LIBRARY_LEN];
    char separator = ',';
    int libraryIndex = 0;
    char *genres[GENRE_LEN];
    int genreIndex = 0;
    char *temp;
    char titleSearch[STRING_LEN];
    Book *tempBook;
    char *options[OPTIONS_LEN] = {
        "Search a book by the title",
        "Display all of the books of a certain genre",
        "Display all of the books contained in the library"
    };
    int answer;
    int genreChoice;
    if(argc!=2 || strlen(argv[1]) > STRING_LEN-1){ //controllo dei parametri di input
        printf("Parametri a linea di comando errati; format corretto: ./libreria.exe nomeFileOrigineDati.estensione\n");
        return -1;
    }
    FILE *originData = fopen(argv[1],"r");
    if(originData == NULL){
        printf("Error while opening the file\n");
        exit(EXIT_FAILURE);
    }
    while(!feof(originData) && libraryIndex<LIBRARY_LEN){ //caricamento dei dati nell'array
        library[libraryIndex] = malloc(sizeof(Book));
        strcpy(library[libraryIndex]->title, RetrieveStringFromFile(originData,separator));
        strcpy(library[libraryIndex]->author, RetrieveStringFromFile(originData,separator));
        library[libraryIndex]->year = atoi(RetrieveStringFromFile(originData,separator));
        library[libraryIndex]->price = atof(RetrieveStringFromFile(originData,separator));
        strcpy(library[libraryIndex]->genre, RetrieveStringFromFile(originData,separator));
        libraryIndex++;
    }
    free(library[libraryIndex-1]); //cancella l'ultima riga vuota
    libraryIndex--;
    if(libraryIndex > LIBRARY_LEN) //controlla se ha caricato tutti i dati
    {
        printf("Massimo spazio allocato superato, non tutti i libri sono stati caricati nel database\n");
    }
    if(!fclose(originData))
        perror("Errore nella chiusura del file");
    for(int i=0; i<libraryIndex; i++){ //aggiunge all'array delle categorie ogni categoria contenuta nella libreria
        temp = uniqueString(genres, genreIndex, library[i]->genre);
        if (temp != NULL) {
            genres[genreIndex] = malloc(strlen(temp)+1);
            strcpy(genres[genreIndex],temp);
            genreIndex++;
        }
    }
    do{
        printf("----------------------------------------------\n");
        printf("Do you want to: \n");
        PrintOptions(options,OPTIONS_LEN);
        printf("[%d]: EXIT\n",OPTIONS_LEN+1);
        printf("Your choice: ");
        scanf("%d",&answer);
        switch(answer){
            case 1: printf("What is the title of the book you would like to search?\nTitle: ");
                    fgetc(stdin); //toglie il \n dello scanf cosi fgets puo leggere la nuova stringa
                    fgets(titleSearch,STRING_LEN,stdin);
                    titleSearch[strlen(titleSearch)-1] = '\0'; //rimpiazza il \n
                    tempBook = SearchBook(library,libraryIndex,titleSearch);
                    if(tempBook == NULL)
                        printf("No book in the library matches that title\n");
                    else
                        PrintBook(*tempBook);
                    break;
            case 2: printf("Please choose one of the following genres: \n");
                    PrintOptions(genres,genreIndex);
                    printf("Your choice: ");
                    scanf("%d",&genreChoice);
                    printf("Here are all of the books currently in the library with that genre: \n");
                    PrintBooksGenre(library,libraryIndex,genres[genreChoice-1]);
                    break;
            case 3: printf("The library contains these books: \n");
                    for(int i=0; i<libraryIndex;i++)
                        PrintBook(*library[i]);
                    break;
            case OPTIONS_LEN+1: break;
            default: printf("Invalid number"); break;
        }
    }while(answer != OPTIONS_LEN+1);
    //free memory
    for(int i=0;i<libraryIndex;i++){
        free(library[i]);
    }
    for(int i=0; i<genreIndex; i++){
        free(genres[i]);
    }
    return 1;
}