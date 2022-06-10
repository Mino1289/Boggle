CXX = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
LIBSDIR = -L.
INCLUDEDIR = -I. 

DEBUG ?= 0 # can be changed on the command line
ifeq ($(DEBUG), 1)
	CFLAGS += -ggdb
	CFLAGS += -DDEBUG
endif

LIBCORENAME = boggle

ifeq ($(OS), Windows_NT)
	EXPORT = export.bat
	LIBTARGET :=$(LIBCORENAME:=.dll)
	CLEANCMD = @del /q *.o *.so *.exe *.dll
else
	CFLAGS += -fpic
	EXPORT = sh export.sh
	LIBTARGET := lib$(LIBCORENAME:=.so)
	CLEANCMD = rm -rf *.o *.so *.exe *.dll
	LIBSDIR += -L/usr/lib
	INCLUDEDIR += -I/usr/include
endif

LIBSOURCE = gui grid game save
LIBSOURCECFILE = $(LIBSOURCE:=.c)
LIBSOURCEOFILE = $(LIBSOURCE:=.o)

EXESOURCE = main
TARGET = $(EXESOURCE:=.exe)
EXESOURCECFILE = $(EXESOURCE:=.c)
EXESOURCEOFILE = $(EXESOURCE:=.o)

all: $(TARGET)

run: $(TARGET)
	$(EXPORT) $(TARGET)

$(TARGET): $(EXESOURCEOFILE) $(LIBTARGET) 
	$(CXX) $(EXESOURCEOFILE) -l$(LIBCORENAME) $(LIBSDIR) -o $(TARGET) -lm

$(LIBTARGET): $(LIBSOURCEOFILE) 
	$(CXX) $(CFLAGS) -shared $(LIBSOURCEOFILE) -o $(LIBTARGET)

.c.o:
	$(CXX) $(CFLAGS) $(INCLUDEDIR) -c -o $@ $<

clean: 
	$(CLEANCMD)
	@echo CLEAN