#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics.hpp>

#include <spdlog/spdlog.h>
#include "Entity.hpp"

namespace ent {


Entity::Entity(std::shared_ptr<sf::Shape> shape) : mShape(shape)
{

  sf::Image image;

  image.loadFromFile("/home/zach/work/basic0/cpp_starter_project/src/GuiManager/lone-bison.png");

  // mBackgroundTexture.loadFromImage(image);
  // auto TextureSize = mBackgroundTexture.getSize();//Get size of texture.
  // auto localBounds = mShape->getLocalBounds();//Get size of window.

  // auto ScaleX = localBounds.width / static_cast<float>(TextureSize.x);
  // auto ScaleY = localBounds.height / static_cast<float>(TextureSize.y);//Calculate scale.

  // mShape->setTexture(&mBackgroundTexture);
  // mShape->setScale(ScaleX, ScaleY);//Set scale.
}


inline void Entity::boundsCheck(float &pos, float &dir, const float MIN, const float MAX)
{

  //width is 1920

  if (pos >= MAX) {
    pos = MAX - (pos - MAX);
    dir *= -1;
  } else if (pos <= MIN) {
    pos = MIN - pos;
    dir *= -1;
  }
}

void Entity::updateColor(const sf::Color requestedColor)
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

  auto localBounds = mShape->getLocalBounds();
  boundsCheck(newXpos, mDirection.mDirVec.x, localBounds.width, MAX_X_MAG - localBounds.width);
  boundsCheck(newYpos, mDirection.mDirVec.y, 0, MAX_Y_MAG - localBounds.height);


  sf::Vector2f requestedPos(newXpos, newYpos);
  spdlog::debug("newX: {}, newY: {}", newXpos, newYpos);
  mShape->setPosition(requestedPos);
  spdlog::set_level(spdlog::level::info);
}

void Entity::applyForce(const sf::Vector2f &vel)
{
  spdlog::set_level(spdlog::level::info);
  spdlog::debug("Entity::applyForce oldX: {}, oldY: {}", mDirection.mDirVec.x, mDirection.mDirVec.y);
  spdlog::debug("Entity::applyForce otherX: {}, otherY: {}", vel.x, vel.y);
  //spdlog::error("newX: {}", mass);

  if ((vel.x > 0 && mDirection.mDirVec.x < 0) || (vel.x < 0 && mDirection.mDirVec.x > 0)) {
    mDirection.mDirVec.x = vel.x;
  }

  if ((vel.y > 0 && mDirection.mDirVec.y < 0) || (vel.y < 0 && mDirection.mDirVec.y > 0)) {
    mDirection.mDirVec.y = vel.y;
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
