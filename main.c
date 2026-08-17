/*
———————————————————————————————————————————————————————————————————————
Project: MP3 ID3v2 Tag Viewer & Editor
Author : Falah Nizar
Date   : 12 May 2025
Language: C

Description:
  This C program allows users to view and edit ID3v2 tags (metadata) in MP3 files.
  It supports operations like displaying tags (Title, Artist, Album, Year, Genre, Comments)
  and editing them by modifying the corresponding tag values inside the file.

File Info:
  - Reads ID3v2 tags from an MP3 file starting after the 10-byte header
  - Edits tags by copying the original file to a new file with updated metadata
  - Automatically validates the MP3 file and the year value when editing

Sample User Inputs:
  ./a.out -v sample.mp3
    → View tags of the file 'sample.mp3'

  ./a.out -e -T "New Title" sample.mp3
    → Change the title tag of 'sample.mp3' to 'New Title'

  ./a.out -h
    → Display help menu with usage information
———————————————————————————————————————————————————————————————————————
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "tag_editor.h"

int main( int argc, char *argv[])
{
    // Perform operarion on file based on user input
    if (strcmp(argv[1], "-v") == 0)
    {
        // Opening the file
        FILE * mpptr = fopen(argv[2], "rb");
        
        if (file_val(mpptr))
            view_tags(mpptr);
        else
            return 0;
    }
    else if (strcmp(argv[1], "-e") == 0)
    {
        // Opening the file
        FILE * mpptr = fopen(argv[4], "rb");

        // Checking the edit specifier entered by user
        char in_tag[5];
        if (strcmp(argv[2],"-T") == 0)
            strcpy(in_tag,"TIT2");
        else if (strcmp(argv[2],"-A") == 0)
            strcpy(in_tag,"TPE1");
        else if (strcmp(argv[2],"-a") == 0)
            strcpy(in_tag,"TALB");
        else if (strcmp(argv[2],"-C") == 0)
            strcpy(in_tag,"TCON");
        else if (strcmp(argv[2],"-c") == 0)
            strcpy(in_tag,"COMM");
        else if (strcmp(argv[2],"-Y") == 0)
        {
            strcpy(in_tag,"TYER");
            // Year number validation
            int year = atoi(argv[3]);
            if ((year<1600) || (year>2100))
            {
                printf("Error: Invalid year.\n");
                return 0;
            }
        }
        else
        {
            printf("Error: Invalid edit specifier.\n");
            return 0;
        }
        in_tag[4] = '\0';

        // Passing inputs to the edit function
        if (file_val(mpptr))
            edit_tags(mpptr, in_tag, argv[3], argv[4]);
        else
            return 0;
    }
    else if (strcmp(argv[1], "-h") == 0)
    {
        view_help();
    }
    else
        printf("Error: Invalid input, try again.\n");
    return 0;
}