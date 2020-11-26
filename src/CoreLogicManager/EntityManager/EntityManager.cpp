#include "EntityManager.hpp"

#include "GuiManager/Messages/GuiManMessages.hpp"
#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"
#include <spdlog/spdlog.h>


namespace entman {

EntityManager::EntityManager()
{

  const auto ids = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

  for (auto id : ids) {
    mPosMap.insert(posMapPair_t(id, std::tuple<float, float>(200 + id * 45, 200 + id * 45)));
    mDirMap.insert(std::pair<gman::shapeId_t, DIRECTIONS_STATE>(id, DIRECTIONS_STATE()));
  }
}


inline void EntityManager::boundsCheck(float &x, const float MAX_N_MAG)
{
  if (x >= MAX_N_MAG) {
    x = MAX_N_MAG;
  } else if (x <= 0) {
    x = 0;
  }
}

void EntityManager::updatePos(const gman::shapeId_t id, const float deltaX, const float deltaY)
{
  float xpos, ypos;
  std::tie(xpos, ypos) = mPosMap[id];

  auto newXpos = xpos + deltaX;
  auto newYpos = ypos + deltaY;

  boundsCheck(newXpos, MAX_X_MAG);
  boundsCheck(newYpos, MAX_Y_MAG);

  mPosMap[id] = std::tuple<float, float>(newXpos, newYpos);
}


void EntityManager::processDirectionsMessage(bool downPress, const sf::Keyboard::Key &key)
{

  for (auto &[id, direction] : mDirMap) {

    //spdlog::debug("processDirectionsMessage: PRESSED: {}, key: {}", downPress, key);


    switch (key) {
    case sf::Keyboard::Left:
    case sf::Keyboard::A:
      spdlog::debug("processDirectionsMessage: LEFT {}", direction.LEFT);
      direction.LEFT = (downPress) ? true : false;
      spdlog::debug("processDirectionsMessage: LEFT {}", direction.LEFT);
      break;
    case sf::Keyboard::Right:
    case sf::Keyboard::D:
      spdlog::debug("processDirectionsMessage: RIGHT {}", direction.RIGHT);
      direction.RIGHT = (downPress) ? true : false;
      spdlog::debug("processDirectionsMessage: RIGHT {}", direction.RIGHT);
      break;
    case sf::Keyboard::Up:
    case sf::Keyboard::W:
      spdlog::debug("processDirectionsMessage: UP {}", direction.UP);
      direction.UP = (downPress) ? true : false;
      spdlog::debug("processDirectionsMessage: UP {}", direction.UP);
      break;
    case sf::Keyboard::Down:
    case sf::Keyboard::S:
      spdlog::debug("processDirectionsMessage: DOWN {}", direction.DOWN);
      direction.DOWN = (downPress) ? true : false;
      spdlog::debug("processDirectionsMessage: DOWN {}", direction.DOWN);
      break;
    default:
      break;
    }


    mDirMap[id] = direction;
  }
}

void EntityManager::update()
{

  for (auto &[id, direction] : mDirMap) {

    // spdlog::debug("update: UP {}", direction.UP);
    // spdlog::debug("update: DOWN {}", direction.DOWN);
    // spdlog::debug("update: LEFT {}", direction.LEFT);
    // spdlog::debug("update: RIGHT {}", direction.RIGHT);

    const auto DELTA_MAG = 1;

    float deltax = 0.0;
    float deltay = 0.0;
    bool diffed = false;

    if (direction.LEFT) {
      diffed = true;
      deltax += -DELTA_MAG;
    }
    if (direction.RIGHT) {
      diffed = true;
      deltax += DELTA_MAG;
    }
    if (direction.UP) {
      diffed = true;
      deltay += -DELTA_MAG;
    }
    if (direction.DOWN) {
      diffed = true;
      deltay += DELTA_MAG;
    }

    if (diffed) {
      updatePos(id, deltax, deltay);
    }
  }
}

}// namespace entman