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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "ParseTree.h"
#include "TokenList.h"

void die(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    exit(1);
}

int main(int argc, char **argv)
{
    InputSource *in = NULL;
    TokenList *toks = NULL;
    ParseTree *pt = NULL;

    if (argc > 1)
        in = new FileInputSource(argv[1]);
    else
        in = new FileInputSource(stdin);

    toks = new TokenList(in);
    pt = new PT_ClassBody(toks, TOK_EOF);
    pt->print();

    return 0;
}

