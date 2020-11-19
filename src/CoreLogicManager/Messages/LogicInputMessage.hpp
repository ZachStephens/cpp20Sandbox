#ifndef LOGIC_INPUT_MESSAGE
#define LOGIC_INPUT_MESSAGE

#include "ThreadCommunicator/Messages/ThreadCommMessages.hpp"

#include <vector>

namespace clman {

using keyCode = uint8_t;

class LogicInputMessage : public ThreadCom::commMsg
{
  //std::vector<uint8_t> bytes;
public:
  keyCode mKeyPress;

  LogicInputMessage()
  {}

  LogicInputMessage(
    keyCode code) : ThreadCom::commMsg(std::vector<uint8_t>({ code }))
  {
  }
};

}// namespace clman

#endif