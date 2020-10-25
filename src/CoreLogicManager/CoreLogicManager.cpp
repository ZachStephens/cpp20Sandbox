#include "CoreLogicManager.hpp"
#include "GuiManager/Messages/GuiManMessages.hpp"
#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"

namespace clman {


void CoreLogicManager::waitForDependencies()
{
  ThreadCom::pingMsg pMsg(mGuiManagerId, mServiceId);
  mThreadComm->ship(pMsg);
}

void CoreLogicManager::run()
{
  using Clock = std::chrono::steady_clock;
  std::chrono::time_point<std::chrono::steady_clock> start, now;
  std::chrono::milliseconds duration;

  start = Clock::now();


  while (true) {

    now = Clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

    waitForDependencies();

    if (duration.count() >= 500) {
      //do stuff every 100 milliseconds
      std::cout << "waiting\n";

      auto guiRequest = gman::guiManRequest(
        5,
        200,
        200);

      mThreadComm->ship(mGuiManagerId, guiRequest);


      start = Clock::now();
    }
  }
}


}// namespace clman