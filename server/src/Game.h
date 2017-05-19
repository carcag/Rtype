#ifndef GAME_H
#define GAME_H

#include <mutex>
#include <thread>
#include <vector>
#ifdef __linux__
#include <unistd.h>
#elif defined _WIN32
#define _WINSOCKAPI_
#include <windows.h>
#endif

#include "Client.h"
#include "Entity.h"

class Game {
private:
  int id_;
  std::vector<Client *> clients_;
  std::mutex entities_mutex_;
  std::vector<Entity *> entities_;
  // If set to true, will be removed on the next loop tick
  bool remove_flag_;
  std::thread *thread_;
  uint16_t base_port_;
  bool game_running_;

  bool send_;

  void remove() { remove_flag_ = true; }
  void RemoveProjectiles();

public:
  Game() = delete;
  Game(int id, std::vector<Client *> clients);
  ~Game();
  Game(const Game &) = delete;
  Game operator=(const Game &) = delete;

  int get_id() const { return id_; }
  std::size_t get_clients_nb() const { return clients_.size(); }

  bool to_remove() const { return remove_flag_; }

  Spaceship *spacehip_by_player_id(uint8_t player_id);
  // Game's API
  void Loop();
  bool RemoveClient(Client *c);
};

#endif // GAME_H
