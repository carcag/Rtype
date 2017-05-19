#ifndef SERVER_H
# define SERVER_H

# include <vector>
# include <list>
# include <string>

# include "net/TcpListener.h"
# include "Client.h"
# include "Lobby.h"
# include "Game.h"

# define TCP_PORT (2020)

class Server {
 private:
  TcpListener listener_;
  std::vector<Client *> clients_;
  std::vector<Lobby *> lobbies_;
  std::vector<Game *> games_;
  int client_id_;
  int lobby_id_;
  int game_id_;

  Server();
  // Util functions
  Lobby *get_lobby_by_id(int id);
  Game *get_game_by_id(int id);
  // Cleaning functions
  void CleanDisconnectedClients();
  void CleanEmptyLobbies();
  void CleanEmptyGames();

 public:
  ~Server();
  Server(const Server&) = delete;
  Server operator=(const Server&) = delete;
  // Server is a singleton
  static Server& get() {
    static Server instance;
    return instance;
  }

  const std::vector<Lobby *>& get_lobbies() { return lobbies_; };

  // Server's API
  Lobby *AddClientToLobby(Client *c, int lobby_id);
  bool RemoveClientFromLobby(Client *c, int lobby_id);
  Lobby *CreateLobby(Client *client, const std::string& lobby_name);
  bool SetClientReady(Client *c, int lobby_id);
  int CreateGame(std::vector<Client *> clients);
  bool RemoveClientFromGame(Client *c, int game_id);
  void Loop();
};

#endif  // SERVER_H
