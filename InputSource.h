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

#ifndef INPUT_SOURCE_H
#define INPUT_SOURCE_H

#include <stdio.h>

class InputSource {
public:
    InputSource();
    unsigned long   line;
    // Wrappers for next()/put() that counts line numbers
    char next();
    void put(char);
    // Actually retrieve the next character
    virtual char actual_next() = 0;
    virtual void actual_put(char) = 0;
};

class FileInputSource : public InputSource {
private:
    FILE            *fp;
    char            *fname;
public:
    FileInputSource(char *);
    FileInputSource(FILE *);
    char actual_next();
    void actual_put(char);
};

class StringInputSource : public InputSource {
private:
    unsigned long   pos;
    char            *str;
public:
    StringInputSource(char *);
    char actual_next();
    void actual_put(char);
};

#endif
