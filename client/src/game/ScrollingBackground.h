#ifndef GAME_SCROLLING_BACKGROUND_H
# define GAME_SCROLLING_BACKGROUND_H

# include <SFML/Graphics.hpp>

class ScrollingBackground {
 private:
  sf::Texture scroll_bg_texture_;
  sf::Sprite scroll_bg_sprite1_;
  sf::Sprite scroll_bg_sprite2_;

  double speed_;

 public:
  ScrollingBackground();
  ScrollingBackground(const ScrollingBackground&) = delete;
  ~ScrollingBackground();
  ScrollingBackground operator=(const ScrollingBackground&) = delete;

  void Update();
  void Render(sf::RenderWindow& window) const;
};

#endif  // GAME_SCROLLING_BACKGROUND_H
