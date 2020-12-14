#ifndef ENTITY
#define ENTITY
#include <SFML/Window/Keyboard.hpp>

#include "GuiManager/Messages/GuiManMessages.hpp"

#include <map>
#include <stdint.h>

namespace ent {

constexpr auto MAX_X_MAG = 1920 - 50;
constexpr auto MAX_Y_MAG = 1080 - 50;


class Entity
{
private:
  struct DIRECTIONS_STATE
  {
    sf::Vector2f mDirVec = { 0.0, 0.0 };
    bool LEFT() { return mDirVec.x < 0; }
    bool RIGHT() { return mDirVec.x > 0; }
    bool UP() { return mDirVec.y > 0; }
    bool DOWN() { return mDirVec.y < 0; }
  };

  std::shared_ptr<sf::Shape> mShape;
  DIRECTIONS_STATE mDirection;

  float mMass = 1;


  void updateShape(const sf::Color requestedColor);


  inline void boundsCheck(float &x, float &dir, const float MAX_N_MAG);

public:
  inline const decltype(mShape) getShape() const { return mShape; };

  inline const decltype(mDirection) getDirection() const { return mDirection; };

  void updatePos(const float deltaX, const float deltaY);

  void updateDirection(const float deltaX, const float deltaY);

  void processDirectionsMessage(bool downPress, const sf::Keyboard::Key &key);

  void applyForce(const sf::Vector2f &vel);

  bool autonomous = true;


  Entity(std::shared_ptr<sf::Shape> shape);
};

}// namespace ent

#endif