#include <stdio.h>
#include "view.h"
#include <string.h>
#include <stdlib.h>

/* Function to convert little to big endianess vice-a-versa */
void endians_conversion(unsigned int *num)
{
    char temp;
    int start = 0;
    int end = 3;
    while(start<end)
    {
        temp = *((char*)num+start);
        *((char*)num+start) = *((char*)num+end);
        *((char*)num+end) = temp;
        start++;
        end--;
    }
}

/* Function to check user option */
Operation checkOperation(char **argv)
{
    if (strcmp(argv[1], "-e") == 0)
        return Edit;
    else if (strcmp(argv[1], "-v") == 0)
        return View;
    else
        return Help;
}

/* Function to guide / provide help to user */
void printHelp()
{
    printf("\n.............Help Menu.....................\n\n");
    printf("1. -v -> to view mp3 file contents\n");
    printf("2. -e -> to edit mp3 file contents\n");
    printf(" 2.1. -t -> to edit song title\n");
    printf(" 2.2. -a -> to edit artist name\n");
    printf(" 2.3. -A -> to edit album name\n");
    printf(" 2.4. -y -> to edit song year\n");
    printf(" 2.5. -m -> to edit song content\n");
    printf(" 2.6. -c -> to edit song comment\n");
    printf("\n............................................\n\n");
}

/* Function to check/validate the mpeg file */
Status readandvalidate(Tag_Reader *tagReader, char *arg)
{
    if (strcmp(strstr(arg, "."), ".mpeg") == 0)
    {
        tagReader->Filename = arg;
        return success;
    }
    else
    {
        printf("\n..................................\n\n");
        printf("ERROR: Mp3 File Type only\n");
        printf("\n..................................\n\n");
    }
    return success;
}

/* Function to open mpeg file */
Status openFiles(Tag_Reader *tagReader)
{
    tagReader->fptr_fname = fopen(tagReader->Filename, "rb+");
    if (tagReader->fptr_fname == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", tagReader->Filename);
        return failure;
    }
    return success;
}

/* Function to check mpeg header and header version */
Status checkheaderandversion(FILE *fname)
{
    char head[4];
    char ver[2];
    fread(head, 3, 1, fname);
    head[3] = '\0';
    fread(ver, 2, 1, fname);
    if (strcmp(head, "ID3") == 0 && ver[0] == 0x03 && ver[1] == 0x00)
    {
        return success;
    }
    return failure;
}

/* Function to view mpeg information */
Status viewInfo(Tag_Reader *tagReader)
{
    if (openFiles(tagReader) == success)
    {
        if (checkheaderandversion(tagReader->fptr_fname) == success)
        {
            if (readInfo(tagReader) == success)
            {
                printInfo(tagReader);
            }
        }
    }
    return success;
}

/* Function to read mpeg file */
Status readInfo(Tag_Reader *tagReader)
{
    rewind(tagReader->fptr_fname);
    /* Skippint the header & header version */
    fseek(tagReader->fptr_fname, 10, SEEK_CUR);
    for (int i = 0; i < 6; i++)
    {
        tags t = tagreader(tagReader->fptr_fname);
        switch (t)
        {
        case m_title:
            /* Reading the title of mpeg file */
            tagReader->pos[m_title]=ftell(tagReader->fptr_fname);
            tagsizereader(tagReader->fptr_fname, &tagReader->Title_size);
            /* Skipping the flag */
            fseek(tagReader->fptr_fname, 3, SEEK_CUR);
            readtaginfo(tagReader->fptr_fname, &tagReader->Title, tagReader->Title_size);
            break;
        case m_album:
            /* Reading the album */
            tagReader->pos[m_album]=ftell(tagReader->fptr_fname);
            tagsizereader(tagReader->fptr_fname, &tagReader->Album_size);
            /* Skipping the flag */
            fseek(tagReader->fptr_fname, 3, SEEK_CUR);
            readtaginfo(tagReader->fptr_fname, &tagReader->album, tagReader->Album_size);
            break;
        case m_artist:
            /* Reading the artist */
            tagReader->pos[m_artist]=ftell(tagReader->fptr_fname);
            tagsizereader(tagReader->fptr_fname, &tagReader->Artist_size);
            /* Skipping the flag */
            fseek(tagReader->fptr_fname, 3, SEEK_CUR);
            readtaginfo(tagReader->fptr_fname, &tagReader->Artist, tagReader->Artist_size);
            break;
        case m_year:
            /* Reading the year of the album */
            tagReader->pos[m_year]=ftell(tagReader->fptr_fname);
            tagsizereader(tagReader->fptr_fname, &tagReader->Year_size);
            /* Skipping the flag */
            fseek(tagReader->fptr_fname, 3, SEEK_CUR);
            readtaginfo(tagReader->fptr_fname, &tagReader->Year, tagReader->Year_size);
            break;
        case m_genre:
            /* Reading the genere of the album */
            tagReader->pos[m_genre]=ftell(tagReader->fptr_fname);
            tagsizereader(tagReader->fptr_fname, &tagReader->Genre_size);
            /* Skipping the flag */
            fseek(tagReader->fptr_fname, 3, SEEK_CUR);
            readtaginfo(tagReader->fptr_fname, &tagReader->Genre, tagReader->Genre_size);
            break;
        case m_comment:
            /* Reading the comments of the mpeg */
            tagReader->pos[m_comment]=ftell(tagReader->fptr_fname);
            tagsizereader(tagReader->fptr_fname, &tagReader->Com_size);
            /* Skipping the flag */
            fseek(tagReader->fptr_fname, 3, SEEK_CUR);
            readtaginfo(tagReader->fptr_fname, &tagReader->Comment, tagReader->Com_size);
            break;
        default:
            break;
        }
    }
    return success;
}

/* Funmction to print the mpeg information */
Status printInfo(Tag_Reader *tagReader)
{

    printf("\n........................Selected View Details...................\n");
    printf("\n................................................................\n");
    printf("\n                MP3 TAG READER AND EDITOR FOR ID3v2          \n");
    printf("\n................................................................\n\n");
    printf("TITLE          : %s \n", tagReader->Title);
    printf("ARTIST         : %s \n", tagReader->Artist);
    printf("ALBUM          : %s \n", tagReader->album);
    printf("YEAR           : %s \n", tagReader->Year);
    printf("MUSIC          : %s \n", tagReader->Genre);
    printf("COMMENT        : %s \n", tagReader->Comment);
    printf("\n................................................................\n\n");
    return success;
}

/* Function to return the tags of the mpeg file */
tags tagreader(FILE *fname)
{
    char tag[4 + 1];
    fread(tag, 4, 1, fname);
    tag[4] = '\0';
    if (strcmp(tag, "TIT2") == 0)
        return m_title;
    else if (strcmp(tag, "TALB") == 0)
        return m_album;
    else if (strcmp(tag, "TPE1") == 0)
        return m_artist;
    else if (strcmp(tag, "TYER") == 0)
        return m_year;
    else if (strcmp(tag, "TCON") == 0)
        return m_genre;
    else if (strcmp(tag, "COMM") == 0)
        return m_comment;
}

/* Function to get the size of tags */
void tagsizereader(FILE *fname, int *size)
{

    unsigned int temp;
    fread(&temp, 4, 1, fname);
    endians_conversion(&temp);
    *size = temp - 1;
}

/* Function to read tag information */
void readtaginfo(FILE *fname, char **name, int size)
{
    char *buffer = malloc(size + 1);
    fread(buffer, size, 1, fname);
    buffer[size] = '\0';
    *name = malloc(size + 1);
    strcpy(*name, buffer);
    free(buffer);
}