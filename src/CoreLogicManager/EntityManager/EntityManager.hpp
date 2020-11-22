#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER

#include <SFML/Window/Keyboard.hpp>

#include "GuiManager/Messages/GuiManMessages.hpp"

#include <map>
#include <stdint.h>

namespace entman {

constexpr auto MAX_X_MAG = 1920 - 50;
constexpr auto MAX_Y_MAG = 1080 - 50;

class EntityManager
{
private:
  struct DIRECTIONS_STATE
  {
    bool LEFT = false;
    bool RIGHT = false;
    bool UP = false;
    bool DOWN = false;
  };

  using posMapPair_t = std::pair<gman::shapeId_t, std::tuple<float, float>>;

  std::map<const gman::shapeId_t, DIRECTIONS_STATE> mDirMap;
  //std::map<const gman::shapeId_t, bool> > mColMap;

  void updatePos(const gman::shapeId_t id, const float deltaX, const float deltaY);

  inline void boundsCheck(float &x, const float MAX_N_MAG);


public:
  std::map<const gman::shapeId_t, std::tuple<float, float>> mPosMap;

  EntityManager();

  void processDirectionsMessage(bool downPress, const sf::Keyboard::Key &key);

  void update();
};

}// namespace entman

#endif