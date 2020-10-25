#ifndef CORE_LOGIC_MANAGER
#define CORE_LOGIC_MANAGER

#include <iostream>
#include <chrono>


#include <spdlog/spdlog.h>
#include <docopt/docopt.h>

#include "RunnableManager/RunnableManager.hpp"
#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"
#include "GuiManager/Messages/GuiManMessages.hpp"

namespace clman {


class CoreLogicManager : public RunnableManager
{
private:
  ThreadCom::serviceId_t mServiceId;
  ThreadCom::serviceId_t mGuiManagerId;
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
  };

  void configGuiManService(const ThreadCom::serviceId_t guiManServiceId) { mGuiManagerId = guiManServiceId; }

  void run();
};
}// namespace clman

#endif