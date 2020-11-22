#include <functional>
#include <iostream>

#include <spdlog/spdlog.h>
#include <docopt/docopt.h>


#include "GuiManager/GuiManager.hpp"
#include "CoreLogicManager/CoreLogicManager.hpp"
#include "ThreadCommunicator/ThreadCommunicator.hpp"
#include "GuiManager/Messages/GuiManMessages.hpp"

#include <thread>
#include <memory>
#include <functional>


int main()
{
  //Use the default logger (stdout, multi-threaded, colored)


  // Instantiate communication object needed to pass messages between gui and core logic
  auto commMsgConnector = std::make_shared<ThreadCom::ThreadCommunicator<ThreadCom::commMsg>>();
  auto guiManRequestConnector = std::make_shared<ThreadCom::ThreadCommunicator<gman::guiManRequest>>();

  // Instantiate managers for gui and core logic
  auto guiMan = std::make_unique<gman::GuiManager>(commMsgConnector, guiManRequestConnector);
  auto coreLogicMan = std::make_unique<clman::CoreLogicManager>(commMsgConnector, guiManRequestConnector);

  coreLogicMan->configGuiManService(guiMan->mServiceId);

  spdlog::set_level(spdlog::level::debug);

  std::jthread threadGui([guiMan = move(guiMan)]() {
    guiMan->run();
  });

  std::jthread threadCoreLogic([coreLogicMan = move(coreLogicMan)]() {
    coreLogicMan->run();
  });

  fmt::print("Hell000000o, from {}\n", "{fmt}");
  spdlog::info("Hello, {}!", "World");
}
