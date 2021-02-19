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

namespace ent::base {

constexpr auto MAX_X_MAG = 1920;
constexpr auto MAX_Y_MAG = 1080;


template<typename SHAPE_T, typename VECT_T>
class Entity : public IEntity<SHAPE_T, VECT_T>
{
private:
  void updateColor(const sf::Color requestedColor)
  {
    mShape->setFillColor(requestedColor);
  }

  float mMass = 1;

protected:
  const sf::FloatRect mTopBounds, mBottomBounds, mLeftBounds, mRightBounds;
  std::shared_ptr<SHAPE_T> mShape;

  void initTopBounds() {}
  void initBottomBounds() {}
  void initLeftBounds() {}
  void initRightBounds() {}

public:
  VECT_T mPendingVelocity;

  VECT_T mVelocity;

  bool autonomous = true;

  inline const std::shared_ptr<SHAPE_T> getShape() const override { return mShape; };


  [[nodiscard]] const sf::FloatRect &getTopBounds() const override { return mTopBounds; }

  [[nodiscard]] const sf::FloatRect &getBottomBounds() const override { return mBottomBounds; }

  [[nodiscard]] const sf::FloatRect &getLeftBounds() const override { return mLeftBounds; }

  [[nodiscard]] const sf::FloatRect &getRightBounds() const override { return mRightBounds; }

  [[nodiscard]] const decltype(mMass) &getMass() const { return mMass; }

  virtual void applyMomentum(const VECT_T momentum)
  {
    auto currentMomentum = mVelocity * mMass;
    auto totalMomentum = momentum + currentMomentum;
    mPendingVelocity += totalMomentum / mMass;
  }

  virtual void applyScale(VECT_T &velToScale)
  {
    velToScale *= static_cast<float>(1);
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

  void forceXPos(const float newXpos)
  {
    VECT_T requestedPos(newXpos, mShape->getPosition().y);
    mShape->setPosition(requestedPos);
  }

  void forceYPos(const float newYpos)
  {
    VECT_T requestedPos(mShape->getPosition().x, newYpos);
    mShape->setPosition(requestedPos);
  }


  void forcePos(const float newXpos, const float newYpos)
  {
    VECT_T requestedPos(newXpos, newYpos);
    mShape->setPosition(requestedPos);
  }

  void updatePos() override
  {
    auto pos = mShape->getPosition();

    auto newXpos = pos.x + mVelocity.x;
    auto newYpos = pos.y + mVelocity.y;

    forcePos(newXpos, newYpos);
  }


  void updateVelocity() override
  {
    mVelocity = mPendingVelocity;// *= static_cast<float>(.995);
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

  bool intersects(const sf::FloatRect &otherBoundary) const override
  {
    const auto boundary = mShape->getGlobalBounds();

    return boundary.intersects(otherBoundary);
  }

  Entity(std::shared_ptr<SHAPE_T> shape, VECT_T initVel) : mShape(shape), mPendingVelocity(initVel)
  {
    updateVelocity();
  }
};


}// namespace ent::base

#endif