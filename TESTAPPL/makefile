

OBJDIR  = ../obj/
BINDIR  = ../bin/
SRCDIR  = ../src/
CC      = cxx
CCFLAGS = -g -define_templates

gis: $(OBJDIR)gis.o $(OBJDIR)main.o 
	$(CC) $(CCFLAGS) -o $(BINDIR)gis $(OBJDIR)gis.o $(OBJDIR)main.o

$(OBJDIR)gis.o: $(SRCDIR)gis.cpp $(SRCDIR)gis.h
	$(CC) $(CCFLAGS) -c -o $(OBJDIR)gis.o $(SRCDIR)gis.cpp

$(OBJDIR)main.o: $(SRCDIR)main.cpp $(SRCDIR)gis.h
	$(CC) $(CCFLAGS) -c -o $(OBJDIR)main.o $(SRCDIR)main.cpp

clean:
	rm $(OBJDIR)*.o
