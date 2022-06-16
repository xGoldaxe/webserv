INTERNAL_SRCS	=	internal/util.cpp \

SRCS	=	../srcs/utils/file.cpp \
			../srcs/utils/string.cpp \
			../srcs/utils/utils.cpp \
			../srcs/errors/http_code.cpp \

TESTS	=	main.cpp\
			tests/demo.cpp \
			tests/chunk.cpp \
			tests/content-length.cpp \
			tests/utils.cpp \

INTERNAL_HEADERS = 	internal/assert_object.hpp \
					internal/util.hpp \

HEADERS =	../srcs/utils/file.hpp \
			../srcs/utils/string.hpp \
			../srcs/utils/utils.hpp \
			../srcs/errors/http_code.hpp \
			../srcs/class/chunk_buffer.hpp \

INT_OBJS	=	${INTERNAL_SRCS:%.cpp=.build/%.o}
OBJS		=	${SRCS:%.cpp=%.o}
TEST_OBJS	= 	${TESTS:%.cpp=.build/%.o}

DEPS		=	${HEADERS:%.hpp=%.hpp}
INT_DEPS	=	${INTERNAL_HEADERS:%.hpp=%.hpp}

NAME	=	tester

CXXFLAGS	=	-Wall -Wextra -Werror -I. -std=c++98 -D DEBUG  -g -O2

RM		=	rm -rf

CXX		=	c++

# COLORS
NONE			= \033[0m
CL_LINE			= \033[2K
B_RED			= \033[1;31m
B_GREEN			= \033[1;32m 
B_MAGENTA 		= \033[1;35m
B_CYAN 			= \033[1;36m

%.o: %.cpp $(DEPS) $(INT_DEPS)
		@$(CXX) $(CXXFLAGS) -Isrcs/ -o $@ -c $<
		@printf "${B_MAGENTA}Compilling $< ...\n${NONE}"

.build/%.o: %.cpp $(DEPS)
		@mkdir -p $(@D)
		@$(CXX) $(CXXFLAGS) -Isrcs/ -o $@ -c $<
		@printf "${B_MAGENTA}Compilling $< ...\n${NONE}"

all:	
		@mkdir -p .build
		@make $(NAME) --no-print-directory

${NAME}:	${INT_OBJS} ${OBJS} ${TEST_OBJS}
		@$(CXX) $(CXXFLAGS) $(OBJS) $(TEST_OBJS) -o $(NAME)
		@printf "${B_GREEN}==>{${NAME}} LINKED SUCCESFULLY<==${NONE}\n"

clean:
		@${RM} ${OBJS}
		@${RM} .build
		@printf "${B_RED}XX clean XX\n${NONE}"

fclean:	clean
		@${RM} ${NAME}
		@printf "${B_RED}XX fclean XX\n${NONE}"

re:		fclean all

.PHONY:	all clean fclean re
