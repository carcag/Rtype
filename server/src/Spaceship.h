#ifndef SPACESHIP_H
# define SPACESHIP_H

# include <algorithm>
# include "Entity.h"

class Spaceship : public Entity {
 private:
  uint8_t id_;
  uint8_t speed_;
  uint8_t width_;
  uint8_t height_;

 public:
  Spaceship() = delete;
  Spaceship(int16_t x, int16_t y, uint8_t id) :
    Entity(x, y, static_cast<Entity::Type>(kPlayer1 + id)),
    speed_(10), id_(id), width_(225), height_(90) {}
  ~Spaceship() {}

  int16_t x() { return x_; }
  int16_t y() { return y_; }
  Type type() { return type_; }
  uint8_t id() { return id_; }

  void set_pos(int16_t x, int16_t y) { x_ = x; y_ = y; }

  void Update() {}
  void MoveUp() { y_ = std::max(y_ - speed_, 0); }
  void MoveDown() {
    y_ = std::min(y_ + speed_, 900 - height_);
  }
  void MoveLeft() { x_ = std::max(x_ - speed_, 0); }
  void MoveRight() {
    x_ = std::min(x_ + speed_, 1600 - width_);
  }
};

#endif  // SPACESHIP_H
