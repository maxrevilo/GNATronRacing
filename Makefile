CC = g++

EXECUTABLE = Compiled/juego

LIBS       = -lmingw32 -lopengl32 -lSDLmain -lSDL -lSDL_ttf 

CFLAGS     = -Wuninitialized -std=c++0x

COMPILE    = $(CC) $(CFLAGS) -c

GNA_FW     = GNAFramework/CPPS/*.cpp 

TINY_XML   = tinyxml/*.cpp 

GAME       = Managers/CPP/*.cpp GameActors/CPP/*.cpp *.cpp

TARGETS    = $(GNA_FW) $(TINY_XML) $(GAME)

OBJFILES  := $(patsubst %.cpp,%.o,$(wildcard $(TARGETS)))




all: $(OBJFILES)
	$(CC) -o $(EXECUTABLE) $(OBJFILES) $(LIBS)
	
%.o: %.cpp
	$(COMPILE) -o $@ $<
	
clean:
	rm -f $(OBJFILES)
	rm -f Compiled/juego

clean_game:
	rm -f Logica/CPPS/*.o Graficas/*.o *.o