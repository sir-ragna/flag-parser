#CC = clang
CFLAGS = -Wall -std=c89
TARGET = example

all: $(TARGET)

debug: $(TARGET).c
	$(CC) -g -Wall -o $(TARGET) $(TARGET).c

perf: $(TARGET).c
	$(CC) -pg -g -Wall -o $(TARGET) $(TARGET).c

release: $(TARGET).c
	$(CC) -O2 -Wall -o $(TARGET) $(TARGET).c

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)