CC     = g++
CFLAGS = -g -Wall -std=c++11
OFILES = ball.o GUI.o main.o map.o player.o simulation.o tools.o

projet: $(OFILES)
	$(CC) $(OFILES) `pkg-config gtkmm-3.0 --cflags --libs` -o $@
	
clean:
	@/bin/rm -f *.o *.x *.c~ *.h~ projet

ball.o: ball.cc ball.h tools.h
	$(CC) $(CFLAGS) -c $< -o $@	

GUI.o: GUI.cc GUI.h 
	$(CC) $(CFLAGS) `pkg-config gtkmm-3.0 --cflags --libs` -c $< -o $@

main.o: main.cc GUI.h simulation.h
	$(CC) $(CFLAGS) `pkg-config gtkmm-3.0 --cflags --libs` -c $< -o $@

map.o: map.cc map.h tools.h define.h
	$(CC) $(CFLAGS) -c $< -o $@	

player.o: player.cc player.h tools.h
	$(CC) $(CFLAGS) -c $< -o $@	

simulation.o: simulation.cc simulation.h ball.h map.h player.h error.h
	$(CC) $(CFLAGS) -c $< -o $@	

tools.o: tools.cc tools.h
	$(CC) $(CFLAGS) -c $< -o $@	