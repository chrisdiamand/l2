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
