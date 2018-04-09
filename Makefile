DIR_SRC = ./src
DIR_OBJ = ./obj
BIN_DIR = ./bin

SRC = $(wildcard ${DIR_SRC}/*.c)  
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC})) 

TARGET = FastProNGS

BIN_TARGET = ${BIN_DIR}/${TARGET}

CC = gcc -std=gnu89

${BIN_TARGET}:${OBJ}
	$(CC) $(OBJ) -lxml2 -g -lpthread -o $@
    
${DIR_OBJ}/%.o:${DIR_SRC}/%.c make_obj_dir make_bin_dir
	$(CC) $(CFLAGS) -O3 -c -g $< -o $@
.PHONY:clean
clean:
	rm obj/*.o
	rm bin/*
	rm $(TARGET)

make_obj_dir:
	@if test ! -d $(DIR_OBJ) ; \
	then \
		mkdir $(DIR_OBJ) ; \
	fi

make_bin_dir:
	@if test ! -d $(BIN_DIR) ; \
	then \
		mkdir $(BIN_DIR) ; \
	fi

install:
	install ${BIN_TARGET} ${TARGET}
	@echo "${TARGET} has been successfully installed at './'."
