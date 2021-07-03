NAME	= webserv

SRCDIR	= source/

CONFDIR	= ${SRCDIR}config/

NODEDIR	= ${CONFDIR}node/

SERVDIR	= serverf/


SRCS	= ${SRCDIR}Number.cpp ${SRCDIR}URI.cpp ${SRCDIR}config.cpp ${SRCDIR}cgi_path.cpp\
		  ${SRCDIR}default_config.cpp ${SRCDIR}error.cpp  ${SRCDIR}update_descriptors.cpp\
		  ${SRCDIR}ft_doublefree.cpp ${SRCDIR}get_next_line.cpp \
		  ${SRCDIR}get_next_line_utils.cpp ${SRCDIR}header.cpp ${SRCDIR}init.cpp ${SRCDIR}datetime.cpp \
		  ${SRCDIR}loop.cpp ${SRCDIR}route.cpp ${SRCDIR}server.cpp ${SRCDIR}tests.cpp \
		  ${SRCDIR}trim.cpp ${SRCDIR}send_protected.cpp ${SRCDIR}file_available.cpp \
		  ${SRCDIR}templates.cpp ${SRCDIR}responce.cpp ${SRCDIR}communication_with_clients.cpp \
		  ${SRCDIR}split_buffer.cpp ${SRCDIR}get_http_line.cpp ${SRCDIR}parse_request.cpp \
		  ${CONFDIR}Interpreter.cpp ${CONFDIR}Lexer.cpp ${CONFDIR}ParseResult.cpp ${CONFDIR}Parser.cpp \
		  ${CONFDIR}Position.cpp ${CONFDIR}Token.cpp ${CONFDIR}Validator.cpp ${CONFDIR}parse.cpp \
		  ${NODEDIR}ANode.cpp ${NODEDIR}ConfigNode.cpp ${NODEDIR}ErrorNode.cpp \
		  ${NODEDIR}IdentifierNode.cpp ${NODEDIR}ParamNode.cpp ${NODEDIR}RouteNode.cpp \
		  ${NODEDIR}ServerNode.cpp \
		  ${SERVDIR}main.cpp

OBJS	= ${SRCS:.cpp=.o}

FLAGS	= -Wall   -I. -I${SRCDIR} -I${CONFDIR} -I${NODEDIR}

.cpp.o:		${OBJS}
			clang++ ${FLAGS} -c $< -o ${<:.cpp=.o}

all:		${NAME}

client:
			make -C clientf

${NAME}:	${OBJS}
			@clang++ ${FLAGS} ${OBJS} -o ${NAME}
			@echo  "\033[36mWebserver succesfully compiled\033[0m"

clean:
			rm -rf ${OBJS}

fclean:		clean
			rm -f ${NAME}

re:			fclean all

.PHONY:		.cpp.o all clean fclean re client
