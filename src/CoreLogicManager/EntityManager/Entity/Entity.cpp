#include <SFML/Graphics/CircleShape.hpp>
#include <spdlog/spdlog.h>
#include "Entity.hpp"

namespace ent {


Entity::Entity(std::shared_ptr<sf::Shape> shape) : mShape(shape)
{
}


inline void Entity::boundsCheck(float &pos, float &dir, const float MAX_N_MAG)
{
  if (pos >= MAX_N_MAG) {
    pos = MAX_N_MAG - (pos - MAX_N_MAG);
    dir *= -1;
  } else if (pos <= 0) {
    pos = 0 - pos;
    dir *= -1;
  }
}

void Entity::updateShape(const sf::Color requestedColor)
{
  mShape->setFillColor(requestedColor);
}


void Entity::updateDirection(const float deltaX, const float deltaY)
{
  spdlog::set_level(spdlog::level::info);
  mDirection.mDirVec.x += deltaX;
  mDirection.mDirVec.y += deltaY;
  spdlog::debug("newX: {}, newY: {}", mDirection.mDirVec.x, mDirection.mDirVec.y);
  spdlog::set_level(spdlog::level::info);
}

void Entity::updatePos(const float deltaX, const float deltaY)
{
  spdlog::set_level(spdlog::level::info);
  auto pos = mShape->getPosition();

  auto newXpos = pos.x + deltaX;
  auto newYpos = pos.y + deltaY;

  boundsCheck(newXpos, mDirection.mDirVec.x, MAX_X_MAG);
  boundsCheck(newYpos, mDirection.mDirVec.y, MAX_Y_MAG);


  sf::Vector2f requestedPos(newXpos, newYpos);
  spdlog::debug("newX: {}, newY: {}", newXpos, newYpos);
  mShape->setPosition(requestedPos);
  spdlog::set_level(spdlog::level::info);
}

void Entity::applyForce(const sf::Vector2f &vel)
{
  spdlog::set_level(spdlog::level::debug);
  spdlog::debug("Entity::applyForce oldX: {}, oldY: {}", mDirection.mDirVec.x, mDirection.mDirVec.y);
  spdlog::debug("Entity::applyForce otherX: {}, otherY: {}", vel.x, vel.y);
  //spdlog::error("newX: {}", mass);
  if (!((vel.x > 0 && mDirection.mDirVec.x > 0) || (vel.x < 0 && mDirection.mDirVec.x < 0))) {
    mDirection.mDirVec.x = -((vel.x + mDirection.mDirVec.x) / 2);
  }

  if (!((vel.y > 0 && mDirection.mDirVec.y > 0) || (vel.y < 0 && mDirection.mDirVec.y < 0))) {
    mDirection.mDirVec.y = -(vel.y + mDirection.mDirVec.y) / 2;
  }

  spdlog::debug("Entity::applyForce newX: {}, newY: {}", mDirection.mDirVec.x, mDirection.mDirVec.y);
  spdlog::set_level(spdlog::level::info);
}

void Entity::processDirectionsMessage(bool downPress, const sf::Keyboard::Key &key)
{

  spdlog::set_level(spdlog::level::info);

  if (autonomous)
    return;

  switch (key) {
  case sf::Keyboard::Left:
  case sf::Keyboard::A:
    spdlog::debug("processDirectionsMessage: LEFT {}", mDirection.LEFT());
    mDirection.mDirVec.x = (downPress) ? -1 : 0;
    spdlog::debug("processDirectionsMessage: LEFT {}", mDirection.LEFT());
    break;
  case sf::Keyboard::Right:
  case sf::Keyboard::D:
    spdlog::debug("processDirectionsMessage: RIGHT {}", mDirection.RIGHT());
    mDirection.mDirVec.x = (downPress) ? 1 : 0;
    spdlog::debug("processDirectionsMessage: RIGHT {}", mDirection.RIGHT());
    break;
  case sf::Keyboard::Up:
  case sf::Keyboard::W:
    spdlog::debug("processDirectionsMessage: UP {}", mDirection.UP());
    mDirection.mDirVec.y = (downPress) ? -1 : 0;
    spdlog::debug("processDirectionsMessage: UP {}", mDirection.UP());
    break;
  case sf::Keyboard::Down:
  case sf::Keyboard::S:
    spdlog::debug("processDirectionsMessage: DOWN {}", mDirection.DOWN());
    mDirection.mDirVec.y = (downPress) ? 1 : 0;
    spdlog::debug("processDirectionsMessage: DOWN {}", mDirection.DOWN());
    break;
  default:
    break;
  }

  spdlog::set_level(spdlog::level::info);
}
}// namespace ent
