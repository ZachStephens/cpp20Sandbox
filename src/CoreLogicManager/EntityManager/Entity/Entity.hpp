#ifndef ENTITY
#define ENTITY

#include "IEntity.hpp"
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


template<typename SHAPE_T, typename VECT_T>
class Entity : public IEntity<SHAPE_T, VECT_T>
{
private:
  float mMass = 1;

  std::shared_ptr<SHAPE_T> mShape;


  void updateColor(const sf::Color requestedColor)
  {
    mShape->setFillColor(requestedColor);
  }

  inline void boundsCheck(float &pos, float &dir, const float MIN, const float MAX)
  {

    //width is 1920
    spdlog::set_level(spdlog::level::info);
    if (pos >= MAX) {
      //pos = MAX - (pos - MAX);
      //make dir negative

      auto curPos = mShape->getPosition();
      spdlog::debug("posX: {}, posY: {}", curPos.x, curPos.y);
      spdlog::debug("dirX: {}, dirY: {}", mVelocity.x, mVelocity.y);
      dir = (dir > 0) ? dir * -1 : dir;
      spdlog::debug("dirX: {}, dirY: {}", mVelocity.x, mVelocity.y);
    } else if (pos <= MIN) {
      //pos = MIN - pos;
      //make dir positive
      auto curPos = mShape->getPosition();
      spdlog::debug("posX: {}, posY: {}", curPos.x, curPos.y);
      spdlog::debug("dirX: {}, dirY: {}", mVelocity.x, mVelocity.y);
      dir = (dir < 0) ? dir * -1 : dir;
      spdlog::debug("dirX: {}, dirY: {}", mVelocity.x, mVelocity.y);
    }
    spdlog::set_level(spdlog::level::info);
  }


public:
  VECT_T mPendingVelocity;
  VECT_T mVelocity;
  bool autonomous = true;

  inline const std::shared_ptr<SHAPE_T> getShape() const override { return mShape; };


  void autoInit()
  {
  }

  const VECT_T getCenterPosition() const override
  {
    const auto &bounds = mShape->getGlobalBounds();
    return VECT_T(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
  }

  inline const decltype(mVelocity) getVelocity() const override { return mVelocity; };

  void setTexture(const sf::Texture &texture)
  {
    // spdlog::set_level(spdlog::level::info);
    // spdlog::info("settingImage");

    mShape->setTexture(&texture);
    // mShape->setScale(ScaleX, ScaleY);//Set scale.
  }

  void updatePos() override
  {
    auto pos = mShape->getPosition();

    auto newXpos = pos.x + mVelocity.x;
    auto newYpos = pos.y + mVelocity.y;

    auto localBounds = mShape->getLocalBounds();
    boundsCheck(newXpos, mVelocity.x, 0, MAX_X_MAG - localBounds.width);
    boundsCheck(newYpos, mVelocity.y, 0, MAX_Y_MAG - localBounds.height);


    VECT_T requestedPos(newXpos, newYpos);
    mShape->setPosition(requestedPos);
  }

  void updateVelocity() override
  {
    mVelocity = mPendingVelocity;
    // mPendingVelocity = {};
  }

  void processVelocityMessage(bool downPress, const sf::Keyboard::Key &key) override
  {

    spdlog::set_level(spdlog::level::info);

    if (autonomous)
      return;
    switch (key) {
    case sf::Keyboard::Left:
    case sf::Keyboard::A:
      mVelocity.x = (downPress) ? -1 : 0;
      break;
    case sf::Keyboard::Right:
    case sf::Keyboard::D:
      mVelocity.x = (downPress) ? 1 : 0;
      break;
    case sf::Keyboard::Up:
    case sf::Keyboard::W:
      mVelocity.y = (downPress) ? -1 : 0;
      break;
    case sf::Keyboard::Down:
    case sf::Keyboard::S:
      mVelocity.y = (downPress) ? 1 : 0;
      break;
    default:
      break;
    }

    spdlog::set_level(spdlog::level::info);
  }

  bool intersects(const IEntity<SHAPE_T, VECT_T> &otherEntity) const override
  {
    const auto boundary = mShape->getGlobalBounds();
    const auto otherBoundary = otherEntity.getShape()->getGlobalBounds();

    return (boundary.intersects(otherBoundary));
  }

  Entity(std::shared_ptr<SHAPE_T> shape, VECT_T initVel) : mShape(shape)
  {
    mPendingVelocity = initVel;
    updateVelocity();
  }
};


}// namespace ent

#endif