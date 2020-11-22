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
#include "EntityManager/EntityManager.hpp"

#include <random>


namespace clman {


class CoreLogicManager : public RunnableManager
{
private:
  ThreadCom::serviceId_t mServiceId;
  ThreadCom::serviceId_t mGuiManagerId;

  // std::mt19937 mGenerator;
  // std::uniform_real_distribution<> mDistrib;

  std::shared_ptr<ThreadCom::ThreadCommunicator<gman::guiManRequest>> mGuiRequester;

  entman::EntityManager mEntityManager;

  void update();

  void coreLogicHandler(std::unique_ptr<ThreadCom::commMsg> msg);

  void waitForDependencies();

public:
  CoreLogicManager(std::shared_ptr<ThreadCom::ThreadCommunicator<ThreadCom::commMsg>> threadComm, std::shared_ptr<ThreadCom::ThreadCommunicator<gman::guiManRequest>> guiManRequestComm);

  void configGuiManService(const ThreadCom::serviceId_t guiManServiceId);

  void run();
};
}// namespace clman

#endif