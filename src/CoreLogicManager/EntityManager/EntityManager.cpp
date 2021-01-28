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
  const sf::Vector2f &initPos,
  const bool fixed)
{

  auto shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(size, size));

  shape->setPosition(initPos);

  shape->setTexture(&texture);

  const auto initVel = sf::Vector2f(floatSpeed * static_cast<float>((mDistrib(mGenerator) - .5)), floatSpeed * static_cast<float>((mDistrib(mGenerator) - .5)));

  mEntityCollection.insert(std::pair<const gman::shapeId_t, SFML_ENTITY_PTR>(id, std::make_unique<SFML_ENTITY>(shape, initVel, fixed)));
}

void EntityManager::init()
{
  std::random_device rd;
  mGenerator = std::mt19937(rd());

  sf::Image image;

  image.loadFromFile("/home/zach/work/basic0/cpp_starter_project/src/GuiManager/lone-bison.png");

  mBisonTexture.loadFromImage(image);
  mBisonTexture.setSmooth(true);

  uint16_t i = 1;


  float floatSpeed = 0.0;
  uint16_t size = 100;


  // build border
  for (uint16_t pos = 0; pos < 1920; pos += size) {
    configureEntity(i++, floatSpeed, size, mDefaultTexture, sf::Vector2f(static_cast<float>(pos), -static_cast<float>(size)), true);//top
    mCollisionManager.mTopIds.insert(i);
    configureEntity(i++, floatSpeed, size, mDefaultTexture, sf::Vector2f(static_cast<float>(pos), 1080), true);//bottom
    mCollisionManager.mBottomIds.insert(i);
  }

  for (uint16_t pos = 0; pos < 1080; pos += size) {
    configureEntity(i++, floatSpeed, size, mDefaultTexture, sf::Vector2f(-static_cast<float>(size), static_cast<float>(pos)), true);//left
    mCollisionManager.mLeftIds.insert(i);
    configureEntity(i++, floatSpeed, size, mDefaultTexture, sf::Vector2f(1920, static_cast<float>(pos)), true);//right
    mCollisionManager.mRightIds.insert(i);
  }

  // configureEntity(2, floatSpeed, size, mDefaultTexture, sf::Vector2f(-1920, 0), true);//left
  // configureEntity(3, floatSpeed, size, mDefaultTexture, sf::Vector2f(1920, 0), true);//right
  // configureEntity(4, floatSpeed, size, mDefaultTexture, sf::Vector2f(0, 1920), true);//bottom

  while (i++ < 200) {
    const uint16_t id = 30000 - i;
    floatSpeed = 1.0;
    size = 40;
    auto initPos = sf::Vector2f(960 + static_cast<float>(1850 * (mDistrib(mGenerator) - .5)), 540 + static_cast<float>(1000 * (mDistrib(mGenerator) - .5)));

    configureEntity(id, floatSpeed, size, mBisonTexture, initPos, false);
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