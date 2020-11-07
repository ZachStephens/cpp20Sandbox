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

  this->mWindow.setFramerateLimit(30);
  ImGui::SFML::Init(this->mWindow);

  mServiceId = mThreadComm->registerHandler(std::bind(&GuiManager::commMsgHandler, this, std::placeholders::_1));
  mGuiRequestHandlerId = mGuiRequester->registerHandler(std::bind(&GuiManager::guiRequestMsgHandler, this, std::placeholders::_1));
}


void GuiManager::updateShape(const shapeId_t shapeId, const sf::Vector2f &requestedPos, bool isTransparent)
{

  if (mShapeCollection.find(shapeId) == mShapeCollection.end()) {
    mShapeCollection.insert(std::pair<const shapeId_t, std::shared_ptr<sf::Shape>>(shapeId, std::make_shared<sf::CircleShape>(15.f, 5)));
    mShapeCollection[shapeId]->setFillColor(sf::Color::Blue);
    mShapeCollection[shapeId]->setPosition(static_cast<float>(mWindow.getSize().x / 2), static_cast<float>(mWindow.getSize().x / 2));
    std::cout
      << "created shape with ID: " << shapeId << "\n";
  } else {


    //auto original_color = mShapeCollection[shapeId]->getFillColor();
    auto shapeToDraw = mShapeCollection[shapeId];
    if (isTransparent) {
      std::cout << "is transparent\n";
      shapeToDraw->setFillColor(sf::Color::Transparent);
    } else {
      shapeToDraw->setFillColor(sf::Color::Blue);
    }


    mShapeCollection[shapeId]->setPosition(requestedPos);
  }
}

void GuiManager::commMsgHandler(std::unique_ptr<ThreadCom::commMsg> msg)
{

  msg.get();
}


void GuiManager::guiRequestMsgHandler(std::unique_ptr<guiManRequest> msg)
{

  //std::cout << "GuiManager.cpp::GuiManRequest Handler\n";


  mRequestQueue.push_back(std::move(msg));
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


      this->updateShape(request->mShapeId, requestedPos, request->mIsTransparent);

      auto shapeToDraw = mShapeCollection[request->mShapeId];


      if (shapeToDraw->getFillColor() == sf::Color::Transparent) {
        std::cout << "setFillColor: "
                  << "Transparent";
      } else {
        std::cout << "setFillColor: "
                  << "Other";
      }
      std::cout << " at "
                << shapeToDraw->getPosition().x
                << ", "
                << shapeToDraw->getPosition().y
                << "\n";
      this->mWindow.draw(*shapeToDraw);
    });


  ImGui::SFML::Render(mWindow);
  mWindow.display();
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
        switch (event.key.code) {
        case sf::Keyboard::Escape:
          mWindow.close();
          break;
        case sf::Keyboard::Left:
        case sf::Keyboard::A:
          std::cout << "Left\n";
          break;
        case sf::Keyboard::Right:
        case sf::Keyboard::D:
          std::cout << "Right\n";
          break;
        case sf::Keyboard::Up:
        case sf::Keyboard::W:
          std::cout << "Up\n";
          break;
        case sf::Keyboard::Down:
        case sf::Keyboard::S:
          std::cout << "Down\n";
          break;
        default:
          break;
        }
      }
    }

    auto elapsed = mDeltaClock.getElapsedTime().asMilliseconds();
    if (elapsed > 150) {
      //std::cout << "elapsed: " << elapsed << "\n";
      update();
    }
  }

  ImGui::SFML::Shutdown();
  mThreadComm->kill();
}
}// namespace gman