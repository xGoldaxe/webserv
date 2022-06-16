SRCS	=	main.cpp\
			../srcs/utils/file.cpp \
			../srcs/utils/string.cpp \
			../srcs/utils/utils.cpp \
			../srcs/errors/http_code.cpp \
			tests/chunk.cpp \
			tests/content-length.cpp \

HEADERS =	internal/assert_object.hpp \
			internal/util.hpp \
			../srcs/utils/file.hpp \
			../srcs/utils/string.hpp \
			../srcs/utils/utils.hpp \
			../srcs/errors/http_code.hpp \
			../srcs/class/chunk_buffer.hpp \


NEW_SRCS =	$(addprefix .build/, $(notdir $(SRCS) ) )

OB 	= ${NEW_SRCS:%.cpp=%.o}

OBJS	=	${SRCS:%.cpp=%.o}

DEPS	=	${HEADERS:%.hpp=srcs/%.hpp}

NAME	=	tester

CPPFLAGS	=	-Wall -Wextra -Werror -I. -std=c++98 -D DEBUG

RM		=	rm -rf

CXX		=	c++

# COLORS
NONE			= \033[0m
CL_LINE			= \033[2K
B_RED			= \033[1;31m
B_GREEN			= \033[1;32m
B_MAGENTA 		= \033[1;35m
B_CYAN 			= \033[1;36m

%.o : %.cpp 
		@$(CXX) ${CPPFLAGS} -I. -o .build/$(notdir $@) -c $? 
		@printf "${B_MAGENTA}Compilling $? ...\n${NONE}"

all:	
		@bash create_symbolic.sh "$(HEADERS) $(SRCS)"
		@make ${NAME} --no-print-directory


${NAME}:	${OB}
		@${CXX} ${CPPFLAGS} ${OB} -o ${NAME} 
		@printf "${B_GREEN}==>{${NAME}} LINKED SUCCESFULLY<==${NONE}\n"

clean:
		@${RM}
		@${RM} ./.build
		@printf "${B_RED}XX clean XX\n${NONE}"

fclean:	clean
		@${RM} ${NAME}
		@printf "${B_RED}XX fclean XX\n${NONE}"

re:		fclean all

.PHONY:	all clean fclean re ${NAME}
