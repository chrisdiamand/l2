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

#ifndef TOKENS_H
#define TOKENS_H

#include <list>
#include <stdio.h>
#include "InputSource.h"

enum TokenType
{
    TOK_NULL,

    TOK_OPEN_BRACKET,   /* ( */
    TOK_CLOSED_BRACKET, /* ) */
    //TOK_STARTBLOCK,     /* { */
    //TOK_ENDBLOCK,       /* } */

    TOK_IDENTIFIER,     /* [a-z|A-Z|_]+ */
    TOK_CLASSNAME,      /* Identifier recognised as a class name */
    TOK_INT,            /* 234 */
    TOK_DECIMAL,        /* 3.141 */
    TOK_STRING,         /* "words" */

    TOK_ASSIGN,         /* = */

    TOK_ISEQUAL,        /* == */
    TOK_LT,             /* < */
    TOK_LTE,            /* <= */
    TOK_GT,             /* > */
    TOK_GTE,            /* >= */

    TOK_ADD,            /* + */
    TOK_SUB,            /* - */
    TOK_MUL,            /* * */
    TOK_DIV,            /* / */

    TOK_CLASS,          /* 'class' */
    TOK_END,            /* 'end' */
    TOK_RETURN,         /* 'return' */
    TOK_DOT,            /* . */
    TOK_COMMA,          /* , */
    TOK_SEMICOLON,      /* ; */
    TOK_EOF             /* End of file */
};

class Token {
public:
    char                *value;
    TokenType           type;
    unsigned int        line;

    void print();
    char *name();
};

class TokenList {
private:
    std::list<Token>    tokens;
    InputSource         *in;

public:
    TokenList(InputSource *);

    void print();
    Token next();
    bool expect(TokenType);

    Token               *cur;
};

char *TokenName(TokenType type);

#endif

