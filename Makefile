CC=gcc
OBJDIR:=$(shell [ -d obj ] || mkdir obj && echo "obj")
CFLAGS=-Wall -Wextra -std=gnu11

TARGETS=main
# main_OBJ=basic.o cJSON.o list.o shuffle.o player.o game.o init_card.o init_character.o init_game.o event.o game_play.o draw.o select.o discard.o dodge.o display.o determine.o bang.o card_func.o main.o
main_OBJ =  basic.o list.o cJSON.o shuffle.o player.o game.o game_play.o \
			init_card.o init_character.o init_game.o \
			event.o bang.o card_func.o damage.o death.o determine.o discard.o dodge.o draw.o lethal.o select.o \
			display.o main.o 


.PHONY: all

all: CFLAGS:=$(CFLAGS) -O2
all: $(TARGETS)

debug: CFLAGS:=$(CFLAGS) -g -DDEBUG -fsanitize=leak -fsanitize=undefined
debug: LDFLAGS:=$(LDFLAGS) -fsanitize=address -lubsan -lasan
debug: $(TARGETS)

dev: CFLAGS:=$(CFLAGS) -g -DDEBUG
dev: LDFLAGS:=$(LDFLAGS)
dev: $(TARGETS)


.SECONDEXPANSION:
$(TARGETS): $$(patsubst %, $(OBJDIR)/%, $$($$@_OBJ))
	$(CC) $(filter %.o, $^) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(TARGETS) obj
