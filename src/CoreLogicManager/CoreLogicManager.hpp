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


#include <random>


namespace clman {

using posMapPair_t = std::pair<gman::shapeId_t, std::tuple<float, float>>;

class CoreLogicManager : public RunnableManager
{
private:
  ThreadCom::serviceId_t mServiceId;
  ThreadCom::serviceId_t mGuiManagerId;


  std::mt19937 mGenerator;
  std::uniform_real_distribution<> mDistrib;

  struct DIRECTIONS_STATE
  {
    bool LEFT = false;
    bool RIGHT = false;
    bool UP = false;
    bool DOWN = false;
  };


  std::map<const gman::shapeId_t, std::tuple<float, float>> mPosMap;
  std::map<const gman::shapeId_t, DIRECTIONS_STATE> mDirMap;
  //std::map<const gman::shapeId_t, bool> > mColMap;

  void updatePos(const gman::shapeId_t id, const float deltaX, const float deltaY);


  std::shared_ptr<ThreadCom::ThreadCommunicator<gman::guiManRequest>> mGuiRequester;

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