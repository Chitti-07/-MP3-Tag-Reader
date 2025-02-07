#include<stdio.h>
#include"types.h"

#define possize 6

typedef struct Tag_Reader 
{
    char *Filename;
    FILE *fptr_fname;
    
    char *Title;
    int Title_size;

    char *album;
    int Album_size;

    char *Artist;
    int Artist_size;

    char *Year;
    int Year_size;

    char *Genre;
    int Genre_size;

    char *Comment;
    int Com_size;

    int pos[possize];

}Tag_Reader;

/*check operation type*/
Operation checkOperation(char **argv);

/*function to print*/
void printHelp();

/*To view all the info*/
Status viewInfo(Tag_Reader *tagReader);

/*to validate the file mp3 or not*/
Status readandvalidate(Tag_Reader *tagReader,char *argv);

/*To open files*/
Status openFiles(Tag_Reader *tagReader);

/*To check header and version */
Status checkheaderandversion(FILE *fname);

/*To readInfo*/
Status readInfo(Tag_Reader *tagReader);

/*to read tag*/
tags tagreader(FILE *fname);

/*To read tag size*/
void tagsizereader(FILE *fname,int *size);

/*To read tag details*/
void readtaginfo(FILE *fname,char **name,int size);

/*to print the information*/
Status printInfo(Tag_Reader *tagReader);

/* Endianess conversions */
void endians_conversion(unsigned int *num);