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


template<class BORDER_T>
uint16_t EntityManager::configureBorderEntity(
  const float size,
  const sf::Texture &texture,
  const sf::Vector2f &initPos)
{

  auto shape = initShape(size, texture, initPos);

  auto latestId = ++mEntityId;

  SFML_ENTITY_PTR fixedEnt = std::make_unique<BORDER_T>(shape);
  mEntityCollection.insert(std::pair<const gman::shapeId_t, SFML_ENTITY_PTR>(latestId, std::move(fixedEnt)));


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


  const float XVEL_CONST = floatSpeed * (getRandom() - HALF);
  const float YVEL_CONST = floatSpeed * (getRandom() - HALF);
  const auto initVel = sf::Vector2f(XVEL_CONST, YVEL_CONST);

  auto latestId = ++mEntityId;

  SFML_ENTITY_PTR fixedEnt = std::make_unique<SFML_ENTITY>(shape, initVel);
  fixedEnt->updateVelocity();

  mEntityCollection.insert(std::pair<const gman::shapeId_t, SFML_ENTITY_PTR>(latestId, std::move(fixedEnt)));

  mDynamicEntityIds.insert(latestId);

  return latestId;
}

void EntityManager::configureBorder(const uint16_t width, const uint16_t height)
{
  uint16_t size = BORDER_WIDTH;

  // build border
  for (uint16_t pos = 0; pos < width; pos += size) {
    using namespace ent::base::fixed;
    configureBorderEntity<FixedYEntity<sf::Shape, sf::Vector2f, 1>>(size, mDefaultTexture, sf::Vector2f(static_cast<float>(pos), -static_cast<float>(size)));//top
    configureBorderEntity<FixedYEntity<sf::Shape, sf::Vector2f, -1>>(size, mDefaultTexture, sf::Vector2f(static_cast<float>(pos), height));//bottom
  }

  for (uint16_t pos = 0; pos < height; pos += size) {
    using namespace ent::base::fixed;
    configureBorderEntity<FixedXEntity<sf::Shape, sf::Vector2f, 1>>(size, mDefaultTexture, sf::Vector2f(-static_cast<float>(size), static_cast<float>(pos)));//left
    configureBorderEntity<FixedXEntity<sf::Shape, sf::Vector2f, -1>>(size, mDefaultTexture, sf::Vector2f(width, static_cast<float>(pos)));//right
  }
}

float EntityManager::getRandom()
{
  return static_cast<float>(mDistrib(mGenerator));
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
  configureBorder(BORDER_WIDTH, BORDER_HEIGHT);

  uint16_t i = 0;
  const float SIZE_INIT = 15;
  const float FLOAT_SPEED_INIT = 2;
  const uint16_t ENT_NUM = 600;

  while (i++ < ENT_NUM) {
    const float DIST_OFFSET = HALF;
    const float RAND_VAL_OFFSET = HALF;

    const auto MY_WIDTH_CONST = RAND_VAL_OFFSET + getRandom() - DIST_OFFSET;
    const auto MY_HEIGHT_CONST = RAND_VAL_OFFSET + getRandom() - DIST_OFFSET;

    auto initPos = sf::Vector2f(BORDER_WIDTH * MY_WIDTH_CONST, BORDER_HEIGHT * MY_HEIGHT_CONST);

    configureEntity(FLOAT_SPEED_INIT, SIZE_INIT, mBisonTexture, initPos);
  }
}

EntityManager::EntityManager()// NOLINT
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

  const auto DELTA_VEL_FACTOR = static_cast<float>(.999);

  for (const auto &id : mDynamicEntityIds) {
    auto &entity = mEntityCollection[id];
    entity->mPendingVelocity *= DELTA_VEL_FACTOR;
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