SRCS	=	main.cpp \
			exit_handle.cpp \
			utils/file.cpp \
			utils/string.cpp \
			init/server.cpp \
			utils/utils.cpp \
			utils/bundle_server_management.cpp \
			init/connection.cpp \
			init/exception_server_not_listening.cpp \
			class/request.cpp \
			class/request_constructor.cpp \
			class/response.cpp \
			class/route.cpp \
			configuration/redirection.cpp \
			configuration/webserv.cpp \
			configuration/server.cpp \
			configuration/bundle_server.cpp \
			http_header/Date.cpp \
			http_header/Server.cpp \
			http_header/Data-length.cpp \
			http_header/Content-Type.cpp \
			internal/mime_types.cpp \
			cgi/cgi_manager.cpp \
			errors/http_code.cpp \
			class/response_error_page.cpp \
			request_header/request_header.cpp \
			processing/multipart_form_data.cpp \
			../request_parsing/srcs/parse_request.cpp \
			../request_parsing/srcs/parse_url.cpp \
			../request_parsing/srcs/utils.cpp \
			../request_parsing/srcs/verify_absolute_url.cpp \

HEADERS =	errors/http_code.hpp \
			exit_handle.hpp \
			init/exception_server_not_listening.hpp \
			init/server.hpp \
			http_header/http_header.hpp \
			webserv.hpp \
			class/request.hpp \
			class/response.hpp \
			class/route.hpp \
			configuration/webserv.hpp \
			configuration/server.hpp \
			cgi/cgi_manager.hpp \
			internal/mime_types.hpp \
			utils/file.hpp \
			utils/string.hpp \

OBJS	=	${SRCS:%.cpp=.build/%.o}
DEPS	=	${HEADERS:%.hpp=srcs/%.hpp}

NAME	=	webserv

CXXFLAGS	=	-Wall -Wextra -Werror -I. -std=c++98 -D DEBUG # -g -O2

RM		=	rm -rf

CXX		=	GLIBCXX_FORCE_NEW=1 c++

# COLORS
NONE			= \033[0m
CL_LINE			= \033[2K
B_RED			= \033[1;31m
B_GREEN			= \033[1;32m 
B_MAGENTA 		= \033[1;35m
B_CYAN 			= \033[1;36m

.build/%.o: srcs/%.cpp $(DEPS)
		@mkdir -p $(@D)
		@$(CXX) $(CXXFLAGS) -Isrcs/ -o $@ -c $<
		@printf "${B_MAGENTA}Compilling $< ...\n${NONE}"

all:	
		@mkdir -p .build
		@make $(NAME) --no-print-directory

${NAME}:	${OBJS}
		@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
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
