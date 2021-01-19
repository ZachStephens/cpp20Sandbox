#ifndef IENTITY
#define IENTITY
#include <SFML/Window/Keyboard.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>


#include "GuiManager/Messages/GuiManMessages.hpp"

template<typename SHAPE_T, typename VECT_T>
class IEntity
{
public:
  virtual inline const std::shared_ptr<SHAPE_T> getShape() const = 0;

  virtual const VECT_T getCenterPosition() const = 0;

  virtual inline const VECT_T getVelocity() const = 0;

  virtual void setTexture(const sf::Texture &texture) = 0;

  virtual void updatePos() = 0;

  virtual void updateVelocity() = 0;

  virtual void processVelocityMessage(bool downPress, const sf::Keyboard::Key &key) = 0;

  virtual bool intersects(const IEntity<SHAPE_T, VECT_T> &otherEntity) const = 0;

  virtual ~IEntity(){};
};

#endif