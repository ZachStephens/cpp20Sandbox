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
    bool LEFT = false;
    bool RIGHT = false;
    bool UP = false;
    bool DOWN = false;
  };

  std::shared_ptr<sf::Shape> mShape;
  DIRECTIONS_STATE mDirection;

  void updateShape(const sf::Vector2f &requestedPos, const sf::Color requestedColor);


  inline void boundsCheck(float &x, const float MAX_N_MAG);

public:
  inline const decltype(mShape) getShape() const { return mShape; };

  inline const decltype(mDirection) getDirection() const { return mDirection; };

  void updatePos(const float deltaX, const float deltaY);

  void processDirectionsMessage(bool downPress, const sf::Keyboard::Key &key);

  Entity(std::shared_ptr<sf::Shape> shape);
};

}// namespace ent

#endif