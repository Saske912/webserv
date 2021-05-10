NAME = progs
FLAGS = -Wall -Wextra -Werror -Llibft -lft -g
SERV = webserv
CLI = client
SRC_FLD = source/
SRC = ${SRC_FLD}error.cpp ${SRC_FLD}init.cpp ${SRC_FLD}loop.cpp ${SRC_FLD}config.cpp \
${SRC_FLD}server.cpp ${SRC_FLD}route.cpp ${SRC_FLD}tests.cpp \
$(SRC_FLD)header.cpp ${SRC_FLD}find.cpp
SERV_SRC = ${SRC} serverf/main.cpp
CLI_SRC = ${SRC} clientf/main.cpp

${NAME} : header.h
	g++  ${FLAGS} ${SERV_SRC} -o ${SERV}
	g++ ${FLAGS} ${CLI_SRC} -o ${CLI}

all: ${NAME}

clean:
	rm -f ${SERV} ${CLI}

re: clean all
