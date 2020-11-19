#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>


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

  this->mWindow.setFramerateLimit(30);
  ImGui::SFML::Init(this->mWindow);

  mServiceId = mThreadComm->registerHandler(std::bind(&GuiManager::commMsgHandler, this, std::placeholders::_1));
  mGuiRequestHandlerId = mGuiRequester->registerHandler(std::bind(&GuiManager::guiRequestMsgHandler, this, std::placeholders::_1));
}


void GuiManager::updateShape(const shapeId_t shapeId, const sf::Vector2f &requestedPos, const sf::Color requestedColor)
{

  if (mShapeCollection.find(shapeId) == mShapeCollection.end()) {
    mShapeCollection.insert(std::pair<const shapeId_t, std::shared_ptr<sf::Shape>>(shapeId, std::make_shared<sf::CircleShape>(25.f, 5)));
    //mShapeCollection[shapeId]->setPosition(static_cast<float>(mWindow.getSize().x / 2), static_cast<float>(mWindow.getSize().x / 2));
    std::cout
      << "created shape with ID: " << shapeId << "\n";
  } else {

    auto shapeToDraw = mShapeCollection[shapeId];
  }


  mShapeCollection[shapeId]->setPosition(requestedPos);
  mShapeCollection[shapeId]->setFillColor(requestedColor);
}

void GuiManager::commMsgHandler(std::unique_ptr<ThreadCom::commMsg> msg)
{

  msg.get();
}


void GuiManager::guiRequestMsgHandler(std::unique_ptr<guiManRequest> msg)
{

  mRequestQueue.push_back(std::move(msg));
}


void GuiManager::printShape(const std::shared_ptr<sf::Shape> &shapeToPrint)
{

  if (shapeToPrint->getFillColor() == sf::Color::Black) {
    std::cout << "FillColor: "
              << "Black";
  } else {
    std::cout << "FillColor: "
              << "Other";
  }

  std::cout << " at "
            << shapeToPrint->getPosition().x
            << ", "
            << shapeToPrint->getPosition().y
            << "\n";
}


void GuiManager::processGuiManRequest(const std::unique_ptr<guiManRequest> &&request)
{
  sf::Vector2f requestedPos(request->mPosX, request->mPosY);

  // If transparency flag is requested draw shape with same color as background
  auto requestedColor = (request->mIsTransparent) ? sf::Color::Black : sf::Color::Green;

  updateShape(request->mShapeId, requestedPos, requestedColor);

  auto shapeToDraw = mShapeCollection[request->mShapeId];

  //printShape(shapeToDraw);

  this->mWindow.draw(*shapeToDraw);
}

void GuiManager::update()
{

  ImGui::SFML::Update(mWindow, mDeltaClock.restart());

  auto concatted = std::string("Hello, world!, ");
  ImGui::Begin(concatted.c_str());
  ImGui::Button("Look at this pretty button");
  ImGui::End();

  mWindow.clear();
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
    bytesToPass.push_back(0);
    bytesToPass.insert(bytesToPass.end(), &key, &key + sizeof(uint8_t));

    auto logicMsg = ThreadCom::commMsg(bytesToPass);
    mThreadComm->ship(2, logicMsg);
  }
}


void GuiManager::onKeyReleased(const sf::Keyboard::Key key)
{

  if (key == sf::Keyboard::Escape) {
    mWindow.close();
  } else {

    std::vector<uint8_t> bytesToPass;
    bytesToPass.push_back(1);
    bytesToPass.insert(bytesToPass.end(), &key, &key + sizeof(uint8_t));

    auto logicMsg = ThreadCom::commMsg(bytesToPass);
    mThreadComm->ship(2, logicMsg);
  }
}

void GuiManager::run()
{
  //ImGui::SFML::Update(mWindow, mDeltaClock.restart());
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
      if (event.type == sf::Event::KeyPressed) {
        onKeyReleased(event.key.code);
      }
    }

    auto elapsed = mDeltaClock.getElapsedTime().asMilliseconds();
    if (elapsed > 2) {
      //std::cout << "elapsed: " << elapsed << "\n";
      update();
    }
  }

  ImGui::SFML::Shutdown();
  mThreadComm->kill();
}
}// namespace gman