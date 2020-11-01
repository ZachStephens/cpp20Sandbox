#ifndef CORE_LOGIC_MANAGER
#define CORE_LOGIC_MANAGER

#include <iostream>
#include <chrono>
#include <map>

#include <spdlog/spdlog.h>
#include <docopt/docopt.h>

#include "RunnableManager/RunnableManager.hpp"
#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"
#include "GuiManager/Messages/GuiManMessages.hpp"

namespace clman {

using posMapPair_t = std::pair<gman::shapeId_t, std::tuple<float, float>>;

class CoreLogicManager : public RunnableManager
{
private:
  ThreadCom::serviceId_t mServiceId;
  ThreadCom::serviceId_t mGuiManagerId;

  std::map<const gman::shapeId_t, std::tuple<float, float>> mPosMap;

  std::shared_ptr<ThreadCom::ThreadCommunicator<gman::guiManRequest>> mGuiRequester;

  void coreLogicHandler(std::unique_ptr<ThreadCom::commMsg> msg)
  {
    msg.get();
    return;
  }

  void waitForDependencies();

public:
  CoreLogicManager(std::shared_ptr<ThreadCom::ThreadCommunicator<ThreadCom::commMsg>> threadComm, std::shared_ptr<ThreadCom::ThreadCommunicator<gman::guiManRequest>> guiManRequestComm) : RunnableManager(threadComm), mGuiRequester(guiManRequestComm)
  {
    mServiceId = mThreadComm->registerHandler(std::bind(&CoreLogicManager::coreLogicHandler, this, std::placeholders::_1));
    mPosMap.insert(posMapPair_t(5, std::tuple<float, float>(200, 200)));
  };

  void configGuiManService(const ThreadCom::serviceId_t guiManServiceId) { mGuiManagerId = guiManServiceId; }

  void run();
};
}// namespace clman

#endif