CC := g++

RM := rm -f

TARGET := rtype_client

INCLUDES_DIR := -Isrc/ -I../common/ -Iinclude/

LIB := lib/unix/

SOURCES_DIR := src/

CXXFLAGS += -g -std=c++11 $(INCLUDES_DIR)

LDFLAGS += -L$(LIB) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -lpthread

CORE := core/

INTRO := intro/

CONNECT := connect/

MENU := menu/

LOBBIESLIST := lobbies_list/

LOBBY := lobby/

CREATELOBBY := create_lobby/

OPTIONS := options/

GAME := game/

GUI := gui/

NET := net/

SOURCES := $(SOURCES_DIR)main.cc \
           $(SOURCES_DIR)$(CORE)GameEngine.cc \
           $(SOURCES_DIR)$(INTRO)IntroState.cc \
           $(SOURCES_DIR)$(CONNECT)ConnectState.cc \
           $(SOURCES_DIR)$(MENU)MenuState.cc \
           $(SOURCES_DIR)$(LOBBIESLIST)LobbiesListState.cc \
           $(SOURCES_DIR)$(GUI)Button.cc \
           $(SOURCES_DIR)$(GUI)ToggleButton.cc \
           $(SOURCES_DIR)$(GUI)TextField.cc \
           $(SOURCES_DIR)$(GUI)ListItem.cc \
           $(SOURCES_DIR)$(GUI)List.cc \
           $(SOURCES_DIR)$(NET)Network.cc \
           $(SOURCES_DIR)$(LOBBY)LobbyState.cc \
           $(SOURCES_DIR)$(OPTIONS)OptionsState.cc \
           $(SOURCES_DIR)$(GAME)GameState.cc \
           $(SOURCES_DIR)$(GAME)ScrollingBackground.cc \
           $(SOURCES_DIR)$(CREATELOBBY)CreateLobbyState.cc

OBJECTS := $(SOURCES:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LDFLAGS)
	@echo "Done !"
	@echo "Run this command before the executable : export LD_LIBRARY_PATH=lib/unix"

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(TARGET)

re: fclean all

.PHONY: all clean fclean re
