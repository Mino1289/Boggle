CXX = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -fpic -g
LIBSDIR = -L.
INCLUDEDIR = -I.

LIBCORENAME = grid
LIBTARGET :=$(LIBCORENAME:=.dll)
LIBSOURCE = grid generation
LIBSOURCECFILE = $(LIBSOURCE:=.c)
LIBSOURCEOFILE = $(LIBSOURCE:=.o)

EXESOURCE = main
TARGET = $(EXESOURCE:=.exe)
EXESOURCECFILE = $(EXESOURCE:=.c)
EXESOURCEOFILE = $(EXESOURCE:=.o)

all: $(TARGET)

run: $(TARGET)
	@export.bat 
	$(TARGET)

$(TARGET): $(EXESOURCEOFILE) $(LIBTARGET) 
	$(CXX) $(EXESOURCEOFILE) -l$(LIBCORENAME) $(LIBSDIR) -o $(TARGET) -lm

$(LIBTARGET): $(LIBSOURCEOFILE) 
	$(CXX) $(CFLAGS) -shared $(LIBSOURCEOFILE) -o $(LIBTARGET)

.c.o:
	$(CXX) $(CFLAGS) $(INCLUDEDIR) -c -o $@ $<

save: $(TARGET)
	$(TARGET) > $(EXESOURCE).txt

clean: 
	@clean.bat
	@echo CLEAN