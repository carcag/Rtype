#include <iostream>
#include "Lobby.h"
#include "Server.h"

Lobby::Lobby(int id, const std::string& name) : id_(id), name_(name),
remove_flag_(false) {
  std::cout << "Created new lobby : " << name << " - " << id << std::endl;
}

Lobby::~Lobby() {
}

bool Lobby::all_clients_ready() {
  for (std::vector<Client *>::iterator it = clients_.begin();
       it != clients_.end(); ++it) {
    if ((*it)->is_ready() == false)
      return false;
  }
  // do not start game if there is not client
  return clients_.size() >= MIN_PLAYER_BEFORE_START;
}

/*
** Lobby's API
*/

bool Lobby::AddClient(Client *c) {
  if (clients_.size() >= MAX_PLAYER_PER_LOBBY)
    return false;
  clients_.push_back(c);
  std::cout << c->get_name() << " joined " << name_ << std::endl;
  for (auto cl : clients_) {
    if (cl != c) {
      std::cout << "notifying: " << cl->get_name() << std::endl;
      cl->SendClientJoinedLobbySig(c->get_id(), c->get_name());
    }
  }
  return true;
}

bool Lobby::RemoveClient(Client *c) {
  std::vector<Client *>::iterator it = clients_.begin();
  while (it != clients_.end()) {
    if ((*it) == c) {
      std::cout << (*it)->get_name() << " left " << name_ << std::endl;
      it = clients_.erase(it);
      if (clients_.size() == 0) {
        remove();
      }
      for (auto cl : clients_) {
        std::cout << "notifying: " << cl->get_name() << std::endl;
        cl->SendClientLeftLobbySig(c->get_id());
      }
      return true;
    } else {
      ++it;
    }
  }
  return false;
}

bool Lobby::ClientChangedStatus(Client *c) {
  std::cout << c->get_name() << " is " << (c->is_ready() ? "ready" :
  "not ready") << std::endl;
  if (all_clients_ready()) {
    Server::get().CreateGame(clients_);
    remove();
  }
  for (auto cl : clients_) {
    if (cl != c) {
      cl->SendReadyUpdateSig(c->get_id(), c->is_ready());
    }
  }
  return true;
}
