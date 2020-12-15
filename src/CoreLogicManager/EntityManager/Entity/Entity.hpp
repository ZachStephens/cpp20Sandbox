#ifndef ENTITY
#define ENTITY
#include <SFML/Window/Keyboard.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>


#include "GuiManager/Messages/GuiManMessages.hpp"

#include <map>
#include <stdint.h>

namespace ent {

constexpr auto MAX_X_MAG = 1920;
constexpr auto MAX_Y_MAG = 1080;


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


  sf::Texture mBackgroundTexture;
  sf::Sprite mBackground;

  float mMass = 1;


  void updateColor(const sf::Color requestedColor);


  inline void boundsCheck(float &x, float &dir, const float MIN, const float MAX);

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