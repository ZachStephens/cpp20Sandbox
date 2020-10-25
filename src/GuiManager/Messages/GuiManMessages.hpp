#ifndef GUI_MANAGER_MESSAGES
#define GUI_MANAGER_MESSAGES

#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"


// constexpr auto GM_REQUEST_SHAPE_ID_IDX = 0;
// constexpr auto GM_REQUEST_POSX_IDX = sizeof(shapeId_t);
// constexpr auto GM_REQUEST_POSY_IDX = GM_REQUEST_POSX_IDX + sizeof(float);
// constexpr auto GM_REQUEST_SIZE = GM_REQUEST_POSY_IDX + sizeof(float);

namespace gman {

using shapeId_t = uint32_t;


class guiManRequest : public ThreadCom::commMsg
{
  //std::vector<uint8_t> bytes;
public:
  shapeId_t mShapeId;
  float mPosX;
  float mPosY;

  guiManRequest() {}

  guiManRequest(
    shapeId_t shapeId,
    float posX,
    float posY) : mShapeId(shapeId), mPosX(posX), mPosY(posY)
  {
  }
};

}// namespace gman

#endif