/*
 * Copyright (c) 2014 Chris Diamand
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
        die("Could not open file \'%s\'", fname);
}

FileInputSource::FileInputSource(FILE *fptr) {
    fp = fptr;
    fname = "<unknown>";

    if (fp == NULL)
        die("Null file pointer passed to struct scan_file");
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
