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

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "TokenList.h"
#include "InputSource.h"

void die(const char *, ...);

/* This could be done with a table but it's not really worth bothering */
char *scanner_token_name(TokenType t)
{
    /* No default case so the compiler will warn when a type is missing */
    switch (t)
    {
        case TOK_NULL:              return "null";
        case TOK_OPEN_BRACKET:      return "\'(\'";
        case TOK_CLOSED_BRACKET:    return "\')\'";
        //case TOK_STARTBLOCK:        return "\'{\'";
        //case TOK_ENDBLOCK:          return "\'}\'";
        case TOK_IDENTIFIER:        return "identifier";
        case TOK_CLASSNAME:         return "classname";
        case TOK_INT:               return "integer";
        case TOK_DECIMAL:           return "decimal";
        case TOK_STRING:            return "string";
        case TOK_ASSIGN:            return "assignment";

        case TOK_ISEQUAL:           return "\'==\'";
        case TOK_LT:                return "\'<\'";
        case TOK_LTE:               return "\'<=\'";
        case TOK_GT:                return "\'>\'";
        case TOK_GTE:               return "\'>=\'";

        case TOK_ADD:               return "\'+\'";
        case TOK_SUB:               return "\'-\'";
        case TOK_MUL:               return "\'*\'";
        case TOK_DIV:               return "\'/\'";

        case TOK_CLASS:             return "\'class\'";
        case TOK_END:               return "\'end\'";
        case TOK_RETURN:            return "\'return\'";
        case TOK_DOT:               return "\'.\'";
        case TOK_COMMA:             return "\',\'";
        case TOK_SEMICOLON:         return "\';\'";
        case TOK_EOF:               return "EOF";
    }
    return "<?>";
}



/* FIXME: Add escape sequences \n etc */
static char *read_quoted_string(InputSource *in)
{
    unsigned int len = 32, pos = 0;
    char *s = (char *) malloc(len), c;
    if (s == NULL)
        die("read_quoted_string(): malloc() failed!", 1);
    do
    {
        c = in->next();
        if (pos >= len - 1)
        {
            char *NewStr;
            len *= 2;
            NewStr = (char *) realloc(s, len);
            if (NewStr == NULL)
                die("read_quoted_string: realloc() failed!", 1);
            s = NewStr;
        }
        s[pos++] = c;
    } while (c != '\"');
    /* Use pos - 1 to remove the final quote */
    s[pos - 1] = '\0';
    return s;
}

static Token scan_next(InputSource *in) {
    char c = 1;
    int i, num_invalid_chars = 0;
    Token T;
    T.line = in->line;
    T.value = NULL;

    while (c != 0)
    {
        c = in->next();
        if (isalpha(c)) /* An identifier */
        {
            char name[512]; /* Arbitrary limit to name length */
            name[0] = c;
            i = 1;
            c = in->next();
            while (isalnum(c) || c == '_')
            {
                name[i++] = c;
                c = in->next();
            }
            name[i] = 0;
            in->put(c);
            if      ( strcmp(name, "class")     == 0 )  {   T.type = TOK_CLASS;     }
            else if ( strcmp(name, "end")       == 0 )  {   T.type = TOK_END;       }
            else if ( strcmp(name, "return")    == 0 )  {   T.type = TOK_RETURN;    }
            else
            {
                T.type = TOK_IDENTIFIER;
                T.value = strdup(name);
            }
            return T;
        }
        /* FIXME: Need to check that there is only 1 '.' and 'e' in a number.
         * Also maybe make 'e' an actual operator instead because that would be cool */
        if (isdigit(c))
        {
            char num[512];
            int ndots = 0;
            num[0] = c;
            i = 1;
            c = in->next();
            while (isdigit(c) || c == '.')
            {
                num[i++] = c;
                if (c == '.')
                    ndots++;
                c = in->next();
            }
            num[i] = 0;
            in->put(c);
            if (ndots > 1)
                die("Invalid number: Too many decimal points", 1);
            T.type = (ndots == 0) ? TOK_INT : TOK_DECIMAL;
            /* It must be strdup'd to stop it being destroyed as soon as this loop returns */
            T.value = strdup(num);
            return T;
        }
        if (c == '=')
        {
            T.type = TOK_ASSIGN;
            /* Check it is =, not == */
            c = in->next();
            if (c == '=') /* It must be comparison: == */
                T.type = TOK_ISEQUAL;
            else
                in->put(c);
            return T;
        }
        if (c == '<')
        {
            T.type = TOK_LT;
            c = in->next();
            if (c == '=')
                T.type = TOK_LTE;
            else
                in->put(c);
            return T;
        }
        if (c == '>')
        {
            T.type = TOK_GT;
            c = in->next();
            if (c == '=')
                T.type = TOK_GTE;
            else
                in->put(c);
            return T;
        }
        if (c == '\"')
        {
            char *s = read_quoted_string(in);
            T.value = s;
            T.type = TOK_STRING;
            return T;
        }
        if (c == '#')
        {
            while (c != '\n')
                c = in->next();
        }
        if (isspace(c)) /* Ignore whitespace */
            continue;
        switch (c)
        {
            case   0:   T.type = TOK_EOF;           return T;
            case '.':   T.type = TOK_DOT;           return T;
            case '+':   T.type = TOK_ADD;           return T;
            case '-':   T.type = TOK_SUB;           return T;
            case '*':   T.type = TOK_MUL;           return T;
            case '/':   T.type = TOK_DIV;           return T;
            case '(':   T.type = TOK_OPEN_BRACKET;  return T;
            case ')':   T.type = TOK_CLOSED_BRACKET;return T;
            //case '{':   T.type = TOK_STARTBLOCK;    return T;
            //case '}':   T.type = TOK_ENDBLOCK;      return T;
            case ',':   T.type = TOK_COMMA;         return T;
            case ';':   T.type = TOK_SEMICOLON;     return T;
            default:
                if (num_invalid_chars++ < 1)
                    fprintf(stderr,
                            "Syntax error, line %lu: Invalid character \'%c\' (%d)\n",
                            in->line, c, c);
                break;
        }
    }
    printf("EOF\n");
    T.type = TOK_EOF;
    return T;
}

Token TokenList::next() {
    return scan_next(in);
}

void TokenList::print() {
    printf("Hi!\n");
}

/* Scan a file and return an array of all the tokens */
TokenList::TokenList(InputSource *input)
{
    in = input;
    next();
}

