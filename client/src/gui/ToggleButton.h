#ifndef GUI_TOGGLE_BUTTON_H
# define GUI_TOGGLE_BUTTON_H

# include <SFML/Graphics.hpp>

class ToggleButton {
 public:
  enum Type {
    // Lobby buttons
    kLobbyReady
  };

 private:
  sf::Texture texture_off_;
  sf::Texture texture_on_;
  sf::Texture hover_texture_;
  sf::Sprite sprite_;
  bool on_;
  bool inside_btn_; // for hover sound

 public:
  ToggleButton();
  ToggleButton(const ToggleButton&) = delete;
  ~ToggleButton();
  ToggleButton operator=(const ToggleButton&) = delete;

  bool get_state() { return on_; }

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

#endif  // GUI_TOGGLE_BUTTON_H
