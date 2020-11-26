#ifndef GUI_MANAGER_MESSAGES
#define GUI_MANAGER_MESSAGES

#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"


namespace gman {

using shapeId_t = uint32_t;


class guiManRequest : public ThreadCom::commMsg
{
  //std::vector<uint8_t> bytes;
public:
  shapeId_t mShapeId;
  float mPosX;
  float mPosY;
  bool mIsTransparent;

  guiManRequest() {}

  guiManRequest(
    shapeId_t shapeId,
    float posX,
    float posY,
    bool isTransparent) : mShapeId(shapeId), mPosX(posX), mPosY(posY), mIsTransparent(isTransparent)
  {
  }
};

}// namespace gman

#endif