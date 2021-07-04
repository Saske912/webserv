POST		= post

PUT			= put

GET			= get

DELETE		= delete

POSTS		= Post.cpp

GETS		= Get.cpp

PUTS		= Put.cpp

DELETS		= Delete.cpp


all:	${POST} ${PUT} ${GET} ${DELETE}


${POST}: ${POSTS}
		clang++ -Wall -Wextra -Werror ${POSTS} -o ${POST}

${PUT}: ${PUTS}
		clang++ -Wall -Wextra -Werror ${PUTS} -o ${PUT}

${GET}: ${GETS}
		clang++ -Wall -Wextra -Werror ${GETS} -o ${GET}

${DELETE}: ${DELETS}
		clang++ -Wall -Wextra -Werror ${DELETS} -o ${DELETE}

clean:

fclean: clean
		rm -rf ${PUT} ${POST} ${GET} ${DELETE}

re:		fclean all

.PHONY:	all clean fclean re
