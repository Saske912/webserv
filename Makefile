NAME = progs
FLAGS = -Wall -Wextra -Werror -Llibft -lft -I. -Isource -Isource/config -g
SERV = webserv
CLI = client
SRC_FLD = source/
CONF_FLD = ${SRC_FLD}config/
SRC = ${SRC_FLD}error.cpp ${SRC_FLD}init.cpp ${SRC_FLD}loop.cpp ${SRC_FLD}config.cpp \
${SRC_FLD}server.cpp ${SRC_FLD}route.cpp ${SRC_FLD}tests.cpp \
$(SRC_FLD)header.cpp ${SRC_FLD}find.cpp ${SRC_FLD}default_config.cpp \
${CONF_FLD}Interpreter.cpp ${CONF_FLD}Lexer.cpp ${CONF_FLD}Node.cpp \
${CONF_FLD}ParseResult.cpp ${CONF_FLD}Parser.cpp ${CONF_FLD}Position.cpp \
${CONF_FLD}Token.cpp ${CONF_FLD}parse.cpp
SERV_SRC = ${SRC} serverf/main.cpp
CLI_SRC = ${SRC} clientf/main.cpp

${NAME} : header.h
	g++  ${FLAGS} ${SERV_SRC} -o ${SERV}
	#g++ ${FLAGS} ${CLI_SRC} -o ${CLI}

all: ${NAME}

clean:
	rm -f ${SERV} ${CLI}

re: clean all
