#ifndef GUI_MANAGER_MESSAGES
#define GUI_MANAGER_MESSAGES

#include <SFML/Graphics/Shape.hpp>

#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"


namespace gman {

using shapeId_t = uint32_t;

enum GUI_COMMAND {
  INIT,
  READ,
  WRITE
};

class guiManRequest : public ThreadCom::commMsg
{
  //std::vector<uint8_t> bytes;
public:
  GUI_COMMAND mCmd;
  std::shared_ptr<const sf::Shape> mShape;

  // guiManRequest() {}
  guiManRequest(
    GUI_COMMAND cmd,
    std::shared_ptr<const sf::Shape> &shape) : mCmd(cmd), mShape(shape)
  {
  }
};


// class guiManResponse : public ThreadCom::commMsg
// {
//   //std::vector<uint8_t> bytes;
// public:
//   shapeId_t mShapeId;

//   guiManResponse() {}

//   guiManResponse(
//     shapeId_t shapeId,
//     float posX,
//     float posY,
//     bool isTransparent) : mShapeId(shapeId), mPosX(posX), mPosY(posY), mIsTransparent(isTransparent)
//   {
//   }
// };


}// namespace gman

#endif