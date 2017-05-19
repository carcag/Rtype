#ifndef ENTITY_HPP
# define ENTITY_HPP

class Entity {
 public:
   enum Type {
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

 protected:
   int16_t x_;
   int16_t y_;
   Type type_;

 public:
  Entity() : x_(0), y_(0), type_(kUnknown) {}
  Entity(int16_t x, int16_t y, Type type) : x_(x), y_(y), type_(type) {}
  virtual ~Entity() {}

  int16_t x() { return x_; }
  int16_t y() { return y_; }
  Type type() { return type_; }

  virtual void set_pos(int16_t x, int16_t y) { x_ = x; y_ = y; }

  virtual void Update() {}
};

#endif  // ENTITY_HPP
