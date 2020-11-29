#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER

#include <SFML/Window/Keyboard.hpp>

#include "GuiManager/Messages/GuiManMessages.hpp"
#include "Entity/Entity.hpp"

#include <map>
#include <stdint.h>

namespace ent::man {


class EntityManager
{
private:
  std::map<gman::shapeId_t, std::unique_ptr<Entity>> mEntityCollection;

public:
  std::unique_ptr<std::vector<std::shared_ptr<sf::Shape>>> getShapesToWrite();

  EntityManager();

  void processDirectionsMessage(bool downPress, const sf::Keyboard::Key &key);

  void update();
};

}// namespace ent::man

#endif