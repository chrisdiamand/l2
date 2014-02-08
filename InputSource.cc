#include <stdio.h>
#include "InputSource.h"

void die(const char *, ...);

InputSource::InputSource() {
    line = 1;
}

char InputSource::next() {
    char c = actual_next();
    if (c == '\n')
        line++;
    return c;
}

void InputSource::put(char c) {
    if (c == '\n')
        line--;
    actual_put(c);
}

StringInputSource::StringInputSource(char *s) {
    str = s;
    pos = 0;
}

char StringInputSource::actual_next() {
    return str[pos++];
}

void StringInputSource::actual_put(char c) {
    pos--;
}

FileInputSource::FileInputSource(char *filename) {
    fname = filename;
    fp = fopen(fname, "r");

    if (fp == NULL)
        die("Could not open file \'%s\'\n", fname);
}

FileInputSource::FileInputSource(FILE *fptr) {
    fp = fptr;
    fname = "<unknown>";

    if (fp == NULL)
        die("Null file pointer passed to struct scan_file\n");
}

char FileInputSource::actual_next() {
    int i = getc(fp);
    if (i == EOF)
        return 0;
    return (char) i;
}

void FileInputSource::actual_put(char c) {
    ungetc(c, fp);
}
