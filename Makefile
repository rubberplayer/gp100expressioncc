CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lasound

TARGET = gp100expressioncc
SRC = src/midi_mapper.c src/send_event.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS) 

clean:
	rm -f $(TARGET)

.PHONY: all clean
