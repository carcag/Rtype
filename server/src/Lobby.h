#ifndef LOBBY_H
# define LOBBY_H

# include <string>
# include <vector>

# include "Client.h"

# define MAX_PLAYER_PER_LOBBY (4)
# define MIN_PLAYER_BEFORE_START (1)

class Lobby {
 private:
  int id_;
  const std::string name_;
  std::vector<Client *> clients_;
  // If set to true, will be removed on the next loop tick
  bool remove_flag_;

  void remove() { remove_flag_ = true; }

  bool all_clients_ready();

 public:
  Lobby() = delete;
  Lobby(int id, const std::string& name);
  ~Lobby();
  Lobby(const Lobby&) = delete;
  Lobby operator=(const Lobby&) = delete;

  int get_id() const { return id_; }
  const std::string& get_name() const { return name_; }
  std::size_t get_clients_nb() const { return clients_.size(); }
  const std::vector<Client *>& get_clients() const { return clients_; }

  bool to_remove() const { return remove_flag_; }
  // Lobby's API
  bool AddClient(Client *c);
  bool RemoveClient(Client *c);
  bool ClientChangedStatus(Client *c);
};

#endif  // LOBBY_H
