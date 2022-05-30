SRCS	=	main.cpp \
			get_response.cpp \
			utils.cpp \
			init/server.cpp \
			init/exception_server_not_listening.cpp \
			class/request.cpp \
			class/response.cpp \
			class/route.cpp \
			class/webserv_conf.cpp \
			http_header/Date.cpp \
			http_header/Server.cpp \
			http_header/Data-length.cpp \
			http_header/Content-Type.cpp \

OBJS	=	${SRCS:%.cpp=./.build/%.o}

NAME	=	webserv

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
