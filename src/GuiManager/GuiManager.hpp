#ifndef GUI_MANAGER
#define GUI_MANAGER

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>


#include <memory>
#include <functional>
#include <deque>

#include "Messages/GuiManMessages.hpp"

#include "RunnableManager/RunnableManager.hpp"
#include "ThreadCommunicator/ThreadCommunicator.hpp"

namespace gman {


class GuiManager : public RunnableManager
{

private:
  std::map<shapeId_t, std::shared_ptr<sf::Shape>> mShapeCollection;
  std::deque<std::unique_ptr<guiManRequest>> mRequestQueue;

  std::shared_ptr<ThreadCom::ThreadCommunicator<guiManRequest>> mGuiRequester;
  ThreadCom::serviceId_t mGuiRequestHandlerId;


  sf::Texture mBackgroundTexture;
  sf::Sprite mBackground;
  sf::Clock mDeltaClock;

  sf::RenderWindow mWindow = sf::RenderWindow(sf::VideoMode(1920, 1080), "Test IMGUI Project");

  void processGuiManRequest(const std::unique_ptr<guiManRequest> &&request);


  void updateShape(const shapeId_t shapeId, const sf::Vector2f &requestedPos, const sf::Color requestedColor);

  //Event Handlers
  // When a key is pressed
  void onKeyPressed(const sf::Keyboard::Key code);
  // When a key is released
  void onKeyReleased(const sf::Keyboard::Key key);

  void update();

  //ThreadCom::commHandler_t mHandler;

  void commMsgHandler(std::unique_ptr<ThreadCom::commMsg> msg);

  void guiRequestMsgHandler(std::unique_ptr<guiManRequest> msg);

  //commHandler_t mHandler;

public:
  GuiManager(std::shared_ptr<ThreadCom::ThreadCommunicator<ThreadCom::commMsg>> threadComm, std::shared_ptr<ThreadCom::ThreadCommunicator<guiManRequest>> guiManRequestComm);

  ~GuiManager(){};

  void run();
};

}// namespace gman

#endif