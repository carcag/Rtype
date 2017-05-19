#include "Server.h"

#include <iostream>

#include "net/SocketException.h"

#ifdef __linux__
#include <signal.h>

volatile sig_atomic_t stop_loop;

void sig_handler(int signum) { stop_loop = 1; }
#endif

#ifdef _WIN32
#include <windows.h>
#endif

Server::Server() : client_id_(0), lobby_id_(0), game_id_(0) {
  listener_.Listen(TCP_PORT);
}

Server::~Server() {
  std::cout << "cleaning up..." << std::endl;
  lobbies_.clear();
  for (auto g : games_) {
    delete g;
  }
  games_.clear();
  for (auto l : lobbies_) {
    delete l;
  }
  lobbies_.clear();
  for (auto c : clients_) {
    delete c;
  }
  clients_.clear();
}

/*
** Util functions
*/

Lobby *Server::get_lobby_by_id(int id) {
  for (auto l : lobbies_) {
    if (l->get_id() == id) {
      return l;
    }
  }
  return nullptr;
}

Game *Server::get_game_by_id(int id) {
  for (auto g : games_) {
    if (g->get_id() == id) {
      return g;
    }
  }
  return nullptr;
}

/*
** Cleaning functions
*/

void Server::CleanDisconnectedClients() {
  auto it = clients_.begin();
  while (it != clients_.end()) {
    if ((*it)->to_remove()) {
      std::cout << "client " << (*it)->get_id() << " disconnected" << std::endl;
      delete *it;
      it = clients_.erase(it);
    } else {
      ++it;
    }
  }
}

void Server::CleanEmptyLobbies() {
  auto it = lobbies_.begin();
  while (it != lobbies_.end()) {
    if ((*it)->to_remove()) {
      std::cout << "removed empty lobby: " << (*it)->get_id() << std::endl;
      delete *it;
      it = lobbies_.erase(it);
    } else {
      ++it;
    }
  }
}

void Server::CleanEmptyGames() {
  auto it = games_.begin();
  while (it != games_.end()) {
    if ((*it)->to_remove()) {
      std::cout << "removed empty game: " << (*it)->get_id() << std::endl;
      delete *it;
      it = games_.erase(it);
    } else {
      ++it;
    }
  }
}

/*
** Server's API
*/

Lobby *Server::AddClientToLobby(Client *c, int lobby_id) {
  auto l = get_lobby_by_id(lobby_id);
  if (l != nullptr) {
    l->AddClient(c);
    return l;
  }
  return nullptr;
}

bool Server::RemoveClientFromLobby(Client *c, int lobby_id) {
  auto l = get_lobby_by_id(lobby_id);
  return l != nullptr ? l->RemoveClient(c) : false;
}

Lobby *Server::CreateLobby(Client *c, const std::string &lobby_name) {
  int lobby_id = lobby_id_++;
  lobbies_.push_back(new Lobby(lobby_id, lobby_name));
  auto lobby = AddClientToLobby(c, lobby_id);
  return lobby;
}

bool Server::SetClientReady(Client *c, int lobby_id) {
  auto l = get_lobby_by_id(lobby_id);
  return l != nullptr ? l->ClientChangedStatus(c) : false;
}

int Server::CreateGame(std::vector<Client *> clients) {
  std::cout << "creating game" << std::endl;
  int game_id = game_id_++;
  games_.push_back(new Game(game_id, clients));
  return game_id;
}

bool Server::RemoveClientFromGame(Client *c, int game_id) {
  auto g = get_game_by_id(game_id);
  return g != nullptr ? g->RemoveClient(c) : false;
}

void Server::Loop() {
  IpAddress ip;
  Socket::Status s;
  lobbies_.push_back(new Lobby(lobby_id_++, "this is a lobby"));
  lobbies_.push_back(new Lobby(lobby_id_++, "another one"));
  lobbies_.push_back(new Lobby(lobby_id_++, "and a third"));
  lobbies_.push_back(new Lobby(lobby_id_++, "and a fourth"));
#ifdef __linux__
  while (!stop_loop) {
#elif defined _WIN32
  while (true) {
#endif
    try {
      // Accept new clients
      if (listener_.HasPendingConnection() == Socket::kOk) {
        TcpSocket *c = new TcpSocket();
        s = listener_.Accept(c, &ip);
        if (s == Socket::kOk) {
          clients_.push_back(new Client(c, ip, client_id_++));
          std::cout << "new connection from : " << ip.toString() << " ("
                    << client_id_ - 1 << ")" << std::endl;
        }
      }
      // Updating clients
      for (auto c : clients_) {
        c->Update();
      }
      CleanDisconnectedClients();
      CleanEmptyLobbies();
      CleanEmptyGames();
// ~ 60 ticks per sec
#ifdef __linux__
      // usleep(16666);
      usleep(166660);
#elif defined _WIN32
      Sleep(160);
#endif
    } catch (SocketException *se) {
      std::cerr << se->what() << std::endl;
      delete se;
    }
  }
}
