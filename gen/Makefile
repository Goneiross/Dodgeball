CC     = g++
CFLAGS = -g -Wall -std=c++11
OFILES = generator.o
clean:
	@/bin/rm -f *.o *.x *.c~ *.h~ generator

generator.o: generator.cc define.h
	$(CC) $(CFLAGS) -c $< -o $@	

generator: $(OFILES)
	$(CC) $(OFILES) `pkg-config gtkmm-3.0 --cflags --libs` -o $@