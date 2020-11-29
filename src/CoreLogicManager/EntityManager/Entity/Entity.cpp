#include <SFML/Graphics/CircleShape.hpp>
#include <spdlog/spdlog.h>
#include "Entity.hpp"

namespace ent {


Entity::Entity(std::shared_ptr<sf::Shape> shape) : mShape(shape)
{
}


inline void Entity::boundsCheck(float &x, const float MAX_N_MAG)
{
  if (x >= MAX_N_MAG) {
    x = MAX_N_MAG;
  } else if (x <= 0) {
    x = 0;
  }
}

void Entity::updateShape(const sf::Vector2f &requestedPos, const sf::Color requestedColor)
{
  mShape->setPosition(requestedPos);
  mShape->setFillColor(requestedColor);
}

void Entity::updatePos(const float deltaX, const float deltaY)
{
  auto pos = mShape->getPosition();

  auto newXpos = pos.x + deltaX;
  auto newYpos = pos.y + deltaY;

  boundsCheck(newXpos, MAX_X_MAG);
  boundsCheck(newYpos, MAX_Y_MAG);


  sf::Vector2f requestedPos(newXpos, newYpos);

  auto requestedColor = sf::Color::Green;

  updateShape(requestedPos, requestedColor);
}


void Entity::processDirectionsMessage(bool downPress, const sf::Keyboard::Key &key)
{

  //spdlog::set_level(spdlog::level::info);

  switch (key) {
  case sf::Keyboard::Left:
  case sf::Keyboard::A:
    spdlog::debug("processDirectionsMessage: LEFT {}", mDirection.LEFT);
    mDirection.LEFT = (downPress) ? true : false;
    spdlog::debug("processDirectionsMessage: LEFT {}", mDirection.LEFT);
    break;
  case sf::Keyboard::Right:
  case sf::Keyboard::D:
    spdlog::debug("processDirectionsMessage: RIGHT {}", mDirection.RIGHT);
    mDirection.RIGHT = (downPress) ? true : false;
    spdlog::debug("processDirectionsMessage: RIGHT {}", mDirection.RIGHT);
    break;
  case sf::Keyboard::Up:
  case sf::Keyboard::W:
    spdlog::debug("processDirectionsMessage: UP {}", mDirection.UP);
    mDirection.UP = (downPress) ? true : false;
    spdlog::debug("processDirectionsMessage: UP {}", mDirection.UP);
    break;
  case sf::Keyboard::Down:
  case sf::Keyboard::S:
    spdlog::debug("processDirectionsMessage: DOWN {}", mDirection.DOWN);
    mDirection.DOWN = (downPress) ? true : false;
    spdlog::debug("processDirectionsMessage: DOWN {}", mDirection.DOWN);
    break;
  default:
    break;
  }

  //spdlog::set_level(spdlog::level::info);
}
}// namespace ent
