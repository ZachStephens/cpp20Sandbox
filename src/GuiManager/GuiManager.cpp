#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <iostream>
#include <map>

#include "GuiManager.hpp"
#include "../ThreadCommunicator/ThreadCommunicator.hpp"

#include <algorithm>
#include <functional>
#include <memory>

namespace gman {

GuiManager::GuiManager(std::shared_ptr<ThreadCom::ThreadCommunicator<ThreadCom::commMsg>> threadComm,
  std::shared_ptr<ThreadCom::ThreadCommunicator<gman::guiManRequest>> guiManRequestComm) : RunnableManager(threadComm), mGuiRequester(guiManRequestComm)
{

  this->mWindow.setFramerateLimit(45);
  ImGui::SFML::Init(this->mWindow);

  mServiceId = mThreadComm->registerHandler(std::bind(&GuiManager::commMsgHandler, this, std::placeholders::_1));
  mGuiRequestHandlerId = mGuiRequester->registerHandler(std::bind(&GuiManager::guiRequestMsgHandler, this, std::placeholders::_1));
}


void GuiManager::updateShape(const shapeId_t shapeId, const sf::Vector2f &requestedPos)
{

  if (mShapeCollection.find(shapeId) == mShapeCollection.end()) {
    mShapeCollection.insert(std::pair<const shapeId_t, std::shared_ptr<sf::Shape>>(shapeId, std::make_shared<sf::CircleShape>(100.f)));
    mShapeCollection[shapeId]->setFillColor(sf::Color::Green);
    std::cout << "created shape with ID: " << shapeId << "\n";
  } else {
    mShapeCollection[shapeId]->setPosition(requestedPos);
  }
}

void GuiManager::commMsgHandler(std::unique_ptr<ThreadCom::commMsg> msg)
{

  //ThreadCom::commMsg cmsg(*(move(msg)));

  //guiManRequest *gmsg = reinterpret_cast<guiManRequest *>(msg.get());

  msg.get();


  // mRequestQueue.push_back(std::unique_ptr<guiManRequest>(gmsg));

  // //std::unique_ptr<guiManRequest> gmsg() = ;

  // std::cout
  //   << mRequestQueue->back()->mShapeId << std::endl;
  // std::cout << mRequestQueue->back()->mPosX << std::endl;
  // std::cout << mRequestQueue->back()->mPosY << std::endl;
}


void GuiManager::guiRequestMsgHandler(std::unique_ptr<guiManRequest> msg)
{

  //ThreadCom::commMsg cmsg(*(move(msg)));

  //guiManRequest *gmsg = reinterpret_cast<guiManRequest *>(msg.get());

  std::cout << "GuiManager.cpp::GuiManRequest Handler\n";


  mRequestQueue.push_back(std::move(msg));

  //std::unique_ptr<guiManRequest> gmsg() = ;

  std::cout
    << mRequestQueue.back()->mShapeId << std::endl;
  std::cout << mRequestQueue.back()->mPosX << std::endl;
  std::cout << mRequestQueue.back()->mPosY << std::endl;
}


void GuiManager::update()
{


  ImGui::SFML::Update(mWindow, mDeltaClock.restart());

  auto concatted = std::string("Hello, world!, ");

  ImGui::Begin(concatted.c_str());
  ImGui::Button("Look at this pretty button");
  ImGui::End();


  mWindow.clear();


  std::for_each(mRequestQueue.begin(),
    mRequestQueue.end(),
    [this](const auto &request) {
      sf::Vector2f requestedPos(request->mPosX, request->mPosY);
      this->updateShape(request->mShapeId, requestedPos);

      auto shapeToDraw = mShapeCollection[request->mShapeId];

      this->mWindow.draw(*shapeToDraw);
    });


  ImGui::SFML::Render(mWindow);
  mWindow.display();
}


void GuiManager::run()
{
  while (mWindow.isOpen()) {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        mWindow.close();
      }
    }
    update();
  }

  ImGui::SFML::Shutdown();
  mThreadComm->kill();
}
}// namespace gman