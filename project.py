src = []
src.append("main.cc")
src.append("InputSource.cc")
src.append("ParseTree.cc")
src.append("TokenList.cc")

lang = CProg("l2", src, flags = "-Wall -pedantic -Wno-deprecated-writable-strings")

makefile = Makefile([lang])
