CC = clang
CFLAGS = -Wall -g -Werror -std=c89

all: $(TARGET)
	$(CC) $(CFLAGS) -o define_rest_collection define_rest_collection.c
	$(CC) $(CFLAGS) -o null_define_bool null_define_bool.c
	$(CC) $(CFLAGS) -o null_define_int null_define_int.c
	$(CC) $(CFLAGS) -o null_define_string null_define_string.c
	$(CC) $(CFLAGS) -o illegal_redefine illegal_redefine.c
	$(CC) $(CFLAGS) -o repeated_parse_flags repeated_parse_flags.c

valgrind:
	valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=33 ./define_rest_collection
	valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=33 ./null_define_bool
	valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=33 ./null_define_int
	valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=33 ./null_define_string
	valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=33 ./illegal_redefine
	valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=33 ./repeated_parse_flags

clean:
	$(RM) define_rest_collection
	$(RM) null_define_bool 
	$(RM) null_define_int
	$(RM) null_define_string
	$(RM) illegal_redefine 
	$(RM) repeated_parse_flags 
