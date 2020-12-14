#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER

#include <SFML/Window/Keyboard.hpp>

#include "GuiManager/Messages/GuiManMessages.hpp"
#include "Entity/Entity.hpp"

#include <map>
#include <set>
#include <stdint.h>
#include <random>
namespace ent::man {

using collision_map_t = std::map<gman::shapeId_t, std::set<gman::shapeId_t>>;

class EntityManager
{
private:
  std::map<gman::shapeId_t, std::unique_ptr<Entity>> mEntityCollection;
  std::map<gman::shapeId_t, uint8_t> mCollisionBufferCounterMap;

  std::mt19937 mGenerator;
  std::uniform_real_distribution<> mDistrib;

  void recordCollision(const gman::shapeId_t id1, const gman::shapeId_t id2, collision_map_t &collisionMap);

  void handleCollisions(const gman::shapeId_t id, const std::vector<gman::shapeId_t> &collisionsIds, collision_map_t &collisionMap);

  const std::vector<gman::shapeId_t> collisionCheck(const gman::shapeId_t &id, const sf::Vector2<float> &diffLocation);


public:
  std::unique_ptr<std::vector<std::shared_ptr<sf::Shape>>> getShapesToWrite();

  EntityManager();

  void processDirectionsMessage(bool downPress, const sf::Keyboard::Key &key);

  void update();
};

}// namespace ent::man

#endif