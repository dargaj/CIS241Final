
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
int NUMBER_STRINGS=10000;
int STRING_LENGTH=41;
char hashes[100000][41];

struct node
{
    char data[41];
    struct node *left;
    struct node *right;
};

struct node *rootNode;
/*
    Read contents of file
    Use parameter filename string
    Check if file exists
    Return string of file data
*/
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

/*
    Use file information to create a hash for file
    return file hash string
*/
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

/*
    Get hashes for all files in a folder
*/
void hashfolder(){
    char result[21];
    static char hexresult[41];
    char names[50];
    size_t offset;

    printf("Enter path to folder: ");
    scanf("%s", names);

    DIR *d;
    DIR *a;
    struct dirent *dir2;
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
                char *store=&filename[(strlen(filename)-5)];
                if (store[1]=='.'||store[0]=='.')
                {
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
        }
        closedir(d);
    }
}

/*
    Given a string, create a hash
    Return hash string
*/
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

struct node *newNode(char *datA){
    struct node *node1 = (struct node*)malloc(sizeof(struct node));
    strcpy(node1->data, datA);
    node1->left=NULL;
    node1->right=NULL;

    return node1;
}
/*
    Given an array of hashes, create a final hash
*/
int hashTreeLevel(char hashesA[][41], int size, struct node *nextLevel[], int numEnt){

    int STRING_LENGTH=41;
    struct node *level[size];
    int num=0;
    int counter=0;
    char *hexresult[41];
    int arraySize=0;
    int oldSize=sizeof(nextLevel);
    if (size%2==0)
        arraySize=size/2+1;
    else
        arraySize=size/2+1;
    char hashStore[arraySize][STRING_LENGTH];

    if (size<2)
    {
        *hexresult=hashesA[0];
        struct node *storeNode;
        storeNode=newNode(*hexresult);
        if (nextLevel!=NULL) {
            storeNode->left=nextLevel[counter];
            counter++;
            storeNode->right=nextLevel[counter];
            counter++;
        }
        rootNode=storeNode;
        printf("Final hash: %s\n\n\n", *hexresult);
        fprintf(fp,"\n\nFinal hash: %s\n\n", *hexresult);
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
    if (nextLevel==NULL) {
        for (int i=0;i<size;i++)
        {
            char *store = hashesA[i];
            struct node *storeNode;
            storeNode=newNode(store);
            level[i]=storeNode;
        }
    } else {
         for (int i=0;i<size;i++)
        {
            char *store = hashesA[i];
            struct node *storeNode;
            storeNode=newNode(store);
            storeNode->left=nextLevel[counter];
            counter++;

          if(i==size-1&&numEnt%2!=0)
                storeNode->right=NULL;
            else
            {
                storeNode->right=nextLevel[counter];
            }

            counter++;
            level[i]=storeNode;
        }
    }
    return 1+hashTreeLevel(hashStore, num, level, size);
}

/*
    Print given level of tree
*/
void printLevel(struct node *node1, int level)
{
    if (node1 == NULL)
        return;

    if (level==0)
        printf("%s\n", node1->data);
    else
    {
        printLevel(node1->left, level - 1);
        printLevel(node1->right, level - 1);
    }

}

/*
    Print given amount of levels of tree
*/
void printElements(struct node *node)
{
    int i;
    int levelCount = height(node);
    //int levelCount=2;
    for (i = 0; i < levelCount; i++)
    {
        printLevel(node, i);
        printf("\n");
    }
}

/*
    Print given level of tree
*/
void printLevelFile(struct node *node1, int level)
{
    if (node1 == NULL)
        return;

    if (level==0)
        fprintf(fp,"%s\n", node1->data);
    else
    {
        printLevelFile(node1->left, level - 1);
        printLevelFile(node1->right, level - 1);
    }

}

/*
    Print given amount of levels of tree
*/
void printElementsFile(struct node *node)
{
    int i;
    int levelCount = height(node);
    //int levelCount=2;
    for (i = 0; i < levelCount; i++)
    {
        printLevelFile(node, i);
        fprintf(fp,"\n");
    }
}
/* Compute the "height" of a tree
*/
int height(struct node* node)
{
    int lheight;
    if (node==NULL)
        return 0;
    else
    {
        lheight = height(node->left);
    }
    return lheight+1;
}

int main()
{
    char *hexresult[41];
    char filename[50];
    char filename2[50];
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
                //hashtree(hashes, countEntries);
                hashTreeLevel(hashes, countEntries, NULL,0);
                break;
            case 5:
                printf("Exiting..\n");
                exit(0);

            default:
                fflush(stdin);
                printf("Invalid selection. Please try again\n");
        }
    }
    fprintf(fp,"\n");
    printElementsFile(rootNode);
    fclose(fp);
    userChoice=0;
    while (userChoice!=3)
    {
        printf("1) Print Hash Tree\n");
        printf("2) Compare hash to file\n");
        printf("3) Exit\n\n");
        fflush(stdin);
        scanf("%d", &userChoice);
            switch(userChoice)
            {
                case 1:
                        printf("\n\nHash Tree: \n");
                        printElements(rootNode);
                        break;
                case 2:
                    printf("Enter previous hash file name: ");
                    scanf("%s", filename2);
                    printf("\n");
                    char *result2=readFile(filename);
                    char *result3=readFile(filename2);
                    int x =strcmp(result3, "");
                    if (x==0)
                    {
                        printf("Try Again\n\n");
                        break;
                    }
                    if (result3==NULL)
                    {
                        printf("Try Again\n\n");
                        break;

                    }
                    char *sub1;
                    char *sub2;
                    printf("\n\nFile %s \n", filename);

                    sub1=strstr(result2, "Final");
                    printf("%.52s\n", sub1);
                    fflush(stdin);
                    printf("\nFile %s \n", filename2);
                    sub2=strstr(result3, "Final");
                    if(sub2[0]!='F')
                    {
                        printf("Invalid file format\n");
                        break;
                    }
                    printf("%.52s\n\n", sub2);
                    fflush(stdin);
                    int a=strncmp(sub1,sub2, 50);
                    if (a == 0)
                        printf("Files are the same\n\n\n");
                    else
                        printf("Files have changed\n\n\n");

                    break;
                case 3:
                    printf("Exiting..\n");
                    exit(0);

                default:
                    fflush(stdin);
                    printf("Invalid selection. Please try again\n");
            }
    }


    return 0;
}
