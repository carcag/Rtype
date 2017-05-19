#include "lobbies_list/LobbiesListState.h"
#include "create_lobby/CreateLobbyState.h"
#include "menu/MenuState.h"
#include "lobby/LobbyState.h"
#include "core/RessourceManager.h"

LobbiesListState::LobbiesListState(sf::RenderWindow& window, Network& net_driver,
  Context& ctx) : State(window, net_driver, ctx) {
  // Load background image
  bg_sprite_.setTexture(RessourceManager::get().get_menu_bg_texture());
  // Lobbies' list background
  lobbies_list_bg_sprite_.setTexture(RessourceManager::get().get_panel_texture());
  lobbies_list_bg_sprite_.setPosition(sf::Vector2f(150, 80));

  missions_logo_sprite_.setTexture(RessourceManager::get().get_missions_logo_texture());
  missions_logo_sprite_.setPosition(sf::Vector2f(500, 120));
  // Lobbies list
  lobbies_list_.set_origin(sf::Vector2f(220, 220));
  lobbies_list_.set_item_height(80);
  lobbies_list_.set_spacing(20);
  // Buttons
  back_btn_.SetTypeAndPosition(Button::kLobbiesListBack, 25, 90);
  refresh_btn_.SetTypeAndPosition(Button::kLobbiesListRefresh, 50, 90);
  create_btn_.SetTypeAndPosition(Button::kLobbiesListCreate, 75, 90);
  // Online status text
  online_status_text_.SetAll(ctx_.is_online() ? "ONLINE" : "OFFLINE", 14,
    ctx_.is_online() ? sf::Color(95, 120, 230) : sf::Color(255, 75, 75), 2, 1);
  // Player name text
  player_name_text_.SetAll("Commander " + ctx_.get_player_name() +
  "'s missions selection :", 30, sf::Color(95, 120, 230), 50, 3);
  // Get the lobbies list
  net_driver_.SendListLobbiesCmd();
}

LobbiesListState::~LobbiesListState() {
}

void LobbiesListState::Events() {
  sf::Event event;
  bool goto_menu_state = false;
  bool goto_lobby_state = false;
  bool goto_create_lobby_state = false;
  while (window_.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      window_.close();
    // Go back to main menu on Escape
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Escape) {
        goto_menu_state = true;
      }
    }
    // List
    int32_t id = -1;
    if ((id = lobbies_list_.ProcessEvent(event)) != -1)
      net_driver_.SendJoinLobbyCmd(id);
    // Buttons
    if (back_btn_.ProcessEvent(event))
      goto_menu_state = true;
    if (refresh_btn_.ProcessEvent(event))
      net_driver_.SendListLobbiesCmd();
    if (create_btn_.ProcessEvent(event))
      goto_create_lobby_state = true;
    // Keys
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Space) {
        goto_create_lobby_state = true;
      }
    }
  }

  NetEvent ne;
  while (net_driver_.PollNetEvent(ne)) {
    if (ne.type == NetEvent::kStatusResp) {
      // Server close the connection
      if (ne.status.ok != 0 && ne.status.cmd == Packet::kInvalid
            && ne.status.err_code == 50) {
        RessourceManager::get().stop_game_music();
        RessourceManager::get().play_intro_music();
        ctx_.set_online(false);
        goto_menu_state = true;
      }
    } else if (ne.type == NetEvent::kListLobbiesResp) {
      lobbies_list_.clear();
      for (int i = 0; i < ne.lobbies_list.lobbies_nb; ++i) {
        lobbies_list_.add_item(
        std::string(ne.lobbies_list.lobbies[i].name) +
        " (" + std::to_string(ne.lobbies_list.lobbies[i].clients_nb) + "/4)",
        ne.lobbies_list.lobbies[i].id);
      }
    } else if (ne.type == NetEvent::kJoinLobbyResp) {
      printf("joined lobby %s\n", ne.join_lobby.lobby.name);
      for (int i = 0; i < ne.join_lobby.lobby.clients_nb; ++i)
        printf("%s\n", ne.join_lobby.clients[i].name);
      ctx_.set_lobby_name(ne.join_lobby.lobby.name);
      ctx_.set_net_event(ne);
      goto_lobby_state = true;
    }
  }
  if (goto_menu_state)
    StateManager::get().replace(new MenuState(window_, net_driver_, ctx_));
  else if (goto_lobby_state)
    StateManager::get().replace(new LobbyState(window_, net_driver_, ctx_));
  else if (goto_create_lobby_state)
    StateManager::get().replace(new CreateLobbyState(window_, net_driver_, ctx_));
}

void LobbiesListState::Update() {
  // Server status
  online_status_text_.SetText(ctx_.is_online() ? "ONLINE" : "OFFLINE");
  online_status_text_.SetFillColor(
      ctx_.is_online() ? sf::Color(95, 120, 230) : sf::Color(255, 75, 75));
}

void LobbiesListState::Render() {
  window_.clear();
  window_.draw(bg_sprite_);
  window_.draw(lobbies_list_bg_sprite_);
  window_.draw(missions_logo_sprite_);
  online_status_text_.Render(window_);
  player_name_text_.Render(window_);
  back_btn_.Render(window_);
  refresh_btn_.Render(window_);
  create_btn_.Render(window_);
  lobbies_list_.Render(window_);
  window_.display();
}
