#ifndef BUTTONS_TEXT_FIELD_H
# define BUTTONS_TEXT_FIELD_H

# include <SFML/Graphics.hpp>

class TextField {
 protected:
  sf::Text text_;

 public:
  TextField();
  ~TextField();
  TextField operator=(const TextField&) = delete;

  void SetText(const std::string& text);
  void SetSize(std::size_t fsize);
  void SetFillColor(const sf::Color& color);
  void SetPosition(const sf::Vector2f& pos, bool flag = true);
  void SetPosition(uint8_t wpercent, uint8_t hpercent, bool flag = true);
  void SetAll(const std::string& text,
              std::size_t fsize,
              const sf::Color& color,
              uint8_t wpercent,
              uint8_t hpercent,
              bool flag = true);
  sf::FloatRect getGlobalBounds();
  void Render(sf::RenderWindow& window);
};

#endif  // BUTTONS_TEXT_FIELD_H
