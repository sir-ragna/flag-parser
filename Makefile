#CC = clang
CFLAGS = -Wall -std=c89
TARGET = example

all: $(TARGET)

test: $(TARGET).c
	g++ -Wall -std=gnu++17    -o $(TARGET) $(TARGET).c          # Test C++17 compat
	gcc -Wall -std=c89     -g -o $(TARGET) $(TARGET).c          # Test c89 compat
	valgrind --leak-check=full ./example -n 999 -z Hallo -a xzy # Test memory leaks

debug: $(TARGET).c
	$(CC) -g -Wall -o $(TARGET) $(TARGET).c

release: $(TARGET).c
	$(CC) -O2 -Wall -o $(TARGET) $(TARGET).c

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)