#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "tag_editor.h"

int file_val(FILE * mpptr)
{
    // Validating if its opened properly ---------------------------- ( Validation 1 )
    if (mpptr == NULL)
    {
        printf("Error: Can not access the file.\n");
        return 0;
    }
    
    // Check if MP3 or not ------------------------------------------ ( Validation 2 )
    char str[4];
    fread(str, sizeof(char), 3, mpptr);
    str[4] = '\0';
    if (strcmp(str, "ID3") != 0)
    {
        printf("Error: Input file is not a MP3, try again.\n");
        return 0;
    }
    return 1;
}

void view_tags(FILE * mpptr)
{
    // Skip the Header
    fseek(mpptr, 10, SEEK_SET);

    char tag[5];
    char tag_name[20];
    int size;           // tag_text size
    int tag_count = 0;  // to check if 6 tags are printed
    int flag = 0;       // to chek if read tag is on of the 6 we want

    printf("———————————————————————————————————————————————————————————————————————\n");
    printf("MP3 Tag Reader & Editor for ID3v2\n");
    printf("———————————————————————————————————————————————————————————————————————\n");
    for ( int i = 0 ; i<=81 ; i++ )
    {
        // Reading the tag
        fread(tag, sizeof(char), 4, mpptr);
        tag[4] = '\0';

        // Chek if read tag is on of the 6 we want
        flag = check_tag(tag, tag_name);

        // Reading the tag_text size & Big Endien to Little Endien conversion
        fread(&size, sizeof(int), 1, mpptr);
        endian_convert(&size);
        // Skipping 2 flags & a null charact er
        fseek(mpptr, 3, SEEK_CUR);
        
        // Reading the tag_text
        char tag_text[size-1]; // [size-1] to exclude "Encoding byte" (text encoding identifier) at the start of the text.
        fread(tag_text, sizeof(char), size-1, mpptr);
        tag_text[size-1] = '\0';
        
        // Printing the Tag Name and Tag Text
        if (flag)
        {
            printf("%-20s : ", tag_name);
            // Printing Tag Text character by character (to avoid encountering with null characters)
            for (int j=0; j<size-1; j++)
            {
                printf("%c",tag_text[j]);
            }
            printf("\n");
            tag_count++;
        }
        
        // Check if 6 tags are printed
        if (tag_count == 6)
            break;
    }
    printf("———————————————————————————————————————————————————————————————————————\n");
    fclose(mpptr);
    return;
}

void edit_tags(FILE * mpptr, char * in_tag, char * new_tag_txt, char * file_name)
{
    // Reading the tag
    fseek(mpptr, 10, SEEK_SET);

    char tag[5];
    char tag_name[20];
    int size;           // tag_text size
    int tag_count = 0;  // to check if 6 tags are printed
    int flag = 0;       // to chek if read tag is on of the 6 we want
    
    for ( int i = 0 ; i <= 81 ; i++ )
    {
        // Reading the tag
        fread(tag, sizeof(char), 4, mpptr);
        tag[4] = '\0';

        // Chek if read tag is on of the 6 we want
        flag = check_tag(tag, tag_name);
        
        // Reading the tag_text size & Big Endien to Little Endien conversion
        fread(&size, sizeof(int), 1, mpptr);
        endian_convert(&size);
        
        // check if current tag matches with input tag
        if (strcmp(in_tag,tag) ==  0)
            break;

        // Skipping 2 flags & a null character
        fseek(mpptr, 3, SEEK_CUR);
        
        // Skipping the tag_text
        fseek(mpptr, size-1, SEEK_CUR);
        
        // Check if 6 tags are printed
        if (flag)
            tag_count++;
        if (tag_count == 6)
        {
            printf("Error: Not able to find the tag in the MP3 file.\n");
            break;
        }
    }
    int tag_start = (ftell(mpptr)) - 4; // (-4) Becaue we read Tag and Size from origional
    fseek(mpptr, 0, SEEK_SET);
    
    FILE * n_mpptr = fopen("edited_sample.mp3", "wb");
    if (n_mpptr == NULL)
    {
        printf("Error: Can not access the [Edited_Sample.mp3] file.\n");
        return;
    }

    char ch;

    // copy data untill the tag is found ----------------------------------------------------------------- ( copy step 1 )
    int count = 0;
    while ((ch = fgetc(mpptr)) != EOF)
    {
        fputc(ch, n_mpptr);
        count++;
        if (count == tag_start)
            break;
    }
    // printf("%d ----\n",tag_start); // ---------------------------------- (TEST 🟩)
    // printf("%lu ---- mpptr file ptr\n", ftell(mpptr)); // -------------- (TEST 🟩)
    // printf("%lu ---- new mpptr file ptr\n\n", ftell(n_mpptr)); // ------ (TEST 🟩)
    

    // write size: 1 byte ------------------------------------------------------------------------------- ( copy step 2 )
    int new_tag_txt_len = strlen(new_tag_txt) + 1; // ------------(+1)
    endian_convert(&new_tag_txt_len);
    fwrite(&new_tag_txt_len, sizeof(int), 1, n_mpptr);
    // move pointer size: 1 byte
    fseek(mpptr, (sizeof(int)*1), SEEK_CUR);
    

    // copy 2 flag and Null character: 3 byte ----------------------------------------------------------- ( copy step 3 )
    count = 0;
    while ((ch = fgetc(mpptr)) != EOF)
    {
        fputc(ch, n_mpptr);
        count++;
        if (count == 3)
            break;
    }
    

    // write tag text size: new_tag_txt_len ------------------------------------------------------------- ( copy step 4 )
    endian_convert(&new_tag_txt_len); // Re-converting new_tag_txt_len to little endien for follwing write function
    fwrite(new_tag_txt, sizeof(char), new_tag_txt_len-1, n_mpptr); // ------------(-1)
    // move pointer size - old text size
    fseek(mpptr, (sizeof(char)*size-1), SEEK_CUR);

    
    // copy rest of the data ----------------------------------------------------------------------------- ( copy step 5 )
    while (fread(&ch, 1, 1, mpptr) > 0)  // not using EOF, because there could be Null characters in between
    {
        fputc(ch, n_mpptr);
    }

    // closing both files
    fclose(mpptr);
    fclose(n_mpptr);
    
    // Removing origional file & Renaming the new file
    if (remove(file_name) == 0)
    {
        if (rename("edited_sample.mp3", file_name) == 0)
        {
            // Printing message to indicate edited part
            printf("———————————————————————————————————————————————————————————————————————\n");
            printf("MP3 Tag Reader & Editor for ID3v2\n");
            printf("———————————————————————————————————————————————————————————————————————\n");
            printf("\nSuccess: %s changed to %s.\n\n", tag_name, new_tag_txt);
            printf("———————————————————————————————————————————————————————————————————————\n");
        }
    }
    return;
}

