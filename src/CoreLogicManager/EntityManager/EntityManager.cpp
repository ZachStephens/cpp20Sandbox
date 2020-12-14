#include <SFML/Graphics/CircleShape.hpp>
#include <spdlog/spdlog.h>

#include "EntityManager.hpp"
#include "Entity/Entity.hpp"

#include "GuiManager/Messages/GuiManMessages.hpp"
#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"

#include <iostream>

namespace ent::man {


EntityManager::EntityManager()
{

  const uint8_t ids[] = { 5, 6, 7, 8, 9, 10 };

  for (const uint8_t id : ids) {
    auto val = static_cast<float>(50 * id);

    auto shape = std::make_shared<sf::CircleShape>(25.f, 4);
    shape->rotate(45);
    shape->setPosition(sf::Vector2f(val, val));
    shape->setFillColor(sf::Color::Green);

    mEntityCollection.insert(std::pair<const gman::shapeId_t, std::unique_ptr<Entity>>(id, std::make_unique<Entity>(shape)));
    if (mEntityCollection[id]->autonomous) {
      mEntityCollection[id]->updateDirection(static_cast<float>(5 * (mDistrib(mGenerator) - .5)), static_cast<float>(5 * (mDistrib(mGenerator) - .5)));
    }
  }
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


void EntityManager::processDirectionsMessage(bool downPress, const sf::Keyboard::Key &key)
{

  // spdlog::set_level(spdlog::level::debug);

  for (auto &mapEntry : mEntityCollection) {

    // spdlog::debug("ProcessDirections for {}", mapEntry.first);
    mapEntry.second->processDirectionsMessage(downPress, key);
  }
  // spdlog::set_level(spdlog::level::info);
}

const std::vector<gman::shapeId_t> EntityManager::collisionCheck(const gman::shapeId_t &id, const sf::Vector2<float> &diffLocation)
{
  auto sh = mEntityCollection[id]->getShape();
  auto bounds = sh->getGlobalBounds();

  auto pos = sh->getPosition();

  spdlog::debug("id: {}, top: {}, left: {}, height: {}, width: {}", id, bounds.top, bounds.left, bounds.height, bounds.width);
  spdlog::debug("id: {}, posX: {}, posY", id, pos.x, pos.y);


  bounds.top += diffLocation.y;
  bounds.left += diffLocation.x;

  std::vector<gman::shapeId_t> collision;


  for (const auto &entry : mEntityCollection) {
    if (entry.first != id && mCollisionBufferCounterMap.find(entry.first) == mCollisionBufferCounterMap.end()) {
      const auto &localbounds = entry.second->getShape()->getGlobalBounds();
      if (bounds.intersects(localbounds)) {
        collision.push_back(entry.first);
        spdlog::debug("collision");
      }
    }
  }

  return collision;
}


void EntityManager::recordCollision(const gman::shapeId_t id1, const gman::shapeId_t id2, collision_map_t &collisionMap)
{
  if (collisionMap.find(id1) != collisionMap.end()) {
    collisionMap[id1].insert(id2);
  } else {
    collisionMap[id1] = { id2 };
  }
  mCollisionBufferCounterMap[id1] = 5;
}

void EntityManager::handleCollisions(const gman::shapeId_t collider_id, const std::vector<gman::shapeId_t> &collisionsIds, collision_map_t &collisionMap)
{

  for (const auto &foreignEntityId : collisionsIds) {

    if (collisionMap.find(collider_id) == collisionMap.end() || (collisionMap[collider_id].find(foreignEntityId) == collisionMap[collider_id].end())) {

      const auto foriegn_vel = mEntityCollection[foreignEntityId]->getDirection().mDirVec;
      const auto collider_vel = mEntityCollection[collider_id]->getDirection().mDirVec;

      mEntityCollection[collider_id]->applyForce(foriegn_vel);
      mEntityCollection[foreignEntityId]->applyForce(collider_vel);

      recordCollision(collider_id, foreignEntityId, collisionMap);
      recordCollision(foreignEntityId, collider_id, collisionMap);
    }
  }
}// namespace ent::man

void EntityManager::update()
{

  collision_map_t colMap;

  for (auto it = mCollisionBufferCounterMap.begin(); it != mCollisionBufferCounterMap.end();) {
    if ((--(it->second)) == 0) {
      mCollisionBufferCounterMap.erase(it++);// or "it = m.erase(it)" since C++11
    } else {
      ++it;
    }
  }

  for (auto &mapEntry : mEntityCollection) {
    auto direction = mapEntry.second->getDirection();


    const float DELTA_MAG = 1;//static_cast<float>(1 + .5 * mDistrib(mGenerator));
    float deltax = DELTA_MAG * direction.mDirVec.x;
    float deltay = DELTA_MAG * direction.mDirVec.y;

    spdlog::set_level(spdlog::level::debug);

    auto diffVec = sf::Vector2f(deltax, deltay);
    auto collisions = collisionCheck(mapEntry.first, diffVec);
    if (collisions.empty()) {

      mapEntry.second->updatePos(deltax, deltay);

    } else {

      spdlog::debug("Handling collisions");
      handleCollisions(mapEntry.first, collisions, colMap);
    }
    // }
    spdlog::set_level(spdlog::level::info);
  }
}

}// namespace ent::man