#include <SFML/Graphics/CircleShape.hpp>
// #include <SFML/Graphics/Color.hpp>

#include <spdlog/spdlog.h>

#include "EntityManager.hpp"
#include "Entity/Entity.hpp"

#include "GuiManager/Messages/GuiManMessages.hpp"
#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"


#include <iostream>

namespace ent::man {


void EntityManager::configureEntity(
  const uint16_t id,
  const float floatSpeed,
  const float size,
  const sf::Texture &texture,
  const sf::Vector2f &initPos)
{

  auto shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(size, size));

  shape->setPosition(initPos);

  shape->setTexture(&texture);

  const auto initVel = sf::Vector2f(floatSpeed * static_cast<float>((mDistrib(mGenerator) - .5)), floatSpeed * static_cast<float>((mDistrib(mGenerator) - .5)));

  mEntityCollection.insert(std::pair<const gman::shapeId_t, SFML_ENTITY_PTR>(id, std::make_unique<SFML_ENTITY>(shape, initVel)));
}

void EntityManager::init()
{
  std::random_device rd;
  mGenerator = std::mt19937(rd());

  sf::Image image;

  image.loadFromFile("/home/zach/work/basic0/cpp_starter_project/src/GuiManager/lone-bison.png");

  mEntityTexture.loadFromImage(image);
  mEntityTexture.setSmooth(true);

  uint16_t i = 15;

  const uint16_t id = 30000 - i;
  const float floatSpeed = 5.0;
  const float size = 170;
  configureEntity(id, floatSpeed, size, mEntityTexture);

  while (i-- > 4) {
    const uint16_t id = 30000 - i;
    const float floatSpeed = 5.0;
    const float size = 170;
    const auto initPos = sf::Vector2f(960 + static_cast<float>(1850 * (mDistrib(mGenerator) - .5)), 540 + static_cast<float>(1000 * (mDistrib(mGenerator) - .5)));

    configureEntity(id, floatSpeed, size, mEntityTexture, initPos);
  }
}

EntityManager::EntityManager()
{
  init();
}

void EntityManager::clearState()
{
  mEntityCollection.clear();
  mEntityTexture = {};
}

void EntityManager::reset()
{
  clearState();
  init();
}

std::unique_ptr<std::vector<std::shared_ptr<sf::Shape>>> EntityManager::getShapesToWrite()
{
  auto shapesToWrite = std::make_unique<std::vector<std::shared_ptr<sf::Shape>>>();
  for (const auto &mapEntry : mEntityCollection) {
    auto shapeToWrite = mapEntry.second->getShape();
    shapesToWrite->push_back(shapeToWrite);
  }
  return shapesToWrite;
}


void EntityManager::processVelocityMessage(bool downPress, const sf::Keyboard::Key &key)
{

  // spdlog::set_level(spdlog::level::debug);

  for (auto &mapEntry : mEntityCollection) {

    // spdlog::debug("ProcessVelocitys for {}", mapEntry.first);
    mapEntry.second->processVelocityMessage(downPress, key);
  }
  // spdlog::set_level(spdlog::level::info);
}

float magnitude(const sf::Vector2f &vec)
{
  return static_cast<float>(std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2)));
}

sf::Vector2f unitVector(const float xComponent, const float yComponent, const float mag)
{
  return sf::Vector2f(xComponent / mag, yComponent / mag);
}


void EntityManager::findCollisions(const gman::shapeId_t &id, collision_map_t &collisionRecord)
{
  auto &entity = *mEntityCollection[id];


  for (const auto &entry : mEntityCollection) {
    if (entry.first != id) {

      auto &otherEntity = *entry.second;

      if (entity.intersects(otherEntity)) {
        collisionRecord.insert({ id, { entry.first } });
      }
    }
  }
}

void EntityManager::processCollisions(collision_map_t &collisionRecord)
{
  spdlog::set_level(spdlog::level::debug);
  for (auto &collider : collisionRecord) {

    auto id = collider.first;
    auto &entity = *mEntityCollection[id];
    auto centerPos = entity.getCenterPosition();

    for (const auto otherId : collider.second) {


      auto &otherEntity = *mEntityCollection[otherId];
      auto otherCenterPos = otherEntity.getCenterPosition();

      auto vecToOther = otherCenterPos - centerPos;
      auto mag = magnitude(vecToOther);
      auto unitVec = unitVector(vecToOther.x, vecToOther.y, mag);

      entity.mPendingVelocity += -(unitVec / static_cast<float>(2));
      spdlog::debug("collision");

      collisionRecord[otherId].erase(id);
    }
    collider.second.clear();
  }
  spdlog::set_level(spdlog::level::info);
}

void EntityManager::update()
{

  collision_map_t collisionRecord;

  for (auto &mapEntry : mEntityCollection) {
    findCollisions(mapEntry.first, collisionRecord);
  }

  processCollisions(collisionRecord);

  for (auto &mapEntry : mEntityCollection) {
    auto &entity = *mapEntry.second;
    entity.updateVelocity();
    entity.updatePos();
  }
}


}// namespace ent::man