int check_tag(char * tag, char * tag_txt)
{
    if (strcmp(tag, "TIT2") == 0)
    {
        strcpy(tag_txt, "Title");
        return 1;
    }
    else if (strcmp(tag, "TPE1") == 0)
    {
        strcpy(tag_txt, "Artist");
        return 1;
    }
    else if (strcmp(tag, "TALB") == 0)
    {
        strcpy(tag_txt, "Album");
        return 1;
    }
    else if (strcmp(tag, "TYER") == 0)
    {
        strcpy(tag_txt, "Year");
        return 1;
    }
    else if (strcmp(tag, "TCON") == 0)
    {
        strcpy(tag_txt, "Content Type");
        return 1;
    }
    else if (strcmp(tag, "COMM") == 0)
    {
        strcpy(tag_txt, "Composer");
        return 1;
    }
    else
        return 0; // Return 0 if input tag does not match with any of the 6 tags
}

void endian_convert(int *num)
{
    int size = 4;
    char *ptr = (char *)num;
    for (int i = 0; i < size / 2; i++)
    {
        char temp = ptr[i];
        ptr[i] = ptr[size - 1 - i];
        ptr[size - 1 - i] = temp;
    }
}

void view_help()
{
    printf("———————————————————————————————————————————————————————————————————————\n");
    printf("MP3 Tag Reader & Editor for ID3v2 — Help Menu\n");
    printf("———————————————————————————————————————————————————————————————————————\n\n");

    printf("Usage:\n");
    printf("  ./a.out [OPTION] [ARGS...]\n\n");

    printf("Options:\n");
    printf("  -v <filename>                          View ID3v2 tags of the MP3 file\n");
    printf("  -e <specifier> <new_value> <filename>  Edit a specific tag in the MP3 file\n");
    printf("  -h                                     Display this help menu\n\n");

    printf("Tag Specifiers (for use with -e option):\n");
    printf("  -T     Title\n");
    printf("  -A     Artist\n");
    printf("  -a     Album\n");
    printf("  -Y     Year   (Valid range: 1600 - 2100)\n");
    printf("  -C     Content Type (Genre)\n");
    printf("  -c     Comments\n\n");

    printf("Examples:\n");
    printf("  ./a.out -v sample.mp3\n");
    printf("      → Displays tags like Title, Artist, Album, Year, Genre, Comments\n\n");

    printf("  ./a.out -e -T \"New Title\" sample.mp3\n");
    printf("      → Updates the title tag in 'sample.mp3' to 'New Title'\n\n");

    printf("  ./a.out -h\n");
    printf("      → Shows this help menu\n");

    printf("\n———————————————————————————————————————————————————————————————————————\n");
}