#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>


#include "GuiManager/Messages/GuiManMessages.hpp"
#include "Entity/Entity.hpp"
#include "CollisionManager/CollisionManager.hpp"

#include <map>
#include <set>
#include <stdint.h>
#include <random>
namespace ent::man {

class EntityManager
{
private:
  using SFML_ENTITY = Entity<sf::Shape, sf::Vector2f>;
  using SFML_ENTITY_PTR = std::unique_ptr<SFML_ENTITY>;

  std::map<gman::shapeId_t, SFML_ENTITY_PTR> mEntityCollection;
  col::CollisionManager<decltype(mEntityCollection), gman::shapeId_t> mCollisionManager;


  sf::Texture mBisonTexture;
  sf::Texture mDefaultTexture;

  std::mt19937 mGenerator;
  std::uniform_real_distribution<> mDistrib;


  void configureEntity(
    const uint16_t id,
    const float floatSpeed,
    const float size,
    const sf::Texture &texture,
    const sf::Vector2f &initPos,
    const bool fixed);

  void
    clearState();

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