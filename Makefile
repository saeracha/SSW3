CC = g++ 
CFLAGS = -O2 
TARGET = Tetris
SERVER = Server

SRCDIR = src
OBJDIR = obj

SOURCES := $(wildcard $(SRCDIR)/*.cc)
OBJS :=  $(SOURCES:$(SRCDIR)/%.cc=$(OBJDIR)/%.o)

All : $(SERVER) $(TARGET)

$(SERVER) : ./src/server/server.cc
	@$(CC) -o $@ $^ -lpthread -D_REENTRANT

$(TARGET) : $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $(SOURCES) -lsfml-graphics -lsfml-window -lsfml-system -lpthread -D_REENTRANT
	@echo "Linking complete!"

$(OBJS) : $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo ""$<" Compiled!"

clean:
	@rm -f $(OBJS) $(TARGET) $(SERVER)
	@echo "Clean up!"
