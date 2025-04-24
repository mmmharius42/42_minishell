# Variables
CC				= cc
CFLAGS			= -Wall -Wextra -Werror -g

# Couleurs pour les messages
GREEN			= \033[0;32m
RESET			= \033[0m

# Includes et Libs spécifiques
INCLUDES_LINUX	= -Ilibs/Libft -I. -Ilibs/gnl -I/usr/include
INCLUDES_MAC 	= -I/opt/homebrew/opt/readline/include -Ilibs/Libft -I. -Ilibs/gnl -DREADLINE

LIBS_LINUX		= libs/Libft/libft.a -L/usr/lib -lreadline -lncurses
LIBS_MAC		= libs/Libft/libft.a -L/opt/homebrew/opt/readline/lib -lreadline -lncurses

# Détection automatique du système
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    INCLUDES = $(INCLUDES_LINUX)
    LIBS = $(LIBS_LINUX)
else ifeq ($(UNAME_S), Darwin)
    INCLUDES = $(INCLUDES_MAC)
    LIBS = $(LIBS_MAC)
endif

# Fichiers sources
SRCS			= main.c src/clean.c \
					src/input/check_input.c src/input/print_path.c \
					src/token/token_manip.c src/token/token_utils.c \
					src/token/token_utils2.c src/token/token.c \
					src/token/token_operator.c src/token/extract_word.c \
					src/expand/expand.c \
					src/expand/variable_expand.c src/expand/variable_expand_utils.c \
					libs/utils/ft_is_path.c libs/utils/ft_env_utils.c \
					libs/utils/ft_env_utils2.c

# Dossier pour les objets
OBJDIR			= objs
OBJS			= $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))
NAME			= minishell
LIBFT			= libs/Libft/libft.a

# Création des dossiers nécessaires
OBJDIRS			= $(sort $(dir $(OBJS)))



# Compilation automatique selon le système
all: $(OBJDIRS) $(LIBFT) $(NAME)

$(OBJDIRS): .stamp

.stamp:
	mkdir -p $(OBJDIRS)
	touch .stamp

updateSources:
	@bash -c '\
		IGNORED_DIRS=("libs" "src/dontcopy"); \
		FIND_CMD="find -type f -name '\''*.c'\''"; \
		for dir in "$${IGNORED_DIRS[@]}"; do \
			FIND_CMD+=" ! -path './\''$$dir/*'\''"; \
		done; \
		eval $$FIND_CMD | sort | awk '\''{print "\t" $$0 " \\"}'\'' \
		| sed '\''$$ s/ \\$$//'\'' > .sources_block; \
		cat .sources_block; \
		echo "SOURCES = \\" > .sources_full; \
		cat .sources_block >> .sources_full; \
		awk '\'' \
			FNR==NR { lines[NR] = $$0; next } \
			/^SOURCES =/ { \
				for (i=1; i<=length(lines); i++) print lines[i]; \
				in_block=1; next \
			} \
			in_block && /^[^\t]/ { in_block=0 } \
			!in_block { print } \
		'\'' .sources_full Makefile > Makefile.tmp; \
		mv Makefile.tmp Makefile; \
		rm -f .sources_block .sources_full; \
		echo "✅ Makefile mis à jour avec les fichiers sources." \
	'

$(LIBFT):
	@make -C libs/Libft

# Ajouter une dépendance pour éviter le relink
$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(GREEN)Minishell compiled !$(RESET)"

# Ajouter la dépendance aux fichiers d'en-tête (.h)
$(OBJDIR)/%.o: %.c minishell.h | $(OBJDIR)
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	find $(OBJDIR) -type f -name "*.o" -delete
	rm -f $(OBJDIRS)/.stamp 2>/dev/null || true
	@make -C libs/Libft clean

fclean: clean
	rm -f $(NAME)
	@make -C libs/Libft fclean

re: fclean all

.PHONY: all clean fclean re mac $(OBJDIRS)
