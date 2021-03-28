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
  template<typename ENT_T>
  void processCollision(ENT_T entity, ENT_T otherEntity)
  {

    auto otherEntVelocity = otherEntity.getVelocity();
    const auto otherEntMass = otherEntity.getMass();

    otherEntity.applyScale(entity.mPendingVelocity);
    entity.applyMomentum(otherEntMass * otherEntVelocity);
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

        // collisionRecord[otherId].erase(id);
      }
      collider.second.clear();
    }
    spdlog::set_level(spdlog::level::info);
  }
};

}// namespace col

#endif