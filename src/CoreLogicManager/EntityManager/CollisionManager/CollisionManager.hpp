#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER

#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>
#include <concepts>

#include <map>
#include <set>


namespace col {


template<std::unsigned_integral KEY_T>
using collision_map_t = std::map<KEY_T, std::set<KEY_T>>;


template<class CollidableCollection, std::unsigned_integral KEY_T>
class CollisionManager
{

private:
  float magnitude(const sf::Vector2f &vec) const
  {
    return static_cast<float>(std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2)));
  }

  sf::Vector2f unitVector(const float xComponent, const float yComponent, const float mag) const
  {
    return sf::Vector2f(xComponent / mag, yComponent / mag);
  }

  template<typename VECT>
  VECT processBorderCollisions(const KEY_T otherId, VECT pendingVel)
  {
    auto [xVel, yVel] = pendingVel;
    // auto xVel = pendingVel.x;

    if (mBottomIds.find(otherId) != mBottomIds.end()) {
      pendingVel.y *= 0;//(yVel > 0) ? -1 : 1;

    } else if (mTopIds.find(otherId) != mTopIds.end()) {
      pendingVel.y *= (yVel > 0) ? 1 : -1;

    } else if (mLeftIds.find(otherId) != mLeftIds.end()) {
      pendingVel.x *= (xVel > 0) ? 1 : -1;

    } else if (mRightIds.find(otherId) != mRightIds.end()) {
      pendingVel.x *= (xVel > 0) ? -1 : 1;
    }
    return pendingVel;
  }

  template<typename ENT_T>
  void processStandardCollision(ENT_T entity, ENT_T otherEntity)
  {
    auto centerPos = entity.getCenterPosition();
    auto otherCenterPos = otherEntity.getCenterPosition();

    auto vecToOther = otherCenterPos - centerPos;
    auto mag = magnitude(vecToOther);
    auto unitVec = unitVector(vecToOther.x, vecToOther.y, mag);
    entity.mPendingVelocity += -(unitVec / static_cast<float>(2));
    otherEntity.mPendingVelocity += (unitVec / static_cast<float>(2));
  }

public:
  std::set<KEY_T> mLeftIds;
  std::set<KEY_T> mRightIds;
  std::set<KEY_T> mTopIds;
  std::set<KEY_T> mBottomIds;
  CollisionManager(){};

  void findCollisions(const KEY_T &id, collision_map_t<KEY_T> &collisionRecord, CollidableCollection &colliders)
  {
    auto &entity = *colliders[id];


    for (const auto &entry : colliders) {
      if (entry.first != id) {

        auto &otherEntity = *entry.second;

        if (entity.intersects(otherEntity)) {
          collisionRecord.insert({ id, { entry.first } });
        }
      }
    }
  }


  void processCollisions(collision_map_t<KEY_T> &collisionRecord, CollidableCollection &colliders)
  {
    spdlog::set_level(spdlog::level::debug);
    for (auto &collider : collisionRecord) {

      auto id = collider.first;
      auto &entity = *colliders[id];

      for (const auto otherId : collider.second) {

        auto &otherEntity = *colliders[otherId];


        if (otherEntity.mFixed) {
          entity.mPendingVelocity = processBorderCollisions<decltype(entity.mPendingVelocity)>(otherId, entity.mPendingVelocity);
        } else {
          processStandardCollision<decltype(entity)>(entity, otherEntity);
        }

        // spdlog::debug("collision");

        collisionRecord[otherId].erase(id);
      }
      collider.second.clear();
    }
    spdlog::set_level(spdlog::level::info);
  }
};

}// namespace col

#endif