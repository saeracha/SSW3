CC = g++ 
CFLAGS = -O2 
TARGET = a.out
SERVER = server
OBJS = main.o player.o settings.o block.o client.o
All : $(SERVER) $(TARGET)

$(SERVER) : server.cc
	$(CC) -o $@ $^ -lpthread -D_REENTRANT

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lsfml-graphics -lsfml-window -lsfml-system -lpthread -D_REENTRANT

%.o : %.cc
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGET) $(SERVER)
