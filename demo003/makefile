SRC:=main.cpp object.cpp events.cpp test.cpp
OBJ:=$(SRC:.cpp=.o)
LIBRARIES:=-lglfw -lGLEW -lGL
FLAGS:=-g -Wall

build:$(OBJ)
	g++ $(OBJ) $(LIBRARIES) $(FLAGS) -o a.out
%.o: %.cpp
	g++ $< -c $(LIBRARIES) $(FLAGS) -o $@
clean:
	rm $(OBJ) a.out -f
