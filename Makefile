CC     = g++
CFLAGS = -g -Wall -std=c++11
OFILES = main.o test.o

all:
	test

clean:
	@/bin/rm -f *.o *.x *.c~ *.h~ prog

test.o: test.cc test.h
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.cc test.h
	$(CC) $(CFLAGS) `pkg-config gtkmm-3.0 --cflags --libs` -c $< -o $@


test: $(OFILES)
	$(CC) $(OFILES) -o $@