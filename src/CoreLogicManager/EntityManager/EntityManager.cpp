#include <SFML/Graphics/CircleShape.hpp>
// #include <SFML/Graphics/Color.hpp>

#include <spdlog/spdlog.h>

#include "EntityManager.hpp"
#include "Entity/Entity.hpp"
#include "Entity/FixedEntity.hpp"

#include "GuiManager/Messages/GuiManMessages.hpp"
#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"

#include <concepts>
#include <iostream>

namespace ent::man {


std::shared_ptr<sf::Shape> EntityManager::initShape(const float size,
  const sf::Texture &texture,
  const sf::Vector2f &initPos)
{

  auto shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(size, size));
  shape->setPosition(initPos);
  shape->setTexture(&texture);

  return shape;
}


uint16_t EntityManager::configureBorderEntity(
  const float size,
  const sf::Texture &texture,
  const sf::Vector2f &initPos,
  const bool xyDirection)
{

  auto shape = initShape(size, texture, initPos);

  auto latestId = ++mEntityId;


  // xyDirection == true -> configure horizontal border entity (xFixedEntity)
  // else create vertical border entity
  if (xyDirection) {
    SFML_ENTITY_PTR fixedEnt = std::make_unique<SFML_XFIXED_ENTITY>(shape);
    mEntityCollection.insert(std::pair<const gman::shapeId_t, SFML_ENTITY_PTR>(latestId, std::move(fixedEnt)));
  } else {
    SFML_ENTITY_PTR fixedEnt = std::make_unique<SFML_YFIXED_ENTITY>(shape);
    mEntityCollection.insert(std::pair<const gman::shapeId_t, SFML_ENTITY_PTR>(latestId, std::move(fixedEnt)));
  }

  mStaticEntityIds.insert(latestId);

  return latestId;
}

uint16_t EntityManager::configureEntity(
  const float floatSpeed,
  const float size,
  const sf::Texture &texture,
  const sf::Vector2f &initPos)
{
  auto shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(size, size));
  shape->setPosition(initPos);
  shape->setTexture(&texture);

  const auto initVel = sf::Vector2f(floatSpeed * static_cast<float>((mDistrib(mGenerator) - .5)), floatSpeed * static_cast<float>((mDistrib(mGenerator) - .5)));

  auto latestId = ++mEntityId;

  SFML_ENTITY_PTR fixedEnt = std::make_unique<SFML_ENTITY>(shape, initVel);

  mEntityCollection.insert(std::pair<const gman::shapeId_t, SFML_ENTITY_PTR>(latestId, std::move(fixedEnt)));

  mDynamicEntityIds.insert(latestId);

  return latestId;
}


void EntityManager::configureBorder(const uint16_t width, const uint16_t height)
{
  uint16_t size = 1920;

  // build border
  for (uint16_t pos = 0; pos < width; pos += size) {
    configureBorderEntity(size, mDefaultTexture, sf::Vector2f(static_cast<float>(pos), -static_cast<float>(size)), false);//top
    configureBorderEntity(size, mDefaultTexture, sf::Vector2f(static_cast<float>(pos), height), false);//bottom
  }

  for (uint16_t pos = 0; pos < height; pos += size) {
    configureBorderEntity(size, mDefaultTexture, sf::Vector2f(-static_cast<float>(size), static_cast<float>(pos)), true);//left
    configureBorderEntity(size, mDefaultTexture, sf::Vector2f(width, static_cast<float>(pos)), true);//right
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
  const float size = 30;
  while (i++ < 60) {
    auto initPos = sf::Vector2f(960 + static_cast<float>(1850 * (mDistrib(mGenerator) - .5)), 540 + static_cast<float>(1000 * (mDistrib(mGenerator) - .5)));

    configureEntity(1.0, size, mBisonTexture, initPos);
  }
}

EntityManager::EntityManager()
{
  init();
}

void EntityManager::clearState()
{
  mEntityCollection.clear();
  mStaticEntityIds.clear();
  mDynamicEntityIds.clear();
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

  for (const auto &id : mDynamicEntityIds) {
    auto &entity = mEntityCollection[id];
    entity->mPendingVelocity *= static_cast<float>(.999);
    //mapEntry.second->mPendingVelocity.y += static_cast<float>(1);
    mCollisionManager.findCollisions(id, collisionRecord, mEntityCollection);
  }

  mCollisionManager.processCollisions(collisionRecord, mEntityCollection);

  for (const auto &id : mDynamicEntityIds) {
    auto &entity = mEntityCollection[id];
    entity->updateVelocity();
    entity->updatePos();
  }
}


}// namespace ent::man