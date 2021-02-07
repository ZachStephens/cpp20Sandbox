#include "CoreLogicManager.hpp"
#include "GuiManager/Messages/GuiManMessages.hpp"
#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"
#include "EntityManager/EntityManager.hpp"
#include <SFML/Window/Keyboard.hpp>


namespace clman {


CoreLogicManager::CoreLogicManager(std::shared_ptr<ThreadCom::ThreadCommunicator<ThreadCom::commMsg>> threadComm, std::shared_ptr<ThreadCom::ThreadCommunicator<gman::guiManRequest>> guiManRequestComm) : RunnableManager(threadComm), mGuiRequester(guiManRequestComm)
{
  mServiceId = mThreadComm->registerHandler(std::bind(&CoreLogicManager::coreLogicHandler, this, std::placeholders::_1));
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


void CoreLogicManager::readMessages(const std::unique_ptr<ThreadCom::commMsg> &&msg)
{
  auto messageBytes = msg.get()->getBytes();

  if (messageBytes.size() < 2) {
    return;// error
  }

  auto buttonPressed = (messageBytes[0]);
  auto key = static_cast<sf::Keyboard::Key>(messageBytes[1]);

  if (key == sf::Keyboard::F5) {
    mEntityManager.reset();
    return;
  }

  mEntityManager.processVelocityMessage(buttonPressed, key);
}

void CoreLogicManager::coreLogicHandler(std::unique_ptr<ThreadCom::commMsg> msg)
{
  mRequestQueue.push_back(std::move(msg));
}

void CoreLogicManager::update()
{

  while (!mRequestQueue.empty()) {
    readMessages(std::move(mRequestQueue.front()));
    mRequestQueue.pop_front();
  }

  mEntityManager.update();

  auto shapesToWrite = mEntityManager.getShapesToWrite();
  std::vector<gman::guiManRequest> guiRequestVec;
  while (!shapesToWrite->empty()) {
    std::shared_ptr<const sf::Shape> shapeToSend(shapesToWrite->back());

    shapesToWrite->pop_back();
    auto guiRequest = gman::guiManRequest(gman::GUI_COMMAND::WRITE, shapeToSend);
    guiRequestVec.push_back(guiRequest);
  }
  mGuiRequester->shipVec(mGuiManagerId, guiRequestVec);
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

    if (duration.count() >= 10) {

      update();

      start = Clock::now();
    }
  }
}


}// namespace clman