#ifndef THREAD_COMMUNICATOR_MESSAGES
#define THREAD_COMMUNICATOR_MESSAGES

namespace ThreadCom {

using serviceId_t = unsigned int;

enum Enum_CommRetVal {
  SUCCESS,
  FAIL
};


class commMsg
{
public:
  commMsg() {}

protected:
  std::vector<uint8_t> mBytes;
};

template<typename MSG_T>
using commHandler_t = std::function<void(std::unique_ptr<MSG_T>)>;

class pingMsg : public commMsg
{
public:
  const serviceId_t mSrcId;
  const serviceId_t mDestId;
  pingMsg(const serviceId_t src, const serviceId_t dest) : mSrcId(src), mDestId(dest) {}
};

}// namespace ThreadCom
#endif