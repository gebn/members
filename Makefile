CFLAGS = -Wall -Werror -O2
OBJ = help.o group.o main.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

members: $(OBJ)
	$(CC) -o $@ $^

clean:
	rm -f *.o members
