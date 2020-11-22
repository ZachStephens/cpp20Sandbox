#include "EntityManager.hpp"

#include "GuiManager/Messages/GuiManMessages.hpp"
#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"
#include <spdlog/spdlog.h>


namespace entman {

EntityManager::EntityManager()
{
  mPosMap.insert(posMapPair_t(5, std::tuple<float, float>(600, 600)));
  mDirMap.insert(std::pair<gman::shapeId_t, DIRECTIONS_STATE>(5, DIRECTIONS_STATE()));
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
  auto &direction = mDirMap[5];

  spdlog::debug("PRESSED: {}", downPress);
  spdlog::debug("UP {}", direction.UP);
  spdlog::debug("DOWN {}", direction.DOWN);
  spdlog::debug("LEFT {}", direction.LEFT);
  spdlog::debug("RIGHT {}", direction.RIGHT);

  switch (key) {
  case sf::Keyboard::Left:
  case sf::Keyboard::A:
    direction.LEFT = (downPress) ? true : false;
    break;
  case sf::Keyboard::Right:
  case sf::Keyboard::D:
    direction.RIGHT = (downPress) ? true : false;
    break;
  case sf::Keyboard::Up:
  case sf::Keyboard::W:
    direction.UP = (downPress) ? true : false;
    break;
  case sf::Keyboard::Down:
  case sf::Keyboard::S:
    direction.DOWN = (downPress) ? true : false;
    break;
  default:
    break;
  }

  mDirMap[5] = direction;
}

void EntityManager::update()
{
  auto &direction = mDirMap[5];

  float deltax = 0.0;
  float deltay = 0.0;
  bool diffed = false;

  if (direction.LEFT) {
    diffed = true;
    deltax += -5;
  }
  if (direction.RIGHT) {
    diffed = true;
    deltax += 5;
  }
  if (direction.UP) {
    diffed = true;
    deltay += -5;
  }
  if (direction.DOWN) {
    diffed = true;
    deltay += 5;
  }

  if (diffed) {
    updatePos(5, deltax, deltay);
  }
}

}// namespace entman