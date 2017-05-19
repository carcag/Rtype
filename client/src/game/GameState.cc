#include "game/GameState.h"
#include "menu/MenuState.h"
#include "lobbies_list/LobbiesListState.h"
#include "core/RessourceManager.h"

GameState::GameState(sf::RenderWindow& window, Network& net_driver,
  Context& ctx) : State(window, net_driver, ctx), send_(0),
  last_fire_(std::chrono::system_clock::now()) {
  // Initializing events
  for (int i = 0; i < sf::Keyboard::KeyCount; ++i)
    curr_events_[i] = false;
  // Online status text
  online_status_text_.SetAll(ctx_.is_online() ? "ONLINE" : "OFFLINE", 14,
    ctx_.is_online() ? sf::Color(95, 120, 230) : sf::Color(255, 75, 75), 2, 1);
  // Change the music
  RessourceManager::get().stop_intro_music();
  RessourceManager::get().play_game_music();
  // Init entities to invalid entities
  for (int i = 0; i < Packet::kMaxEntities; ++i)
    entities_[i] = Entity(0, 0, Entity::kUnknown);
}

GameState::~GameState() {
}

void GameState::Events() {
  sf::Event event;
  int8_t keys[Packet::kMaxKeys] = {0};
  bool goto_menu_state = false;
  while (window_.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      window_.close();
    if (event.type == sf::Event::KeyPressed)
      curr_events_[event.key.code] = true;
    if (event.type == sf::Event::KeyReleased)
      curr_events_[event.key.code] = false;
  }
  // Keyboard
  if (curr_events_[sf::Keyboard::Up]) {
    keys[0] = 1;
  }
  if (curr_events_[sf::Keyboard::Down]) {
    keys[1] = 1;
  }
  if (curr_events_[sf::Keyboard::Left]) {
    keys[2] = 1;
  }
  if (curr_events_[sf::Keyboard::Right]) {
    keys[3] = 1;
  }
  if (curr_events_[sf::Keyboard::Space]) {
    // Only triggering this key if it was not already triggered in the last 1/3s
    if ((last_fire_ + std::chrono::milliseconds(333)) < std::chrono::system_clock::now()) {
      keys[4] = 1;
      last_fire_ = std::chrono::system_clock::now();
    }
  }

  // Network events
  NetEvent ne;
  while (net_driver_.PollNetEvent(ne)) {
    if (ne.type == NetEvent::kStatusResp) {
      printf("status response from server : %d\n", ne.status.ok);
      if (ne.status.ok)
        printf("error : %s\n", ne.status.err_msg);
      // Server close the connection, return to menu
      if (ne.status.ok != 0 && ne.status.cmd == Packet::kInvalid
            && ne.status.err_code == 50) {
        RessourceManager::get().stop_game_music();
        RessourceManager::get().play_intro_music();
        ctx_.set_online(false);
        goto_menu_state = true;
      }
    }
    if (ne.type == NetEvent::kServerPack) {
      int i = 0;
      for (; i < ne.server_pack.entities_nb; ++i) {
        entities_[i] = Entity(ne.server_pack.entities[i].x,
          ne.server_pack.entities[i].y,
          static_cast<Entity::Type>(ne.server_pack.entities[i].type));
      }
      for (; i < Packet::kMaxEntities; ++i) {
        entities_[i] = Entity();
      }
    }
  }
  // Send keyboard state every 5 frames (every ~80ms)
  if (send_ % 2 == 0) {
    net_driver_.SendClientPack(keys);
  }
  send_++;
  if (goto_menu_state)
    StateManager::get().replace(new MenuState(window_, net_driver_, ctx_));
  // Keyboard management
  for (int i = 0; i < sf::Keyboard::KeyCount; ++i)
    prev_events_[i] = curr_events_[i];
}

void GameState::Update() {
  // Server status
  online_status_text_.SetText(ctx_.is_online() ? "ONLINE" : "OFFLINE");
  online_status_text_.SetFillColor(
      ctx_.is_online() ? sf::Color(95, 120, 230) : sf::Color(255, 75, 75));
  sbg_.Update();
}

void GameState::Render() {
  window_.clear();
  sbg_.Render(window_);
  online_status_text_.Render(window_);
  for (auto entity : entities_) {
    if (entity.type() != Entity::kUnknown) {
      entity.Render(window_);
    }
  }
  window_.display();
}
