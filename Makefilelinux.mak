CXX = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -fpic -g
LIBSDIR = -L. -L/usr/lib
INCLUDEDIR = -I. -I/usr/include

LIBCORENAME = grid
LIBTARGET :=lib$(LIBCORENAME:=.so)
LIBSOURCE = grid
LIBSOURCECFILE = $(LIBSOURCE:=.c)
LIBSOURCEOFILE = $(LIBSOURCE:=.o)

EXESOURCE = main
TARGET = $(EXESOURCE:=.exe)
EXESOURCECFILE = $(EXESOURCE:=.c)
EXESOURCEOFILE = $(EXESOURCE:=.o)

all: $(TARGET)

run: $(TARGET)
	@./export.sh 
	./$(TARGET)

$(TARGET): $(EXESOURCEOFILE) $(LIBTARGET) 
	$(CXX) $(EXESOURCEOFILE) -l$(LIBCORENAME) $(LIBSDIR) -o $(TARGET) -lm

$(LIBTARGET): $(LIBSOURCEOFILE) 
	$(CXX) $(CFLAGS) -shared $(LIBSOURCEOFILE) -o $(LIBTARGET)

.c.o:
	$(CXX) $(CFLAGS) $(INCLUDEDIR) -c -o $@ $<

clean: 
	@rm -rf *.o *.so *.exe *.dll *.zip
	@echo CLEAN