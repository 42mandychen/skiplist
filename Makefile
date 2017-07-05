# The project ('all') is just to build the executable file 'skiplist'
# This line simply says that 'all' depends on 'skiplist'.
all : skiplist

# 'skiplist' depends on one .o (object) file
# add any new .o files to the list
# build it using g++
skiplist : skiplist.o
	g++ -o skiplist skiplist.cc

# To produce an executable file called skiplist
# so that ./skiplist command works
# build it using g++
skiplist.exe : skiplist.o
	g++ skiplist.o -o skiplist.exe

# skiplist.o is dependant on one .cc file and no .h files
# IMPORTANT: when you #include a new .h file, be sure to add it here.
# Otherwise, the 'make' command may not re-build your program even
# though you changed that .h file.
# -Wall: make the compiler display all warnings it knows about;
# hopefully it'll catch something for us
# -c: only compile, don't link (we do that above)
# -g: put in debug information, for those of you who are using
# debuggers (like gdb or ddd or xxgdb)
skiplist.o : skiplist.cc skiplist.h
	g++ -Wall -c skiplist.cc

# and same idea for other .o files ...



# 'clean' removes any files that were created via the Makefile,
# leaving you with only your original source code files
clean :
	rm *.o
	rm skiplist
