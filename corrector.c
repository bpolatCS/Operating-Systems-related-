#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>


const char *fileExt(const char *fileName) {
    const char *d = strrchr(fileName, '.');

    if(!d || d == fileName) 
    {
        return "";
    }
    return d + 1;
}


void listAllFiles(const char* dirname, char name[15], char surname[15], char gender[3]){
    DIR *dir = opendir(dirname);
    if(dir == NULL){
        return;
    }
   
    struct dirent* entity;
    entity = readdir(dir);

    while(entity != NULL){
        
        FILE * fileToBeOpenned;
        if(entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name,"..") != 0 ){

            char p[100]= {0}; //characters hopefully enough
            strcat(p, dirname);
            strcat(p, "/");
            strcat(p, entity->d_name);
            
            listAllFiles(p,name,surname,gender);
        }
        else if (entity->d_type == DT_REG && strcmp(fileExt(entity->d_name), "txt") == 0 && !(strcmp(entity->d_name, "database.txt") == 0 && strcmp(dirname, ".") == 0)) 
        {
            
            char allTheOtherPaths[50] = {0};
            
            strcat(allTheOtherPaths, dirname);
            strcat(allTheOtherPaths, "/");
            strcat(allTheOtherPaths, entity->d_name);
            fileToBeOpenned = fopen(allTheOtherPaths, "r+");
            char buffer[300];
            bool checked =false;



             while(fscanf(fileToBeOpenned, "%s", buffer) != EOF){
               
                if(strcmp(buffer, name) == 0){
                    checked = true;
                }
                if(checked == true){
                    if(gender[0] == 'm'){
     
                        fseek(fileToBeOpenned, -strlen(buffer) - 4, SEEK_CUR);
                       
                        fputs("Mr.", fileToBeOpenned);
                        
                        fseek(fileToBeOpenned, strlen(buffer) + 2, SEEK_CUR);
                       
                        fputs(surname, fileToBeOpenned);
                        
                    }
                    if(gender[0] == 'f'){
                        fseek(fileToBeOpenned, -strlen(buffer) - 4, SEEK_CUR);
                        fputs("Ms.", fileToBeOpenned);
                        fseek(fileToBeOpenned, strlen(buffer) + 2, SEEK_CUR);
                        fputs(surname, fileToBeOpenned);
                    }
                }
                checked = false;


                
            } 
            fclose(fileToBeOpenned);
        }

        entity = readdir(dir);
    }
       
}

int main(int argc, char* argv[]){

    FILE *database;
 
    database=fopen("database.txt", "r");

   
    char buffer[300];
    int startidx, endidx, j, counter;
    char name[15];
    char surname[15];
    char gender[3];

    counter = 0;
    
    while(fscanf(database, "%s", buffer) != EOF){
        if(counter == 0){
            strcpy(gender, buffer);
            counter++;
        }
        else if(counter == 1){
            strcpy(name, buffer);
            counter++;
        }
        else if(counter == 2){
            strcpy(surname, buffer);
            listAllFiles(".",name,surname,gender);
            counter = 0;
        }
    }
    fclose(database);
 
    return 0;
}



