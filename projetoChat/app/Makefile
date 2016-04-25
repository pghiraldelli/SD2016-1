# October 2006
# Prof.: Silvana Rossetto
#------------------------------------------------------------

VERSION = 1.0

ACTIVE_PASSENT = active_user.c
PASSIVE_PASSENT = passive_user.c
ACT = active_user
PASS = passive_user

#-------------------------------------------------------------
# the rest of the file should not change
INC = ../include
SRC = ../src
OBJ = ../obj
BIN = ../bin

CC = gcc -Wall
CFLAGS = -I$(INC)
LFLAGS = -lpthread
RM = rm

# make all
all:  $(OBJ)/mysocket.o $(BIN)/$(ACT) $(BIN)/$(PASS)

$(OBJ)/mysocket.o: $(SRC)/mysocket.c $(INC)/mysocket.h
	$(CC) -c $(CFLAGS) -o $@ $(SRC)/mysocket.c

$(BIN)/$(ACT): $(OBJ)/mysocket.o $(ACTIVE_PASSENT)
	$(CC) $(CFLAGS) -o $@ $(OBJ)/mysocket.o $(ACTIVE_PASSENT) $(LFLAGS)

$(BIN)/$(PASS): $(OBJ)/mysocket.o $(PASSIVE_PASSENT)
	$(CC) $(CFLAGS) -o $@ $(OBJ)/mysocket.o $(PASSIVE_PASSENT)
# version
version:
	@echo $(VERSION) "(Draft Version!)"
# clean
clean:
	$(RM) -rf $(OBJ)/*.o  $(BIN)/*
