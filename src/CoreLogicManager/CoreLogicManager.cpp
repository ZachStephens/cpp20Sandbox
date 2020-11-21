#include "CoreLogicManager.hpp"
#include "GuiManager/Messages/GuiManMessages.hpp"
#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"

#include <SFML/Window/Keyboard.hpp>

namespace clman {


constexpr auto MAX_X_MAG = 1920 - 50;
constexpr auto MAX_Y_MAG = 1080 - 50;

CoreLogicManager::CoreLogicManager(std::shared_ptr<ThreadCom::ThreadCommunicator<ThreadCom::commMsg>> threadComm, std::shared_ptr<ThreadCom::ThreadCommunicator<gman::guiManRequest>> guiManRequestComm) : RunnableManager(threadComm),
                                                                                                                                                                                                           mGuiRequester(guiManRequestComm)
{
  mServiceId = mThreadComm->registerHandler(std::bind(&CoreLogicManager::coreLogicHandler, this, std::placeholders::_1));
  mPosMap.insert(posMapPair_t(5, std::tuple<float, float>(600, 600)));
  mDirMap.insert(std::pair<gman::shapeId_t, DIRECTIONS_STATE>(5, DIRECTIONS_STATE()));

  std::random_device randdev;
  mGenerator = std::mt19937(randdev());
  mDistrib = std::uniform_real_distribution<>(-3, 3);
}

void CoreLogicManager::configGuiManService(const ThreadCom::serviceId_t guiManServiceId)
{
  mGuiManagerId = guiManServiceId;
}

void CoreLogicManager::waitForDependencies()
{
  ThreadCom::pingMsg pMsg(mGuiManagerId, mServiceId);
  mThreadComm->ship(pMsg);
}


void CoreLogicManager::updatePos(const gman::shapeId_t id, const float deltaX, const float deltaY)
{
  float xpos, ypos;
  std::tie(xpos, ypos) = mPosMap[id];

  auto newXpos = xpos + deltaX;
  auto newYpos = ypos + deltaY;

  if (newXpos >= MAX_X_MAG) {
    newXpos = MAX_X_MAG;
  } else if (newXpos <= 0) {
    newXpos = 0;
  }

  if (newYpos >= MAX_Y_MAG) {
    newYpos = MAX_Y_MAG;
  } else if (newYpos <= 0) {
    newYpos = 0;
  }

  mPosMap[5] = std::tuple<float, float>(newXpos, newYpos);
}

void CoreLogicManager::update()
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

  if (diffed) updatePos(5, deltax, deltay);
}


void CoreLogicManager::coreLogicHandler(std::unique_ptr<ThreadCom::commMsg> msg)
{
  auto messageBytes = msg.get()->getBytes();

  constexpr uint8_t BUTTON_STATE_IDX = 0;
  //constexpr uint8_t BUTTON_PRESSED_VALUE = 0;

  auto buttonPressed = (messageBytes[BUTTON_STATE_IDX]);


  sf::Keyboard::Key *keyPtr = reinterpret_cast<sf::Keyboard::Key *>(&messageBytes[1]);

  auto &direction = mDirMap[5];

  std::cout << "PRESSED: " << buttonPressed << '\n';
  std::cout << "UP" << direction.UP << '\n';
  std::cout << "DOWN" << direction.DOWN << '\n';
  std::cout << "LEFT" << direction.LEFT << '\n';
  std::cout << "RIGHT" << direction.RIGHT << '\n';

  sf::Keyboard::Key key = *keyPtr;
  switch (key) {
  case sf::Keyboard::Left:
  case sf::Keyboard::A:

    direction.LEFT = (buttonPressed) ? true : false;
    break;
  case sf::Keyboard::Right:
  case sf::Keyboard::D:
    direction.RIGHT = (buttonPressed) ? true : false;
    break;
  case sf::Keyboard::Up:
  case sf::Keyboard::W:
    direction.UP = (buttonPressed) ? true : false;
    break;
  case sf::Keyboard::Down:
  case sf::Keyboard::S:
    direction.DOWN = (buttonPressed) ? true : false;
    break;
  default:
    break;
  }

  mDirMap[5] = direction;

  return;
}


void CoreLogicManager::run()
{
  using Clock = std::chrono::steady_clock;
  std::chrono::time_point<std::chrono::steady_clock> start, now;
  std::chrono::milliseconds duration;

  start = Clock::now();


  while (!mThreadComm->killFlag) {

    now = Clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

    if (duration.count() >= 5) {
      //do stuff every 100 milliseconds
      //std::cout << "waiting\n";

      //Erase
      // for (const auto &item : mPosMap) {
      //   auto guiRequest = gman::guiManRequest(
      //     item.first,
      //     std::get<0>(item.second),
      //     std::get<1>(item.second),
      //     true);
      //   mGuiRequester->ship(mGuiManagerId, guiRequest);
      // }

      update();

      for (const auto &item : mPosMap) {
        auto guiRequest = gman::guiManRequest(
          item.first,
          std::get<0>(item.second),
          std::get<1>(item.second),
          false);
        mGuiRequester->ship(mGuiManagerId, guiRequest);
      }

      start = Clock::now();
    }
  }
}


}// namespace clman