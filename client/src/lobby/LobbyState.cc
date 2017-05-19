#include "lobby/LobbyState.h"
#include "menu/MenuState.h"
#include "lobbies_list/LobbiesListState.h"
#include "game/GameState.h"
#include "core/RessourceManager.h"

LobbyState::LobbyState(sf::RenderWindow& window, Network& net_driver,
  Context& ctx) : State(window, net_driver, ctx) {
  // Load background image
  bg_sprite_.setTexture(RessourceManager::get().get_menu_bg_texture());
  // Lobby background
  lobby_bg_sprite_.setTexture(RessourceManager::get().get_panel_texture());
  lobby_bg_sprite_.setPosition(sf::Vector2f(150, 80));
  // Players list
  players_list_.set_origin(sf::Vector2f(800, 250));
  players_list_.set_item_height(80);
  players_list_.set_spacing(20);
  players_list_.set_items_centered(true);
  // Buttons
  leave_btn_.SetTypeAndPosition(Button::kLobbyLeave, 25, 90);
  ready_btn_.SetTypeAndPosition(ToggleButton::kLobbyReady, 75, 90);
  // Online status text
  online_status_text_.SetAll(ctx_.is_online() ? "ONLINE" : "OFFLINE", 14,
    ctx_.is_online() ? sf::Color(95, 120, 230) : sf::Color(255, 75, 75), 2, 1);
  // Player name text
  player_name_text_.SetAll("Lobby : " + ctx_.get_lobby_name(),
    30, sf::Color(95, 120, 230), 50, 3);
  // Fill the players' list
  auto ne = ctx_.get_net_event();
  for (int i = 0; i < ne.join_lobby.lobby.clients_nb; i++)
    players_list_.add_item(ne.join_lobby.clients[i].name,
    ne.join_lobby.clients[i].id);
}

LobbyState::~LobbyState() {
}

void LobbyState::Events() {
  sf::Event event;
  bool goto_menu_state = false;
  bool goto_lobbieslist_state = false;
  bool goto_game_state = false;
  while (window_.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      window_.close();
    // Buttons
    if (leave_btn_.ProcessEvent(event)) {
      net_driver_.SendLeaveLobbyCmd();
    }
    if (ready_btn_.ProcessEvent(event)) {
      net_driver_.SendSetReadyCmd(ready_btn_.get_state());
    }
    // Keys
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Escape) {
        net_driver_.SendLeaveLobbyCmd();
      }
    }
    // Player list (animate, no events)
    players_list_.ProcessEvent(event);
  }

  NetEvent ne;
  while (net_driver_.PollNetEvent(ne)) {
    if (ne.type == NetEvent::kStatusResp) {
      printf("status response from server : %d\n", ne.status.ok);
      if (ne.status.ok)
        printf("status response from server : %s\n", ne.status.err_msg);
      if (ne.status.cmd == Packet::kLeaveLobbyCmd && ne.status.ok == 0)
        goto_lobbieslist_state = true;
      if (ne.status.cmd == Packet::kSetReadyCmd && ne.status.ok == 0)
        printf("sucessfully switched state\n");
        // Server close the connection, return to menu
      if (ne.status.ok != 0 && ne.status.cmd == Packet::kInvalid
            && ne.status.err_code == 50) {
        RessourceManager::get().stop_game_music();
        RessourceManager::get().play_intro_music();
        ctx_.set_online(false);
        goto_menu_state = true;
      }
    } else if (ne.type == NetEvent::kClientJoinedLobbySig) {
      printf("client %d joined this lobby\n", ne.client_left.client_id);
      players_list_.add_item(ne.client_joined.client_name,
        ne.client_joined.client_id);
    } else if (ne.type == NetEvent::kClientLeftLobbySig) {
      printf("client %d left this lobby\n", ne.client_left.client_id);
      players_list_.remove_item(ne.client_left.client_id);
    } else if (ne.type == NetEvent::kReadyUpdateSig) {
      printf("player %d changed his status to %d\n", ne.ready_update.client_id,
        ne.ready_update.ready);
    } else if (ne.type == NetEvent::kGameStartedSig) {
      printf("game is starting!\nyou are player %d\n",
        ne.game_started.player_nb);
      goto_game_state = true;
    }
  }

  if (goto_lobbieslist_state)
    StateManager::get().replace(new LobbiesListState(window_, net_driver_, ctx_));
  else if (goto_menu_state)
    StateManager::get().replace(new MenuState(window_, net_driver_, ctx_));
  else if (goto_game_state)
    StateManager::get().replace(new GameState(window_, net_driver_, ctx_));
}

void LobbyState::Update() {
  // Server status
  online_status_text_.SetText(ctx_.is_online() ? "ONLINE" : "OFFLINE");
  online_status_text_.SetFillColor(
      ctx_.is_online() ? sf::Color(95, 120, 230) : sf::Color(255, 75, 75));
}

void LobbyState::Render() {
  window_.clear();
  window_.draw(bg_sprite_);
  window_.draw(lobby_bg_sprite_);
  players_list_.Render(window_);
  online_status_text_.Render(window_);
  player_name_text_.Render(window_);
  leave_btn_.Render(window_);
  ready_btn_.Render(window_);
  window_.display();
}
