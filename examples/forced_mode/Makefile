CC = gcc
CFLAGS = -Wall -Wextra
INCLUDES = -I. -I../common -I../..
LIBS = -lrt

TARGET = forced_mode
SOURCES = forced_mode.c ../common/common.c ../../bme68x.c

OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

help:
	@echo "Usage: make [target]"
	@echo "Targets:"
	@echo "  all        Build the BME68X forced mode example (default)"
	@echo "  clean      Remove compiled files"
	@echo "  help       Display this help message"

depend:
	$(CC) -MM $(CFLAGS) $(INCLUDES) $(SOURCES) > .depend

-include .depend

.PHONY: all clean help depend