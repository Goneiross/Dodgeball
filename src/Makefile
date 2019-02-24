CC     = g++
CFLAGS = -g -Wall -std=c++11
OFILES = main.o window.o

all:
	test

clean:
	@/bin/rm -f *.o *.x *.c~ *.h~ prog

window.o: window.cc window.h
	$(CC) $(CFLAGS) `pkg-config gtkmm-3.0 --cflags --libs` -c $< -o $@

main.o: main.cc window.h
	$(CC) $(CFLAGS) `pkg-config gtkmm-3.0 --cflags --libs` -c $< -o $@


test: $(OFILES)
	$(CC) $(OFILES) `pkg-config gtkmm-3.0 --cflags --libs` -o $@