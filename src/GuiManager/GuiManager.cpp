#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>


#include <spdlog/spdlog.h>

#include "GuiManager.hpp"
#include "../ThreadCommunicator/ThreadCommunicator.hpp"
#include "CoreLogicManager/Messages/LogicInputMessage.hpp"

#include <iostream>
#include <map>
#include <algorithm>
#include <functional>
#include <memory>

namespace gman {


GuiManager::GuiManager(std::shared_ptr<ThreadCom::ThreadCommunicator<ThreadCom::commMsg>> threadComm,
  std::shared_ptr<ThreadCom::ThreadCommunicator<gman::guiManRequest>> guiManRequestComm) : RunnableManager(std::move(threadComm)), mGuiRequester(std::move(guiManRequestComm))
{
  const auto FRAME_RATE_LIMIT = 90;
  this->mWindow.setFramerateLimit(FRAME_RATE_LIMIT);
  ImGui::SFML::Init(this->mWindow);

  this->mWindow.setActive(false);


  sf::Image image;

  image.loadFromFile("/home/zach/Pictures/bison.jpeg");

  mBackgroundTexture.loadFromImage(image);

  auto TextureSize = mBackgroundTexture.getSize();//Get size of texture.
  auto WindowSize = mWindow.getSize();//Get size of window.

  auto ScaleX = static_cast<float>(WindowSize.x) / static_cast<float>(TextureSize.x);
  auto ScaleY = static_cast<float>(WindowSize.y) / static_cast<float>(TextureSize.y);//Calculate scale.

  mBackground.setTexture(mBackgroundTexture);
  mBackground.setScale(ScaleX, ScaleY);//Set scale.


  mServiceId = mThreadComm->registerHandler(this->commMsgHandler);
  mGuiRequestHandlerId = mGuiRequester->registerHandler(this->guiRequestMsgHandler);
}


// void GuiManager::guiRequestMsgHandler(std::unique_ptr<guiManRequest> msg)
// {
//   // mRequestQueue.push_back(std::move(msg));
//   auto shapeToDraw = msg->mShape;
//   this->mWindow.draw(*shapeToDraw);
// }


// void GuiManager::processGuiManRequest(std::unique_ptr<std::vector<guiManRequest>> requestVec)
// {
//   spdlog::set_level(spdlog::level::info);
//   spdlog::debug("processGuiManRequest start");
//   if (requestVec) {
//     mRequestVecPending.swap(requestVec);
//   } else {
//     mRequestVecPending->clear();
//   }

//   spdlog::debug("processGuiManRequest requestVec Moved");
//   spdlog::set_level(spdlog::level::info);
// }

void GuiManager::update()
{
  spdlog::set_level(spdlog::level::info);

  ImGui::SFML::Update(mWindow, mDeltaClock.restart());

  // Requests of other entities to be drawn are handled as they appear
  spdlog::debug("Check if pending empty");
  if (mRequestVecPending && !mRequestVecPending->empty()) {
    spdlog::debug("Attempt Swap");
    mRequestVec.swap(mRequestVecPending);
    spdlog::debug("Move Completed");
  }

  if (mRequestVec && !mRequestVec->empty()) {
    spdlog::debug("Attempt draw");
    mWindow.clear();
    mWindow.draw(mBackground);
    for (const auto &request : *mRequestVec) {
      auto shapeToDraw = request.mShape;
      mWindow.draw(*shapeToDraw);
    }
    spdlog::debug("Attempt clear");
    mRequestVec->clear();
    spdlog::debug("mRequestVec cleared");
  }

  spdlog::set_level(spdlog::level::info);
  // while (!mRequestVec.empty()) {
  //   processGuiManRequest(std::move(mRequestQueue.front()));
  //   mRequestVec.pop_front();
  // }
  auto concatted = std::string("Hello, world!, ");
  ImGui::Begin(concatted.c_str());
  ImGui::Button("Look at this pretty button");
  ImGui::End();

  // mWindow.clear();
}


void GuiManager::onKeyPressed(const sf::Keyboard::Key key)
{

  if (key == sf::Keyboard::Escape) {
    mWindow.close();
  } else {

    std::vector<uint8_t> bytesToPass;
    bytesToPass.push_back(1);
    const auto keyAsByte = static_cast<uint8_t>(key);
    spdlog::debug("keyAsByte: {}", keyAsByte);
    bytesToPass.push_back(keyAsByte);

    auto logicMsg = ThreadCom::commMsg(bytesToPass);
    spdlog::debug("Is pressed {}", key);
    mThreadComm->ship(2, logicMsg);
  }
}


void GuiManager::onKeyReleased(const sf::Keyboard::Key key)
{
  if (key == sf::Keyboard::Escape) {
    mWindow.close();
  } else {

    std::vector<uint8_t> bytesToPass;
    bytesToPass.push_back(0);
    const auto keyAsByte = static_cast<uint8_t>(key);
    spdlog::debug("keyAsByte: {}", keyAsByte);
    bytesToPass.push_back(keyAsByte);

    auto logicMsg = ThreadCom::commMsg(bytesToPass);
    spdlog::debug("Is released {}", key);
    mThreadComm->ship(2, logicMsg);
  }
}

void GuiManager::run()
{
  //ImGui::SFML::Update(mWindow, mDeltaClock.restart());

  // Running in new thread.  Any other threads must set window to inactive
  mWindow.setActive(true);

  mWindow.setKeyRepeatEnabled(false);

  while (mWindow.isOpen()) {
    spdlog::set_level(spdlog::level::debug);
    sf::Event event{};
    while (mWindow.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        mWindow.close();
      }
      if (event.type == sf::Event::KeyPressed) {
        onKeyPressed(event.key.code);//NOLINT
      }
      if (event.type == sf::Event::KeyReleased) {
        onKeyReleased(event.key.code);//NOLINT
      }
    }


    // clear at regular intervals
    auto elapsed = mDeltaClock.getElapsedTime().asMilliseconds();
    if (elapsed > 2) {

      update();
      ImGui::SFML::Render(mWindow);
      mWindow.display();
    }
  }

  ImGui::SFML::Shutdown();
  mThreadComm->kill();
}
}// namespace gman