CC = clang
CFLAGS = -Wall -Werror -std=c89
TARGET = example

all: $(TARGET)

# Test compilation with gcc and clang in c89 and c++17 and check for
# memory leaks with valgrind
test: $(TARGET).c
	g++ -Wall -Werror -std=gnu++17    -o $(TARGET) $(TARGET).c  # Test C++17 compat with gcc
	cat $(TARGET).c > $(TARGET).cpp # clang++ doesn't accept *.c files only *.cpp files
	clang++ -Wall -Werror -std=gnu++17 -o $(TARGET) $(TARGET).cpp  # Test C++17 compat with clang
	rm $(TARGET).cpp
	gcc   -Wall -Werror -std=c89 -g -o $(TARGET) $(TARGET).c  # Test c89 compat with gcc
	clang -Wall -Werror -std=c89 -g -o $(TARGET) $(TARGET).c  # Test c89 compat with clang
	valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=33 ./example --help
	valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=33 ./example --keep
	valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=33 ./example --dns 8.8.8.8


debug: $(TARGET).c
	$(CC) $(CFLAGS) -g -o $(TARGET) $(TARGET).c

release: $(TARGET).c
	$(CC) $(CFLAGS) -O2 -o $(TARGET) $(TARGET).c

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)