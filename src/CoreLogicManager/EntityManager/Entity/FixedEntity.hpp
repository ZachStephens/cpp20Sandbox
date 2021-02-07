#ifndef FIXED_ENTITY
#define FIXED_ENTITY

#include "IEntity.hpp"
#include <SFML/Window/Keyboard.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>


#include "GuiManager/Messages/GuiManMessages.hpp"

#include <map>
#include <stdint.h>

namespace ent::base::fixed {


template<typename SHAPE_T, typename VECT_T>
class FixedEntity : public Entity<SHAPE_T, VECT_T>
{
private:
public:
  inline const std::shared_ptr<SHAPE_T> getShape() const override { return this->mShape; };

  const VECT_T getCenterPosition() const override
  {
    const auto &bounds = this->mShape->getGlobalBounds();
    return VECT_T(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
  }

  inline const VECT_T getVelocity() const override { return VECT_T(0.0, 0.0); };

  void setTexture(const sf::Texture &texture)
  {
    // spdlog::set_level(spdlog::level::info);
    // spdlog::info("settingImage");

    this->mShape->setTexture(&texture);
    // this->mShape->setScale(ScaleX, ScaleY);//Set scale.
  }

  void updatePos() override
  {
    return;
  }

  void updateVelocity() override
  {
    return;
  }

  void processVelocityMessage(bool downPress, const sf::Keyboard::Key &key) override
  {

    spdlog::set_level(spdlog::level::info);

    if (downPress)
      return;
    switch (key) {
    case sf::Keyboard::Left:
    case sf::Keyboard::A:
      // mVelocity.x = (downPress) ? -1 : 0;
      // break;
    case sf::Keyboard::Right:
    case sf::Keyboard::D:
      // mVelocity.x = (downPress) ? 1 : 0;
      // break;
    case sf::Keyboard::Up:
    case sf::Keyboard::W:
      // mVelocity.y = (downPress) ? -1 : 0;
      // break;
    case sf::Keyboard::Down:
    case sf::Keyboard::S:
      // mVelocity.y = (downPress) ? 1 : 0;
      spdlog::debug("Border recieved key message");
      break;
    default:
      break;
    }

    spdlog::set_level(spdlog::level::info);
  }

  bool intersects(const sf::FloatRect &otherBoundary) const override
  {
    const auto boundary = this->mShape->getGlobalBounds();

    return boundary.intersects(otherBoundary);
  }

  FixedEntity(std::shared_ptr<SHAPE_T> shape) : Entity<SHAPE_T, VECT_T>(shape, { 0.0, 0.0 })
  {
  }
};

template<typename SHAPE_T, typename VECT_T>
class FixedXEntity : public FixedEntity<SHAPE_T, VECT_T>
{
};

template<typename SHAPE_T, typename VECT_T>
class FixedYEntity : public FixedEntity<SHAPE_T, VECT_T>
{
};


}// namespace ent::base::fixed

#endif