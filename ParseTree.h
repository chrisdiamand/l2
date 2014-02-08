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

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <list>

#include "TokenList.h"

class ParseTree {
public:
    virtual void print() = 0;
};

class PT_Decl : public ParseTree {
public:
    // Name of the thing being declared
    char                        *name;
};

class PT_TypeSpec : public ParseTree {
public:
    PT_TypeSpec(TokenList *);
    char                        *name;
    void print();
};

class PT_ClassBody : public ParseTree {
public:
    PT_ClassBody(TokenList *, TokenType);
    void print();
    std::list<PT_Decl *>        *declarations;
};

class PT_ClassDecl : public PT_Decl {
public:
    PT_ClassDecl(TokenList *);
    std::list<PT_TypeSpec *>    *typenames;
    PT_ClassBody                *body;
    void print();
};

class PT_VarDecl : public PT_Decl {
public:
    void print();
};

class PT_MethodDecl : public PT_Decl {
public:
    PT_MethodDecl(TokenList *);
    void print();
};

#endif /* PARSE_TREE_H */
