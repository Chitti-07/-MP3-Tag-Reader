#include "edit.h"
#include <string.h>

/* Function to edit the mpeg file */
Status editInfo(Tag_Reader *tagReader, char *ch, char *name)
{
    if (openFiles(tagReader) == success)
    {
        FILE *fptr_dest = fopen("sample.mp3", "wb+");
        if (fptr_dest == NULL)
        {
            perror("fopen");
            fprintf(stderr, "ERROR: Unable to open file %s\n", "sample.mp3");

            return failure;
        }
        if (checkheaderandversion(tagReader->fptr_fname) == success)
        {
            if (readInfo(tagReader) == success)
            {
                renametag(tagReader, ch, name, fptr_dest);
                copytoriginal(tagReader->fptr_fname, fptr_dest);
            }
        }
    }
    return success;
}

/* Function to edit the tags */
Status renametag(Tag_Reader *tagReader, char *ch, char *name, FILE *fptr_dest)
{
    if (copyheader(tagReader->fptr_fname, fptr_dest) == success)
    {
        switch (checkedit(ch))
        {
        case t:
            /* Editing the mpeg tag */
            copyContent(tagReader->fptr_fname, tagReader->pos[m_title], fptr_dest);
            copynewcon(tagReader->fptr_fname, name, fptr_dest);
            copyremaining(tagReader->fptr_fname, tagReader->Title_size, fptr_dest);
            break;
        case a:
            /* Editing the artist name */
            copyContent(tagReader->fptr_fname, tagReader->pos[m_album], fptr_dest);
            copynewcon(tagReader->fptr_fname, name, fptr_dest);
            copyremaining(tagReader->fptr_fname, tagReader->Album_size, fptr_dest);
            break;
        case A:
            /* Editing the Album name */
            copyContent(tagReader->fptr_fname, tagReader->pos[m_artist], fptr_dest);
            copynewcon(tagReader->fptr_fname, name, fptr_dest);
            copyremaining(tagReader->fptr_fname, tagReader->Artist_size, fptr_dest);
            break;
        case y:
            /* Editing the year of album */
            copyContent(tagReader->fptr_fname, tagReader->pos[m_year], fptr_dest);
            copynewcon(tagReader->fptr_fname, name, fptr_dest);
            copyremaining(tagReader->fptr_fname, tagReader->Year_size, fptr_dest);
            break;
        case m:
            /* Editing the gener of the music */
            copyContent(tagReader->fptr_fname, tagReader->pos[m_genre], fptr_dest);
            copynewcon(tagReader->fptr_fname, name, fptr_dest);
            copyremaining(tagReader->fptr_fname, tagReader->Genre_size, fptr_dest);
            break;
        case c:
            /* Editing the comment of the album */
            copyContent(tagReader->fptr_fname, tagReader->pos[m_comment], fptr_dest);
            copynewcon(tagReader->fptr_fname, name, fptr_dest);
            copyremaining(tagReader->fptr_fname, tagReader->Com_size, fptr_dest);
            break;
        default:
            break;
        }
    }
    return success;
}

/* Function to copy contents */
Status copyContent(FILE *fname, int pos, FILE *fptr_dest)
{
    char ch;
    for (int i = 10; i < pos; i++)
    {
        fread(&ch, 1, 1, fname);
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return success;
}

/* Function to copy the new content to the mpeg file */
Status copynewcon(FILE *fname, char *name, FILE *fptr_dest)
{
    int len = strlen(name) + 1;
    char bytes[4];
    for (int i = 0; i < 4; i++)
    {
        bytes[3 - i] = ((len >> 8 * i) & 0xFF);
    }
    fwrite(bytes, 4, 1, fptr_dest);
    fseek(fname, 4, SEEK_CUR);
    char buffer[3];
    fread(buffer, 3, 1, fname);
    fwrite(buffer, 3, 1, fptr_dest);
    fwrite(name, len - 1, 1, fptr_dest);
    return success;
}

/* Function to copy remaining data to mpeg file */
Status copyremaining(FILE *fname, int size, FILE *fptr_dest)
{
    fseek(fname, size, SEEK_CUR);
    char ch;
    while ((ch = getc(fname)) != EOF)
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return success;
}

/* Function to copy header file */
Status copyheader(FILE *fname, FILE *fptr_dest)
{
    rewind(fname);
    char buffer[10];
    fread(buffer, 10, 1, fname);
    fwrite(buffer, 10, 1, fptr_dest);
    return success;
}

/* Function to copy content to original file */
Status copytoriginal(FILE *fname, FILE *fptr_dest)
{
    rewind(fname);
    rewind(fptr_dest);
    char ch;
    while ((ch = getc(fptr_dest)) != EOF)
    {
        fwrite(&ch, 1, 1, fname);
    }
    fclose(fptr_dest);
    remove("sample.mp3");
    return success;
}

/* Function to check tags */
edittags checkedit(char *ch)
{
    if (strcmp(ch, "-t") == 0)
        return t;
    else if (strcmp(ch, "-a") == 0)
        return a;
    else if (strcmp(ch, "-A") == 0)
        return A;
    else if (strcmp(ch, "-y") == 0)
        return y;
    else if (strcmp(ch, "-m") == 0)
        return m;
    else if (strcmp(ch, "-c") == 0)
        return c;
}

