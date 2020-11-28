#include <SFML/Graphics/CircleShape.hpp>
#include <spdlog/spdlog.h>

#include "EntityManager.hpp"
#include "Entity/Entity.hpp"

#include "GuiManager/Messages/GuiManMessages.hpp"
#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"


namespace ent::man {

EntityManager::EntityManager()
{

  const uint8_t ids[] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

  for (const uint8_t id : ids) {
    auto val = static_cast<float>(50 * id);

    auto shape = std::make_shared<sf::CircleShape>(25.f, 5);
    shape->setPosition(sf::Vector2f(val, val));

    auto Ent = Entity(shape);
    mEntityCollection.insert(std::pair<const gman::shapeId_t, Entity>(id, Ent));
  }
}


std::unique_ptr<std::vector<std::shared_ptr<sf::Shape>>> EntityManager::getShapesToWrite()
{
  auto shapesToWrite = std::make_unique<std::vector<std::shared_ptr<sf::Shape>>>();
  for (const auto mapEntry : mEntityCollection) {
    auto shapeToWrite = mapEntry.second.getShape();
    shapesToWrite->push_back(shapeToWrite);
  }
  return shapesToWrite;
}


void EntityManager::processDirectionsMessage(bool downPress, const sf::Keyboard::Key &key)
{

  spdlog::set_level(spdlog::level::info);

  for (const auto mapEntry : mEntityCollection) {
    auto entity = mapEntry.second;

    entity.processDirectionsMessage(downPress, key);
  }
}


void EntityManager::update()
{

  for (const auto mapEntry : mEntityCollection) {
    auto entity = mapEntry.second;

    auto direction = entity.getDirection();
    // spdlog::debug("update: UP {}", direction.UP);
    // spdlog::debug("update: DOWN {}", direction.DOWN);
    // spdlog::debug("update: LEFT {}", direction.LEFT);
    // spdlog::debug("update: RIGHT {}", direction.RIGHT);


    const auto DELTA_MAG = 5;
    float deltax = 0.0;
    float deltay = 0.0;
    bool diffed = false;

    if (direction.LEFT) {
      diffed = true;
      deltax += -DELTA_MAG;
    }
    if (direction.RIGHT) {
      diffed = true;
      deltax += DELTA_MAG;
    }
    if (direction.UP) {
      diffed = true;
      deltay += -DELTA_MAG;
    }
    if (direction.DOWN) {
      diffed = true;
      deltay += DELTA_MAG;
    }

    if (diffed) {
      entity.updatePos(deltax, deltay);
    }
  }
}

}// namespace ent::man