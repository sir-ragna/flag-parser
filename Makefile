CC = clang
CFLAGS = -Wall -std=c89
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
	valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=33 ./example -n 999 -z Hallo -a xzy # Test memory leaks
	valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=33 ./example --hard -k         # Test memory leaks

debug: $(TARGET).c
	$(CC) -g -Wall -o $(TARGET) $(TARGET).c

release: $(TARGET).c
	$(CC) -O2 -Wall -o $(TARGET) $(TARGET).c

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)