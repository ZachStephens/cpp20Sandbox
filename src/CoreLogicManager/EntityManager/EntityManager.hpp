#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>


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
  using SFML_ENTITY = Entity<sf::Shape, sf::Vector2f>;
  using SFML_ENTITY_PTR = std::unique_ptr<SFML_ENTITY>;

  std::map<gman::shapeId_t, SFML_ENTITY_PTR> mEntityCollection;

  sf::Texture mEntityTexture;

  std::mt19937 mGenerator;
  std::uniform_real_distribution<> mDistrib;


  void findCollisions(const gman::shapeId_t &id, collision_map_t &collisionRecord);

  void processCollisions(collision_map_t &collisionRecord);

  void configureEntity(
    const uint16_t id,
    const float floatSpeed,
    const float size,
    const sf::Texture &texture,
    const sf::Vector2f &initPos);

  void clearState();

  void init();

public:
  std::unique_ptr<std::vector<std::shared_ptr<sf::Shape>>> getShapesToWrite();

  EntityManager();

  void reset();

  void processVelocityMessage(bool downPress, const sf::Keyboard::Key &key);

  void update();
};

}// namespace ent::man

#endif