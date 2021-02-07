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

  // template<typename ENT_T>
  // void processBorderCollision(ENT_T entity)
  // {
  //   auto topBounds = borderEntity.getTopBounds();
  //   auto bottomBounds = borderEntity.getBottomBounds();
  //   auto leftBounds = borderEntity.getLeftBounds();
  //   auto rightBounds = borderEntity.getRightBounds();


  //   bool collideTop = (entity.intersects(topBounds));
  //   bool collideLeft = (entity.intersects(leftBounds));
  //   bool collideBottom = (entity.intersects(bottomBounds));
  //   bool collideRight = (entity.intersects(rightBounds));


  //   if (collideLeft) {
  //     entity.mPendingVelocity.x += -entity.mPendingVelocity.x - 1;
  //   } else if (collideRight) {
  //     entity.mPendingVelocity.x += -entity.mPendingVelocity.x + 1;
  //   }


  //   if (collideTop) {
  //     entity.mPendingVelocity.y += -entity.mPendingVelocity.y - 1;
  //   } else if (collideBottom) {
  //     entity.mPendingVelocity.y += -entity.mPendingVelocity.y + 1;
  //   }
  // }

  template<typename ENT_T>
  void processCollision(ENT_T entity, ENT_T otherEntity)
  {


    const auto centerPos = entity.getCenterPosition();
    const auto entVelocity = entity.getVelocity();
    const auto entMass = entity.getMass();

    //
    auto opposingForce = otherEntity.applyMomentum(centerPos, entVelocity, entMass);

    auto otherCenterPos = otherEntity.getCenterPosition();

    auto vecToOther = otherCenterPos - centerPos;
    auto mag = magnitude(vecToOther);
    auto unitVec = unitVector(vecToOther.x, vecToOther.y, mag);
    entity.mPendingVelocity += -(unitVec / static_cast<float>(2));
    otherEntity.mPendingVelocity += (unitVec / static_cast<float>(2));
  }

public:
  CollisionManager(){};

  void findCollisions(const KEY_T &id, collision_map_t<KEY_T> &collisionRecord, CollidableCollection &colliders)
  {
    auto &entity = *colliders[id];


    for (const auto &entry : colliders) {
      if (entry.first != id) {

        auto &otherEntity = *entry.second;

        if (entity.intersects(otherEntity.getShape()->getGlobalBounds())) {
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

        using ENTITY_T = decltype(entity);
        processCollision<ENTITY_T>(entity, otherEntity);

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