#ifndef GAME_ENTITY_HPP
# define GAME_ENTITY_HPP

# include <SFML/Graphics.hpp>
# include "core/RessourceManager.h"

class Entity {
 public:
   enum Type : int {
     kPlayer1 = 0,
     kPlayer2,
     kPlayer3,
     kPlayer4,
     kProjectile1,
     kProjectile2,
     kProjectile3,
     kProjectile4,
     kEnnemy1,
     kEnnemy2,
     kBoss,
     kWall,
     kUnknown
   };

 private:
   int16_t x_;
   int16_t y_;
   Type type_;
   sf::Sprite sprite_;

 public:
  Entity() : x_(0), y_(0), type_(kUnknown) {}
  Entity(int16_t x, int16_t y, Type type) : x_(x), y_(y), type_(type) {
    switch (type_) {
      case kPlayer1:
        sprite_.setTexture(RessourceManager::get().get_spaceship1_texture());
        break;
      case kPlayer2:
        sprite_.setTexture(RessourceManager::get().get_spaceship2_texture());
        break;
      case kPlayer3:
        sprite_.setTexture(RessourceManager::get().get_spaceship3_texture());
        break;
      case kPlayer4:
        sprite_.setTexture(RessourceManager::get().get_spaceship4_texture());
        break;
      case kProjectile1:
        sprite_.setTexture(RessourceManager::get().get_pew1_texture());
        break;
      case kProjectile2:
        sprite_.setTexture(RessourceManager::get().get_pew2_texture());
        break;
      case kProjectile3:
        sprite_.setTexture(RessourceManager::get().get_pew3_texture());
        break;
      case kProjectile4:
        sprite_.setTexture(RessourceManager::get().get_pew4_texture());
        break;
    }
    sprite_.setPosition(x_, y_);
  }
  ~Entity() {}

  int16_t x() { return x_; }
  int16_t y() { return y_; }
  Type type() { return type_; }

  void set_pos(int16_t x, int16_t y) { x_ = x; y_ = y; }

  void Render(sf::RenderWindow& window) { window.draw(sprite_); }
};

#endif  // GAME_ENTITY_HPP
