NAME = progs
FLAGS = -Wall -Wextra -Werror
SERV = webserv
CLI = client
SRC_FLD = source/
SRC = ${SRC_FLD}error.cpp
SERV_SRC = ${SRC} serverf/main.cpp
CLI_SRC = ${SRC} clientf/main.cpp

${NAME} : header.h
	g++ ${FLAGS} ${SERV_SRC} -o ${SERV}
	g++ ${FLAGS} ${CLI_SRC} -o ${CLI}

all: ${NAME}

clean:
	rm -f ${SERV} ${CLI}

re: clean all
