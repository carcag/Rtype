#ifndef PROJECTILE_H
# define PROJECTILE_H

# include <algorithm>
# include "Entity.h"

class Projectile : public Entity {
 private:
  uint8_t player_id_;
  uint8_t speed_;
  uint8_t width_;
  uint8_t height_;
  bool to_remove_;

  void remove() { to_remove_ = true; }

 public:
  Projectile() = delete;
  Projectile(int16_t x, int16_t y, uint8_t player_id) :
    Entity(x, y, static_cast<Entity::Type>(kProjectile1 + player_id)),
    speed_(20), player_id_(player_id), width_(20), height_(20), to_remove_(false) {}
  ~Projectile() {}

  int16_t x() { return x_; }
  int16_t y() { return y_; }
  Type type() { return type_; }
  uint8_t player_id() { return player_id_; }
  bool to_remove() { return to_remove_; }

  void Update() {
    if (x_ > 1600) {
      remove();
    } else {
      x_ += speed_;
    }
  }
};

#endif  // PROJECTILE_H
