SRCS	=	print_or_store.cpp \
			main.cpp \
			parse_request.cpp \
			parse_url.cpp \
			utils.cpp \
			verify_absolute_url.cpp \
			outside/http_code.cpp \
			outside/string.cpp \

OBJS	=	${SRCS:%.cpp=./.build/%.o}

NAME	=	reqparser

CPPFLAGS	=	-Wall -Wextra -Werror -I. -std=c++98 -D DEBUG

RM		=	rm -rf

CCP		=	c++

# COLORS
NONE			= \033[0m
CL_LINE			= \033[2K
B_RED			= \033[1;31m
B_GREEN			= \033[1;32m
B_MAGENTA 		= \033[1;35m
B_CYAN 			= \033[1;36m

./.build/%.o : srcs/%.cpp
		@mkdir -p $(@D)
		@$(CCP) ${CPPFLAGS} -I. -o $@ -c $?
		@printf "${B_MAGENTA}Compilling $? ...\n${NONE}"

all:	
		@mkdir -p .build
		@make ${NAME} --no-print-directory

${NAME}:	${OBJS}
		@${CCP} ${CPPFLAGS} ${OBJS} -o ${NAME}
		@printf "${B_GREEN}==>{${NAME}} LINKED SUCCESFULLY<==${NONE}\n"

clean:
		@${RM}
		@${RM} ./.build
		@printf "${B_RED}XX clean XX\n${NONE}"

fclean:	clean
		@${RM} ${NAME}
		@printf "${B_RED}XX fclean XX\n${NONE}"

re:		fclean all

.PHONY:	all clean fclean re
