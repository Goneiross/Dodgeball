CC     = g++
CFLAGS = -g -std=c++11
OFILES = ball.o GUI.o main.o map.o player.o simulation.o tools.o obstacle.o floyd.o pathfinding.o aStar.o

projet: $(OFILES)
	$(CC) $(OFILES) `pkg-config gtkmm-3.0 --cflags --libs` -o $@
	
clean:
	@/bin/rm -f *.o *.x *.c~ *.h~ projet

ball.o: ball.cc ball.h tools.h map.h define.h
	$(CC) $(CFLAGS) -c $< -o $@	

GUI.o: GUI.cc GUI.h simulation.h define.h tools.h
	$(CC) $(CFLAGS) `pkg-config gtkmm-3.0 --cflags --libs` -c $< -o $@

main.o: main.cc GUI.h simulation.h
	$(CC) $(CFLAGS) `pkg-config gtkmm-3.0 --cflags --libs` -c $< -o $@

map.o: map.cc map.h
	$(CC) $(CFLAGS) -c $< -o $@	 

obstacle.o : obstacle.cc obstacle.h map.h tools.h define.h
	$(CC) $(CFLAGS) -c $< -o $@	

player.o: player.cc player.h map.h tools.h pathfinding.h define.h
	$(CC) $(CFLAGS) -c $< -o $@	

simulation.o: simulation.cc simulation.h ball.h obstacle.h player.h error.h GUI.h pathfinding.h
	$(CC) $(CFLAGS) -c $< -o $@	

pathfinding.o: pathfinding.cc pathfinding.h player.h obstacle.h floyd.h aStar.h define.h
	$(CC) $(CFLAGS) -c $< -o $@	

floyd.o: floyd.cc floyd.h define.h tools.h player.h obstacle.h
	$(CC) $(CFLAGS) -c $< -o $@	

aStar.o: aStar.cc aStar.h obstacle.h player.h tools.h define.h
	$(CC) $(CFLAGS) -c $< -o $@	