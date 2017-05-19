#include "create_lobby/CreateLobbyState.h"
#include "lobbies_list/LobbiesListState.h"
#include "menu/MenuState.h"
#include "core/RessourceManager.h"

CreateLobbyState::CreateLobbyState(sf::RenderWindow& window, Network& net_driver,
  Context& ctx) : State(window, net_driver, ctx), goto_menu_state_(false) {
  // Load background image
  bg_sprite_.setTexture(RessourceManager::get().get_menu_bg_texture());
  // Line
  line_.setSize(sf::Vector2f(600, 1));
  line_.setPosition(sf::Vector2f(500, 220));
  line_.setFillColor(sf::Color(215, 213, 244, 175));
  // Connect text
  create_lobby_text_.SetAll("Enter a lobby name :",
    38, sf::Color(95, 120, 230), 50, 10);
  // Buttons
  back_btn_.SetTypeAndPosition(Button::kLobbiesListBack, 25, 90);
  create_btn_.SetTypeAndPosition(Button::kLobbiesListCreate, 75, 90);
  // Online status text
  online_status_text_.SetAll(ctx_.is_online() ? "ONLINE" : "OFFLINE", 14,
    ctx_.is_online() ? sf::Color(95, 120, 230) : sf::Color(255, 75, 75), 2, 1);
  // Player's name text
  lobby_name_text_.SetAll("", 50, sf::Color(95, 120, 230), 50, 20);
}

CreateLobbyState::~CreateLobbyState() {
}

bool CreateLobbyState::process_text_entered(sf::Event& event) {
  if (event.text.unicode == 8) {
    RessourceManager::get().play_key_pressed3();
    lobby_name_ = lobby_name_.substring(0, lobby_name_.getSize() - 1);
  } else if (event.text.unicode == 13) {
    RessourceManager::get().play_key_pressed2();
    printf("%s\n", lobby_name_.toAnsiString().c_str());
    net_driver_.SendCreateLobbyCmd(lobby_name_.toAnsiString());
    return true;
  } else if (filter_displayable(event.text.unicode) && lobby_name_.getSize() < 16) {
    RessourceManager::get().play_key_pressed1();
    lobby_name_ += static_cast<char>(event.text.unicode);
  }
  return false;
}

void CreateLobbyState::Events() {
  sf::Event event;
  bool goto_menu_state = false;
  bool goto_lobbies_list_state = false;

  while (window_.pollEvent(event)) {
    // Closing window
    if (event.type == sf::Event::Closed)
      window_.close();
    // Go back to lobbies list on Escape
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Escape) {
        goto_lobbies_list_state = true;
      }
    }
    // Text input
    if (event.type == sf::Event::TextEntered)
      if (process_text_entered(event))
        goto_lobbies_list_state = true;
    // Buttons
    if (back_btn_.ProcessEvent(event))
      goto_lobbies_list_state = true;
    if (create_btn_.ProcessEvent(event)) {
      printf("%s\n", lobby_name_.toAnsiString().c_str());
      net_driver_.SendCreateLobbyCmd(lobby_name_.toAnsiString());
      goto_lobbies_list_state = true;
    }
  }
  NetEvent ne;
  while (net_driver_.PollNetEvent(ne)) {
    if (ne.type == NetEvent::kStatusResp) {
      // Server close the connection
      if (ne.status.ok != 0 && ne.status.cmd == Packet::kInvalid
            && ne.status.err_code == 50) {
        ctx_.set_online(false);
        goto_menu_state = true;
      }
    }
  }
  if (goto_menu_state_)
    StateManager::get().replace(new MenuState(window_, net_driver_, ctx_));
  else if (goto_lobbies_list_state)
    StateManager::get().replace(new LobbiesListState(window_, net_driver_, ctx_));
  else if (goto_menu_state)
    StateManager::get().replace(new MenuState(window_, net_driver_, ctx_));
}

void CreateLobbyState::Update() {
  lobby_name_text_.SetText(lobby_name_);
  lobby_name_text_.SetPosition(50, 20);
}

void CreateLobbyState::Render() {
  window_.clear();
  window_.draw(bg_sprite_);
  window_.draw(line_);
  online_status_text_.Render(window_);
  back_btn_.Render(window_);
  create_btn_.Render(window_);
  create_lobby_text_.Render(window_);
  lobby_name_text_.Render(window_);
  window_.display();
}
