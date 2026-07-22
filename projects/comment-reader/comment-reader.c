/* Created in 2025. 
* 
* A program that reads comments */

#include <stdio.h>
#include <stdlib.h>

FILE *in_fp, *fopen();
void comment_extract(FILE *in_fp);

int main() {
    if ( (in_fp = fopen("comments.txt", "r") ) == NULL )
        printf("ERROR - cannot open comments.txt file\n");
    comment_extract(in_fp);
    fclose(in_fp);
    return 0;
}

void comment_extract(FILE *in_fp) {
    int ch;
    int previous = 0;
    int comments = 0;
    
    while ( (ch = fgetc(in_fp) ) != EOF ) {
        if (!comments) {
            if ( previous == '/' && ch == '*' ) {
                comments = 1;
                printf("\n/"); }
        }
        else {
            if ( previous == '*' && ch == '/') {
                printf("*/\n");
                comments = 0; }
            else { putchar(previous); }
        }
        previous = ch;
    }
}
