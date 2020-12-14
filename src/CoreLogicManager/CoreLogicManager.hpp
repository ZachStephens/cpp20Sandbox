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

//#include <random>
//#include <deque>

namespace clman {


class CoreLogicManager : public RunnableManager
{
private:
  ThreadCom::serviceId_t mServiceId;
  ThreadCom::serviceId_t mGuiManagerId;

  std::shared_ptr<ThreadCom::ThreadCommunicator<gman::guiManRequest>> mGuiRequester;

  ent::man::EntityManager mEntityManager;


  void readMessages(const std::unique_ptr<ThreadCom::commMsg> &&msg);

  void coreLogicHandler(std::unique_ptr<ThreadCom::commMsg> msg);

  void waitForDependencies();

  void update();

public:
  CoreLogicManager(std::shared_ptr<ThreadCom::ThreadCommunicator<ThreadCom::commMsg>> threadComm, std::shared_ptr<ThreadCom::ThreadCommunicator<gman::guiManRequest>> guiManRequestComm);

  void configGuiManService(const ThreadCom::serviceId_t guiManServiceId);

  void run();
};
}// namespace clman

#endif