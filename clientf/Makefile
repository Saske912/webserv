POST		= post

PUT			= put

GET			= get



POSTS		= Post.cpp

GETS		= Get.cpp

PUTS		= Put.cpp


all:	${POST} ${PUT} ${GET}


${POST}: ${POSTS}
		clang++ -Wall -Wextra -Werror ${POSTS} -o ${POST}

${PUT}: ${PUTS}
		clang++ -Wall -Wextra -Werror ${PUTS} -o ${PUT}

${GET}: ${GETS}
		clang++ -Wall -Wextra -Werror ${GETS} -o ${GET}

clean:

fclean: clean
		rm -rf ${PUT} ${POST} ${GET}

re:		fclean all

.PHONY:	all clean fclean re
