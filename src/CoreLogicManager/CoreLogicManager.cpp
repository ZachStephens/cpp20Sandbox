#include "CoreLogicManager.hpp"
#include "GuiManager/Messages/GuiManMessages.hpp"
#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"

#include <SFML/Window/Keyboard.hpp>

namespace clman {


CoreLogicManager::CoreLogicManager(std::shared_ptr<ThreadCom::ThreadCommunicator<ThreadCom::commMsg>> threadComm, std::shared_ptr<ThreadCom::ThreadCommunicator<gman::guiManRequest>> guiManRequestComm) : RunnableManager(threadComm), mGuiRequester(guiManRequestComm)
{
  mServiceId = mThreadComm->registerHandler(std::bind(&CoreLogicManager::coreLogicHandler, this, std::placeholders::_1));
  mPosMap.insert(posMapPair_t(5, std::tuple<float, float>(600, 600)));

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

  mPosMap[5] = std::tuple<float, float>(xpos + deltaX, ypos + deltaY);
}

void CoreLogicManager::update()
{
  float deltaX = 0;//static_cast<float>(mDistrib(mGenerator));
  float deltaY = 0;//static_cast<float>(mDistrib(mGenerator));
  updatePos(5, deltaX, deltaY);
}


void CoreLogicManager::coreLogicHandler(std::unique_ptr<ThreadCom::commMsg> msg)
{
  auto messageBytes = msg.get()->getBytes();

  constexpr uint8_t BUTTON_STATE_IDX = 0;
  constexpr uint8_t BUTTON_PRESSED_VALUE = 0;

  auto buttonPressed = (messageBytes[BUTTON_STATE_IDX] == BUTTON_PRESSED_VALUE);


  sf::Keyboard::Key *keyPtr = reinterpret_cast<sf::Keyboard::Key *>(&messageBytes[1]);


  auto direction = mDirMap[5];

  std::cout << "UP" << direction.UP << '\n';
  std::cout << "DOWN" << direction.DOWN << '\n';
  std::cout << "LEFT" << direction.LEFT << '\n';
  std::cout << "RIGHT" << direction.RIGHT << '\n';

  sf::Keyboard::Key key = *keyPtr;
  switch (key) {
  case sf::Keyboard::Left:
  case sf::Keyboard::A:
    direction.LEFT = buttonPressed;
    break;
  case sf::Keyboard::Right:
  case sf::Keyboard::D:
    direction.RIGHT = buttonPressed;
    break;
  case sf::Keyboard::Up:
  case sf::Keyboard::W:
    direction.UP = buttonPressed;
    break;
  case sf::Keyboard::Down:
  case sf::Keyboard::S:
    direction.DOWN = buttonPressed;
    break;
  default:

    break;
  }

  if (direction.LEFT) {
    updatePos(5, -20, 0);
  }
  if (direction.RIGHT) {
    updatePos(5, 20, 0);
  }
  if (direction.UP) {
    updatePos(5, 0, -20);
  }
  if (direction.DOWN) {
    updatePos(5, 0, 20);
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

    if (duration.count() >= 25) {
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