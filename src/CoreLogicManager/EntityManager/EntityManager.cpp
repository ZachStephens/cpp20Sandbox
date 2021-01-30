#include <SFML/Graphics/CircleShape.hpp>
// #include <SFML/Graphics/Color.hpp>

#include <spdlog/spdlog.h>

#include "EntityManager.hpp"
#include "Entity/Entity.hpp"
#include "Entity/FixedEntity.hpp"

#include "GuiManager/Messages/GuiManMessages.hpp"
#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"


#include <iostream>

namespace ent::man {


uint16_t EntityManager::configureEntity(
  const float floatSpeed,
  const float size,
  const sf::Texture &texture,
  const sf::Vector2f &initPos,
  const bool fixed)
{

  auto shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(size, size));

  shape->setPosition(initPos);

  shape->setTexture(&texture);

  const auto initVel = sf::Vector2f(floatSpeed * static_cast<float>((mDistrib(mGenerator) - .5)), floatSpeed * static_cast<float>((mDistrib(mGenerator) - .5)));

  auto latestId = ++mEntityId;


  if (fixed) {
    SFML_ENTITY_PTR fixedEnt = std::make_unique<SFML_FIXED_ENTITY>(shape);
    mEntityCollection.insert(std::pair<const gman::shapeId_t, SFML_ENTITY_PTR>(latestId, std::move(fixedEnt)));
  } else {
    mEntityCollection.insert(std::pair<const gman::shapeId_t, SFML_ENTITY_PTR>(latestId, std::make_unique<SFML_ENTITY>(shape, initVel)));
  }

  return latestId;
}


void EntityManager::configureBorder(const uint16_t width, const uint16_t height)
{
  float floatSpeed = 0.0;
  uint16_t size = 250;

  // build border
  for (uint16_t pos = 0; pos < width; pos += size) {
    mCollisionManager.mTopIds.insert(configureEntity(floatSpeed, size, mDefaultTexture, sf::Vector2f(static_cast<float>(pos), -static_cast<float>(size)), true));//top
    mCollisionManager.mBottomIds.insert(configureEntity(floatSpeed, size, mDefaultTexture, sf::Vector2f(static_cast<float>(pos), height), true));//bottom
  }

  for (uint16_t pos = 0; pos < height; pos += size) {
    mCollisionManager.mLeftIds.insert(configureEntity(floatSpeed, size, mDefaultTexture, sf::Vector2f(-static_cast<float>(size), static_cast<float>(pos)), true));//left
    mCollisionManager.mRightIds.insert(configureEntity(floatSpeed, size, mDefaultTexture, sf::Vector2f(width, static_cast<float>(pos)), true));//right
  }
}

void EntityManager::init()
{
  std::random_device rd;
  mGenerator = std::mt19937(rd());

  sf::Image image;

  image.loadFromFile("/home/zach/work/basic0/cpp_starter_project/src/GuiManager/lone-bison.png");

  mBisonTexture.loadFromImage(image);
  mBisonTexture.setSmooth(true);


  // build border
  configureBorder(1920, 1080);

  uint16_t i = 0;
  const float size = 76;
  while (i++ < 20) {
    auto initPos = sf::Vector2f(960 + static_cast<float>(1850 * (mDistrib(mGenerator) - .5)), 540 + static_cast<float>(1000 * (mDistrib(mGenerator) - .5)));

    configureEntity(1.0, size, mBisonTexture, initPos, false);
  }
}

EntityManager::EntityManager()
{
  init();
}

void EntityManager::clearState()
{
  mEntityCollection.clear();
  mBisonTexture = {};
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


void EntityManager::update()
{

  col::collision_map_t<gman::shapeId_t> collisionRecord;

  for (auto &mapEntry : mEntityCollection) {
    mapEntry.second->mPendingVelocity *= static_cast<float>(.99);
    mapEntry.second->mPendingVelocity.y += static_cast<float>(1);
    if (!mapEntry.second->mFixed) {
      mCollisionManager.findCollisions(mapEntry.first, collisionRecord, mEntityCollection);
    }
  }

  mCollisionManager.processCollisions(collisionRecord, mEntityCollection);

  for (auto &mapEntry : mEntityCollection) {
    auto &entity = *mapEntry.second;
    entity.updateVelocity();
    entity.updatePos();
  }
}


}// namespace ent::man