#ifndef TAG_EDITOR_H
#define TAG_EDITOR_H

#include <stdio.h>

void view_tags(FILE *);
int file_val(FILE *);
int check_tag(char *, char *);
void endian_convert(int *);
void edit_tags(FILE *, char *, char *, char *);
void view_help();

#endif