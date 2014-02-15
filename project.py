src = []
src.append("main.cc")
src.append("InputSource.cc")
src.append("ParseTree.cc")
src.append("TokenList.cc")

CFLAGS = "-Wall -pedantic"
CFLAGS += " -std=c++0x"
CFLAGS += " -Wno-write-strings"

lang = CProg("l2", src, flags = CFLAGS)

makefile = Makefile([lang])
