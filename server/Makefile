CC := g++

RM := rm -f

TARGET := rtype_server

INCLUDES_DIR := -Isrc/ -I../common/

LIB := lib/

SOURCES_DIR := src/

CXXFLAGS += -g -std=c++11 $(INCLUDES_DIR)

LDFLAGS += -lpthread

UNIX := unix/

NET := net/

THREADS := threads/

SOURCES := $(SOURCES_DIR)main.cc \
           $(SOURCES_DIR)$(THREADS)unixThread.cc \
           $(SOURCES_DIR)$(THREADS)threadException.cc \
           $(SOURCES_DIR)$(NET)IpAddress.cc \
           $(SOURCES_DIR)$(NET)SocketException.cc \
           $(SOURCES_DIR)$(NET)$(UNIX)UnixSocket.cc \
           $(SOURCES_DIR)$(NET)$(UNIX)UnixUdpSocket.cc \
           $(SOURCES_DIR)$(NET)$(UNIX)UnixTcpSocket.cc \
           $(SOURCES_DIR)$(NET)$(UNIX)UnixTcpListener.cc \
           $(SOURCES_DIR)Client.cc \
           $(SOURCES_DIR)Lobby.cc \
           $(SOURCES_DIR)Server.cc \
           $(SOURCES_DIR)Game.cc

OBJECTS := $(SOURCES:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(TARGET)

re: fclean all

.PHONY: all clean fclean re
