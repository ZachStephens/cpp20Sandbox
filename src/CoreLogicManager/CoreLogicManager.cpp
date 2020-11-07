#include "CoreLogicManager.hpp"
#include "GuiManager/Messages/GuiManMessages.hpp"
#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"


namespace clman {


CoreLogicManager::CoreLogicManager(std::shared_ptr<ThreadCom::ThreadCommunicator<ThreadCom::commMsg>> threadComm, std::shared_ptr<ThreadCom::ThreadCommunicator<gman::guiManRequest>> guiManRequestComm) : RunnableManager(threadComm), mGuiRequester(guiManRequestComm)
{
  mServiceId = mThreadComm->registerHandler(std::bind(&CoreLogicManager::coreLogicHandler, this, std::placeholders::_1));
  mPosMap.insert(posMapPair_t(5, std::tuple<float, float>(600, 600)));

  std::random_device randdev;
  mGenerator = std::mt19937(randdev());
  mDistrib = std::uniform_real_distribution<>(-10, 10);
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
  float xpos, ypos;
  std::tie(xpos, ypos) = mPosMap[5];


  float deltaX = static_cast<float>(mDistrib(mGenerator));
  float deltaY = static_cast<float>(mDistrib(mGenerator));
  mPosMap[5] = std::tuple<float, float>(xpos + deltaX, ypos + deltaY);
}


void CoreLogicManager::coreLogicHandler(std::unique_ptr<ThreadCom::commMsg> msg)
{
  msg.get();
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

    waitForDependencies();

    if (duration.count() >= 1) {
      //do stuff every 100 milliseconds
      //std::cout << "waiting\n";

      update();

      for (const auto &item : mPosMap) {
        auto guiRequest = gman::guiManRequest(
          item.first,
          std::get<0>(item.second),
          std::get<1>(item.second));
        mGuiRequester->ship(mGuiManagerId, guiRequest);
      }

      start = Clock::now();
    }
  }
}


}// namespace clman