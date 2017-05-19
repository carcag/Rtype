#include "Game.h"
#include "ClientEvent.h"
#include "Entity.h"
#include "Projectile.h"
#include "Spaceship.h"
#include <chrono>

Game::Game(int id, std::vector<Client *> clients)
    : id_(id), clients_(clients), remove_flag_(false), send_(true),
      base_port_(2020 + (id * 4)) {
  int i = 0;
  for (auto c : clients_) {
    c->set_location(Client::kInGame);
    c->set_location_id(id_);
    auto udp_sock = new UdpSocket();
    udp_sock->Bind(base_port_ + i);
    c->set_udp_socket(udp_sock);
    c->set_player_id(i);
    entities_.push_back(new Spaceship(100, (i + 1) * 150, i));
    c->SendGameStartedSig(base_port_ + i, i);
    ++i;
  }
  base_port_ += i;
  std::cout << "base_port_: " << base_port_ << '\n';
  thread_ = new std::thread(&Game::Loop, this);
  game_running_ = true;
  std::cout << "started game" << std::endl;
}

Game::~Game() {
  game_running_ = false;
  for (auto e : entities_) {
    delete e;
  }
  entities_.clear();
  thread_->join();
  delete thread_;
  std::cout << "stopped game" << std::endl;
}

Spaceship *Game::spacehip_by_player_id(uint8_t player_id) {
  for (auto entity : entities_) {
    if (auto spaceship = dynamic_cast<Spaceship *>(entity)) {
      if (spaceship->id() == player_id)
        return spaceship;
    }
  }
  return nullptr;
}

/*
** Game's API
*/

void Game::Loop() {
  while (clients_.size() && game_running_) {
    std::chrono::steady_clock::time_point begin =
        std::chrono::steady_clock::now();
    entities_mutex_.lock();
    for (auto c : clients_) {
      ClientEvent ce;
      if (c->PollEvents(ce)) {
        Spaceship *s = spacehip_by_player_id(c->get_player_id());
        if (!s)
          continue;
        if (ce.type == ClientEvent::kKeysPressed) {
          if (ce.keys.pressed[0] == 1)
            s->MoveUp();
          if (ce.keys.pressed[1] == 1)
            s->MoveDown();
          if (ce.keys.pressed[2] == 1)
            s->MoveLeft();
          if (ce.keys.pressed[3] == 1)
            s->MoveRight();
          if (ce.keys.pressed[4])
            entities_.push_back(
                new Projectile(s->x(), s->y(), c->get_player_id()));
        }
      }
    }
    for (auto e : entities_) {
      e->Update();
    }
    RemoveProjectiles();
    if (send_) {
      for (auto c : clients_) {
        c->SendServerPack(entities_);
      }
      send_ = false;
    } else {
      send_ = true;
    }
    entities_mutex_.unlock();
// ~ 60 ticks per sec
#ifdef __linux__
    usleep(16666);
#elif defined _WIN32
    Sleep(160);
#endif
    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();
    // std::cout << "Time difference = " <<
    // std::chrono::duration_cast<std::chrono::nanoseconds> (end -
    // begin).count() <<std::endl;
  }
}

void Game::RemoveProjectiles() {
  auto it = entities_.begin();
  while (it != entities_.end()) {
    if (auto proj = dynamic_cast<Projectile *>(*it)) {
      if (proj->to_remove()) {
        delete *it;
        entities_.erase(it);
      } else {
        ++it;
      }
    } else {
      ++it;
    }
  }
}

bool Game::RemoveClient(Client *c) {
  std::vector<Client *>::iterator it = clients_.begin();
  while (it != clients_.end()) {
    if ((*it) == c) {
      // Removing the client's spaceship
      entities_mutex_.lock();
      for (auto it = entities_.begin(); it != entities_.end(); ++it) {
        if (auto spaceship = dynamic_cast<Spaceship *>(*it)) {
          if (spaceship->id() == c->get_player_id()) {
            it = entities_.erase(it);
            break;
          }
        }
      }
      entities_mutex_.unlock();
      std::cout << (*it)->get_name() << " left the game " << id_ << std::endl;
      clients_.erase(it);
      if (clients_.size() == 0) {
        remove();
      }
      for (auto cl : clients_) {
        cl->SendClientDisconSig((*it)->get_id());
      }
      return true;
    } else {
      ++it;
    }
  }
  return false;
}
