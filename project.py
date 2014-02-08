src = []
src.append("main.cc")
src.append("InputSource.cc")
src.append("ParseTree.cc")
src.append("TokenList.cc")

CFLAGS = "-Wall -pedantic -std=c++11 "
CFLAGS += "-Wno-deprecated-writable-strings"

lang = CProg("l2", src, flags = CFLAGS)

makefile = Makefile([lang])
