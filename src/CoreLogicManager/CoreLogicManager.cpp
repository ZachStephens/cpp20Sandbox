#include "CoreLogicManager.hpp"
#include "GuiManager/Messages/GuiManMessages.hpp"
#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"
#include "EntityManager/EntityManager.hpp"
#include <SFML/Window/Keyboard.hpp>

namespace clman {


CoreLogicManager::CoreLogicManager(std::shared_ptr<ThreadCom::ThreadCommunicator<ThreadCom::commMsg>> threadComm, std::shared_ptr<ThreadCom::ThreadCommunicator<gman::guiManRequest>> guiManRequestComm) : RunnableManager(threadComm), mGuiRequester(guiManRequestComm)
{
  mServiceId = mThreadComm->registerHandler(std::bind(&CoreLogicManager::coreLogicHandler, this, std::placeholders::_1));

  // std::random_device randdev;
  // mGenerator = std::mt19937(randdev());
  // mDistrib = std::uniform_real_distribution<>(-3, 3);
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


void CoreLogicManager::update()
{
  mEntityManager.update();

  const auto &posMap = mEntityManager.mPosMap;

  for (const auto &item : posMap) {
    auto guiRequest = gman::guiManRequest(
      item.first,
      std::get<0>(item.second),
      std::get<1>(item.second),
      false);
    mGuiRequester->ship(mGuiManagerId, guiRequest);
  }
}


void CoreLogicManager::coreLogicHandler(std::unique_ptr<ThreadCom::commMsg> msg)
{
  auto messageBytes = msg.get()->getBytes();

  constexpr uint8_t BUTTON_STATE_IDX = 0;
  //constexpr uint8_t BUTTON_PRESSED_VALUE = 0;

  auto buttonPressed = (messageBytes[BUTTON_STATE_IDX]);


  sf::Keyboard::Key *keyPtr = reinterpret_cast<sf::Keyboard::Key *>(&messageBytes[1]);

  sf::Keyboard::Key key = *keyPtr;


  mEntityManager.processDirectionsMessage(buttonPressed, key);
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

      update();

      start = Clock::now();
    }
  }
}


}// namespace clman