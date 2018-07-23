
/******************************************************************************
 *  Name:           Jessica Darga
 *  Date:           7/20/2018
 *  Class:          CIS241-01
 *  Instructor:     Dr. Vijay Bhuse
 *  Assignment:     Final Project
 *  Description:    Project to input files and hash each file. Final has will be
 *                  output
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <dirent.h>

int countEntries=0;
FILE *fp;
char hashes[1000][41];



char *readFile(char *filename) {
    if(access(filename, F_OK)==-1) {
        printf("File does not exist..\n");
        return "";
    }
    FILE *f = fopen(filename, "rt");
    assert(f);
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buffer = (char *) malloc(length + 1);
    buffer[length] = '\0';
    fread(buffer, 1, length, f);
    fclose(f);
    return buffer;
}

char* hashone() {
    char result[21];
    static char hexresult[41];
    char filename[50];
    size_t offset;

    printf("Enter path to file with filename: ");
    scanf("%s", filename);

    char *content =readFile(filename);
    if (strlen(content)>0) {
        fprintf(fp, "%s\n",filename);
         fflush(fp);
        SHA1(result, content, strlen(content));

        for( offset = 0; offset < 20; offset++) {
            sprintf( ( hexresult + (2*offset)), "%02x", result[offset]&0xff);
        }
        fflush(stdin);
        printf("\n");
        countEntries++;
        return hexresult;
    }
    return "\n ";

}
char* hashFile(char* filename) {
    char result[21];
    static char hexresult[41];
    //char filename[50];
    size_t offset;

    char *content =readFile(filename);
    if (strlen(content)>0) {
        fprintf(fp, "%s\n",filename);
         fflush(fp);
        SHA1(result, content, strlen(content));

        for( offset = 0; offset < 20; offset++) {
            sprintf( ( hexresult + (2*offset)), "%02x", result[offset]&0xff);
        }
        fflush(stdin);
        printf("\n");
        countEntries++;
        return hexresult;
    }
    return "\n ";

}
void hashfolder(){
    char result[21];
    static char hexresult[41];
    char names[50];
    size_t offset;

    printf("Enter path to folder: ");
    scanf("%s", names);

    DIR *d;
    struct dirent *dir;
    d=opendir(names);
    if (d) {
        while((dir=readdir(d))!=NULL) {
            if(dir->d_name[0]!='.')
            {
                char filename[50];
                strcpy(filename,names);
                strcat(filename,"/");
                strcat(filename,dir->d_name);
                printf("Hashing file: %s\n", filename);
                char *content =readFile(filename);
                if (strlen(content)>0) {
                    SHA1(result, content, strlen(content));
                    for( offset = 0; offset < 20; offset++) {
                        sprintf( ( hexresult + (2*offset)), "%02x", result[offset]&0xff);
                    }
                    strcpy(hashes[countEntries], hexresult);
                    countEntries++;
                    fprintf(fp, "%s\n",filename);
                    fflush(fp);
                }
            }
        }
        closedir(d);
    }


}
char* hashString(char *content) {
    char result[21];
    static char hexresult[41];
    size_t offset;
    if (strlen(content)>0) {
        SHA1(result, content, strlen(content));

        for( offset = 0; offset < 20; offset++) {
            sprintf( ( hexresult + (2*offset)), "%02x", result[offset]&0xff);
        }
    }
    return hexresult;

}
int hashtree(char hashesA[][41], int size){

    int STRING_LENGTH=41;
    int num=0;
    char *hexresult[41];
    int arraySize=0;
    if (size%2==0)
        arraySize=size/2+1;
    else
        arraySize=size/2+1;

    char hashStore[arraySize][STRING_LENGTH];
    if (size<2)
    {
        *hexresult=hashesA[0];
        printf("Final hash: %s\n", *hexresult);
        fprintf(fp,"\n\nFinal hash: %s\n", *hexresult);
        fflush(fp);
        return 1;
    }
    for (int i=0;i<size;i++) {
        if (i != size-1)
        {
            char *result[STRING_LENGTH*2+1];
            strncpy(result, hashesA[i], strlen(hashesA[i])+1);
            strncat(result, hashesA[i+1], strlen(hashesA[i+1]));
            *hexresult=hashString(result);
            strcpy(hashStore[num], *hexresult);
            num++;
            i++;
        }
        else {
            if (size%2!=0)
            {
                char *result[STRING_LENGTH+1];
                *hexresult=hashesA[size-1];
                strcpy(hashStore[num], *hexresult);
                num++;
            }
        }
    }
    printf("\nFile hashes: \n");
    for (int i=0;i<num;i++)
        printf(" %s\n",hashStore[i]);
    printf("\n");
    return 1+hashtree(hashStore, num);

}
int main()
{
    int NUMBER_STRINGS=10000;
    int STRING_LENGTH=41;
    char *hexresult[41];
    char *result[41];
    char filename[50];
    char filename2[50];
    char *result1[41];
    int userChoice;

    printf("Enter file name to save hash information to (ex hash.txt): ");
    scanf("%s", filename);
    fp=fopen(filename,"w+");
    fprintf(fp,"File Names: \n");
    fflush(fp);
    fflush(stdin);

    while (userChoice!=4)
    {
        printf("\nMerkle Hash Tree\n");
        printf("1) Enter file name\n");
        printf("2) Enter directory name\n");
        printf("3) Print file hashes\n");
        printf("4) Stop file entry, get hash\n");
        printf("5) Exit\n");
        scanf("%d", &userChoice);
        switch(userChoice)
        {
            case 1:
                *hexresult=hashone();
                if (strlen(*hexresult)<20) {
                    break;
                }
                else{
                    strcpy(hashes[countEntries-1], *hexresult);
                    fflush(stdin);
                    break;
                }
            case 2:
                hashfolder();
                break;
            case 3:
                printf("File hashes..\n");
                for (int i=0;i<countEntries;i++)
                    printf("%s\n",hashes[i]);
                printf("\n");
                break;
            case 4:
                printf("Get hash..\n");
                hashtree(hashes, countEntries);
                break;
            case 5:
                printf("Exiting..\n");
                exit(0);

            default:
                fflush(stdin);
                printf("Invalid selection. Please try again\n");
        }
    }
    fclose(fp);
    printf("Would you like to compare hash to previous hash?\n");
    printf("1) yes   2) quit:  ");
    fflush(stdin);
    scanf("%d", &userChoice);
    if (userChoice==1)
    {

        printf("Enter previous has file name: ");
        scanf("%s", filename2);
        char *result2=readFile(filename);
        char *result3=readFile(filename2);
        char *sub1;
        char *sub2;
        printf("\n\nFile %s \n", filename);

        sub1=strstr(result2, "Final");
        printf("%s\n", sub1);
        fflush(stdin);
        printf("\nFile %s \n", filename2);
        sub2=strstr(result3, "Final");
        printf("%.52s\n\n", sub2);
        fflush(stdin);
        int a=strncmp(sub1,sub2, 50);
        if (a == 0)
            printf("Files are the same\n");
        else
            printf("Files have changed\n");



    }
    return 0;
}
