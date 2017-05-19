#ifndef GUI_BUTTON_H
# define GUI_BUTTON_H

# include <SFML/Graphics.hpp>

class Button {
 public:
  enum Type {
    // Menu buttons
    kMenuOptions,
    kMenuLaunch,
    kMenuAbort,
    // Lobbies list buttons
    kLobbiesListBack,
    kLobbiesListRefresh,
    kLobbiesListCreate,
    // Lobby buttons
    kLobbyLeave,
    kLobbyReady
  };

 private:
  sf::Texture texture_;
  sf::Texture hover_texture_;
  sf::Sprite sprite_;
  bool inside_btn_; // for hover sound

 public:
  Button();
  Button(const Button&) = delete;
  ~Button();
  Button operator=(const Button&) = delete;

  void SetType(Type type);
  void SetPosition(const sf::Vector2f& pos);
  void SetPosition(uint8_t wpercent, uint8_t hpercent, bool flag = true);
  void SetTypeAndPosition(Type type, const sf::Vector2f& pos);
  void SetTypeAndPosition(Type type,
                          uint8_t wpercent,
                          uint8_t hpercent,
                          bool flag = true);
  sf::FloatRect getGlobalBounds();
  // Returns true if the button was pressed, and takes care of hover fx
  bool ProcessEvent(const sf::Event& event);
  void Render(sf::RenderWindow& window);
};

#endif  // GUI_BUTTON_H
