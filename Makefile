CC = gcc
CFLAGS = -Wall -Werror

SRCS_LIST = main.c grafo_listaadj.c
OBJS_LIST = $(SRCS_LIST:.c=.o)
TARGET_LIST = ep1_lista_13831982.exe

SRCS_MATRIX = main.c grafo_matrizadj.c
OBJS_MATRIX = $(SRCS_MATRIX:.c=.o)
TARGET_MATRIX = ep1_matriz_13831982.exe

.PHONY: all clean

all: $(TARGET_LIST) $(TARGET_MATRIX)

$(TARGET_LIST): $(OBJS_LIST)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_MATRIX): $(OBJS_MATRIX)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS_LIST) $(OBJS_MATRIX)

cleanall: clean
	rm -f $(TARGET_LIST) $(TARGET_MATRIX)