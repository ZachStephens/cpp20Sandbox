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
  std::shared_ptr<ThreadCom::ThreadCommunicator<gman::guiManRequest>> guiManRequestComm) : RunnableManager(threadComm), mGuiRequester(guiManRequestComm)
{

  this->mWindow.setFramerateLimit(15);
  ImGui::SFML::Init(this->mWindow);

  sf::Image image;

  image.loadFromFile("/home/zach/Pictures/bison.jpeg");

  mBackgroundTexture.loadFromImage(image);

  auto TextureSize = mBackgroundTexture.getSize();//Get size of texture.
  auto WindowSize = mWindow.getSize();//Get size of window.

  auto ScaleX = static_cast<float>(WindowSize.x) / static_cast<float>(TextureSize.x);
  auto ScaleY = static_cast<float>(WindowSize.y) / static_cast<float>(TextureSize.y);//Calculate scale.

  mBackground.setTexture(mBackgroundTexture);
  mBackground.setScale(ScaleX, ScaleY);//Set scale.


  mServiceId = mThreadComm->registerHandler(std::bind(&GuiManager::commMsgHandler, this, std::placeholders::_1));
  mGuiRequestHandlerId = mGuiRequester->registerHandler(std::bind(&GuiManager::guiRequestMsgHandler, this, std::placeholders::_1));
}


void GuiManager::commMsgHandler(std::unique_ptr<ThreadCom::commMsg> msg)
{
  msg.get();
}


void GuiManager::guiRequestMsgHandler(std::unique_ptr<guiManRequest> msg)
{
  mRequestQueue.push_back(std::move(msg));
}


void GuiManager::processGuiManRequest(const std::unique_ptr<guiManRequest> &&request)
{
  auto shapeToDraw = request->mShape;
  this->mWindow.draw(*shapeToDraw);
}

void GuiManager::update()
{
  mWindow.clear();
  mWindow.draw(mBackground);
  ImGui::SFML::Update(mWindow, mDeltaClock.restart());

  auto concatted = std::string("Hello, world!, ");
  ImGui::Begin(concatted.c_str());
  ImGui::Button("Look at this pretty button");
  ImGui::End();

  //mWindow.clear();

  while (!mRequestQueue.empty()) {
    processGuiManRequest(std::move(mRequestQueue.front()));
    mRequestQueue.pop_front();
  }


  ImGui::SFML::Render(mWindow);
  mWindow.display();
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
  mWindow.setKeyRepeatEnabled(false);
  mWindow.draw(mBackground);
  while (mWindow.isOpen()) {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        mWindow.close();
      }
      if (event.type == sf::Event::KeyPressed) {
        onKeyPressed(event.key.code);
      }
      if (event.type == sf::Event::KeyReleased) {
        onKeyReleased(event.key.code);
      }
    }

    //auto elapsed = mDeltaClock.getElapsedTime().asMilliseconds();
    //if (elapsed > 2) {
    update();
    //}
  }

  ImGui::SFML::Shutdown();
  mThreadComm->kill();
}
}// namespace gman