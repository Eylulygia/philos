NAME        := philo
CC        := gcc
FLAGS    := -Wall -Wextra -Werror -g -I include

SRCS        :=      main.c \
                          src/args.c \
                          src/sim.c \
                          src/monitor.c \
                          src/teardown.c \
                          src/routine.c \
                          src/eat.c \
                          src/ftime.c \
                          src/log.c \
                          src/state.c \
                          src/init.c \
                          src/parser.c \
                          
OBJS        := $(SRCS:.c=.o)

.c.o:
	${CC} ${FLAGS} -c $< -o ${<:.c=.o}


CLR_RMV		:= \033[0m
RED		    := \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
CYAN 		:= \033[1;36m
RM		    := rm -f

${NAME}:	${OBJS}
			@echo "${YELLOW}$(NAME) $(GREEN)Derleniyor ${CLR_RMV}..."
			${CC} ${FLAGS} -o ${NAME} ${OBJS}
			@echo "$(GREEN)$(NAME) Başarıyla oluşturuldu[0m ✔️"

all:		${NAME}

bonus:		all

clean:
			@ ${RM} *.o */*.o */*/*.o
			@ echo "$(CYAN)$(NAME) $(CLR_RMV)object dosyaları $(RED)Siliniyor... ✔️"

fclean:		clean
			@ ${RM} ${NAME}
			@ echo "$(CYAN)$(NAME) $(CLR_RMV)dosyası $(RED)Siliniyor... ✔️"

re:			fclean all

.PHONY:		all clean fclean re
