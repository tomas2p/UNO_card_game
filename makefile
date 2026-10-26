CC = g++
CXXFLAGS = -std=c++17 -o
SRC = src/
OUT = game_uno++
OBJ0 = ${SRC}card_class.cc
OBJ1 = ${SRC}card_pack_class.cc
OBJ2 = ${SRC}game_ia.cc
OBJ3 = ${SRC}game_online.cc
OBJ4 = ${SRC}player_class.cc
OBJ5 = ${SRC}uno_main.cc
OBJ6 = ${SRC}uno_tools.cc
OBJ7 = ${SRC}game_class.cc
OBJ = ${OBJ0} ${OBJ1} ${OBJ2} ${OBJ3} ${OBJ4} ${OBJ5} ${OBJ6} ${OBJ7}

all: ${OUT}

${OUT}:
	${CC} ${CXXFLAGS} $@ ${OBJ}
clean:
	rm -f ./${OUT}