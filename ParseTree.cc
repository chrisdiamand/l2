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
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ParseTree.h"
#include "TokenList.h"

/*
static void syntaxerror(TokenList *T, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Syntax error, line %u: ", T->ct.line);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    fflush(stderr);
}
*/

#if 0
static void class_typename_list(struct class_t *cl)
{
    char *typenames[16]; /* Who needs more than that? */
    int i;
    for (i = 0; i < sizeof(typenames) / sizeof(char *); i++)
    {
        typenames[i] = T->ct.value;
        T->expect(TOK_IDENTIFIER);

        if (T->ct.type != TOK_COMMA)
            break;
        T->expect(TOK_COMMA);
    }
    // i isn't incremented at the end of the for loop as it terminates
    // because of 'break;', not the loop condition. Increase it here:
    cl->nparams = i + 1;
    cl->paramnames = GC_malloc(sizeof(char *) * cl->nparams);
    memcpy(cl->paramnames, typenames, sizeof(char *) * cl->nparams);
}
#endif

/* Parse a class declaration and add it to namespace */
PT_ClassDecl::PT_ClassDecl(TokenList *T) {
    char *classname = NULL;
    T->expect(TOK_CLASS);
    classname = T->ct.value;
    T->expect(TOK_IDENTIFIER);

    /* Template typename list */
    if (T->ct.type == TOK_LT) {
        T->expect(TOK_LT);
        //class_typename_list(cl);
        T->expect(TOK_GT);
    }

    /* The actual class body */
    body = new PT_ClassBody(T, TOK_END);
    T->expect(TOK_END);
}

void PT_TypeSpec::print() {
    printf("ts");
}

PT_TypeSpec::PT_TypeSpec(TokenList *T) {

    char *tname = T->ct.value;
    T->expect(TOK_IDENTIFIER);

    /* Allow inner classes e.g. 'module.SomeType x;' */
    while (T->ct.type == TOK_DOT) {
        char *innername;
        T->expect(TOK_DOT);
        innername = T->ct.value;
        T->expect(TOK_IDENTIFIER);
        tname = innername;
    }

    if (T->ct.type == TOK_LT) {
        T->expect(TOK_LT);

        while (1) {
            ParseTree *generic_type = new PT_TypeSpec(T);
            printf("%p\n", generic_type);
            if (T->ct.type != TOK_COMMA)
                break;
            T->expect(TOK_COMMA);
        }

        T->expect(TOK_GT);
    }
}

PT_MethodDecl::PT_MethodDecl(TokenList *T) {
    T->expect(TOK_OPEN_BRACKET);
    T->expect(TOK_CLOSED_BRACKET);
}

/* Parse a function/variable declaration. If it is syntactically
 * correct, add it to namespace, otherwise return false. */
static PT_Decl *function_or_variable_declaration(TokenList *T) {
    char *name;
    PT_TypeSpec *type = new PT_TypeSpec(T);
    PT_Decl *ret;
    printf("%p\n", type);

    name = T->ct.value;
    T->expect(TOK_IDENTIFIER);

    if (T->ct.type == TOK_OPEN_BRACKET) {
        ret = new PT_MethodDecl(T);
        ret->name = name;
        return ret;
    } // Else: Variable declarations
    return NULL;
}

/* Parse a list of declarations (variables, methods and classes)
 * and add them to namespace/class N */
PT_ClassBody::PT_ClassBody(TokenList *T, TokenType finish) {
    int errorcount = 0;

    declarations = new std::list<PT_Decl *>;

    while (T->ct.type != finish && T->ct.type != TOK_EOF)
    {
        PT_Decl *decl = NULL;
        switch (T->ct.type)
        {
            case TOK_CLASS:
                decl = new PT_ClassDecl(T);
                errorcount = 0;
                break;
            case TOK_IDENTIFIER:
                decl = function_or_variable_declaration(T);
                errorcount = 0;
                break;
            case TOK_SEMICOLON:
                T->expect(TOK_SEMICOLON);
                break;
            default:
                if (errorcount++ == 0)
                    T->expectedError("declaration");
                T->next();
                break;
        }
        if (decl != NULL)
            declarations->push_back(decl);
    }
}

void PT_ClassBody::print() {
    std::list<PT_Decl *>::iterator it;
    printf("--- ClassBody ---\n");
    for (auto it = declarations->begin(); it != declarations->end(); it++) {
        PT_Decl *d = *it;
        d->print();
    }
}

void PT_ClassDecl::print() {
    printf("ClassDecl\n");
}

void PT_MethodDecl::print() {
    printf("MethodDecl\n");
}
