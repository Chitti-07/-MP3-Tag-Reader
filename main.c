#include <stdio.h>
#include "edit.h"

int main(int argc, char *argv[])
{
    Tag_Reader tagReader;
    /* Checking / validating the arguments */
    if (argc > 1)
    {
        /* Operations for edit option */
        if (checkOperation(argv) == Edit)
        {
            if (argc >= 4)
            {
                if (readandvalidate(&tagReader, argv[4]) == success)
                {
                    editInfo(&tagReader,argv[2],argv[3]);
                }
            }
            else
            {
                printf("\n...................................\n");
                printf("\n    please enter correct arguments \n");
                printf("\n......................................\n");
            }
        }
        /* Operation for view option */
        else if (checkOperation(argv) == View)
        {
            if (readandvalidate(&tagReader, argv[2]) == success)
            {
                viewInfo(&tagReader);
            }
        }
        /* Operation for help */
        else if (checkOperation(argv) == Help)
        {
            printHelp();
        }
    }
    /* Guide for users */
    else
    {
        printf("\n........................................................\n\n");
        printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
        printf("USAGE : \n");
        printf("To view use : ./a.out -v mp3filename\n");
        printf("To edit use : ./a.out -e  -t/-a/-A/-m/-y/-c mp3filename\n");
        printf("To get help use : ./a.out --help\n");
        printf("\n........................................................\n\n");
    }
}