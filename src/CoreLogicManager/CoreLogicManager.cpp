#include "CoreLogicManager.hpp"
#include "GuiManager/Messages/GuiManMessages.hpp"
#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"

#include <ranges>


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


  while (!mThreadComm->killFlag) {

    now = Clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

    waitForDependencies();

    if (duration.count() >= 500) {
      //do stuff every 100 milliseconds
      std::cout << "waiting\n";


      for (const auto &item : mPosMap) {
        auto guiRequest = gman::guiManRequest(
          item.first,
          std::get<0>(item.second),
          std::get<1>(item.second));
        mGuiRequester->ship(mGuiManagerId, guiRequest);
      }

      float xpos, ypos;
      std::tie(xpos, ypos) = mPosMap[5];
      mPosMap[5] = std::tuple<float, float>(xpos + 5, ++ypos + 5);


      start = Clock::now();
    }
  }
}


}// namespace clman