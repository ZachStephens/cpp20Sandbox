#ifndef THREAD_COMMUNICATOR
#define THREAD_COMMUNICATOR

#include <map>
#include <queue>
#include <functional>
#include <memory>
#include <iostream>
#include <vector>

#include "Messages/ThreadCommMessages.hpp"

namespace ThreadCom {

template<class MSG_T>
class ThreadCommunicator
{

private:
  struct serviceInfo
  {
    //std::queue<std::unique_ptr<const MSG_T>> msgQueue;

    //TODO: change to function pointer to service msg handler

    commHandler_t<MSG_T> msgHandler;
  };

  struct serviceInfoVec
  {
    //std::queue<std::unique_ptr<const MSG_T>> msgQueue;

    //TODO: change to function pointer to service msg handler

    commHandler_t<std::vector<MSG_T>> msgHandler;
  };

  std::map<const serviceId_t, std::unique_ptr<serviceInfo>> mMsgMap;

  std::map<const serviceId_t, std::unique_ptr<serviceInfoVec>> mMsgVecMap;
  //void react();

public:
  bool killFlag = false;


  void kill() { killFlag = true; }

  Enum_CommRetVal ship(const pingMsg &pMsg)
  {
    if (mMsgMap.find(pMsg.mDestId) == mMsgMap.end()) {
      return Enum_CommRetVal::FAIL;
    }
    auto handler = mMsgMap[pMsg.mDestId]->msgHandler;
    handler(std::make_unique<MSG_T>(pMsg));
    return Enum_CommRetVal::SUCCESS;
  }

  Enum_CommRetVal ship(const serviceId_t sId, const MSG_T &cMsg)
  {


    if (mMsgMap.find(sId) == mMsgMap.end()) {
      std::cout << "\nThreadCommuncator::ship: Unable to ship to handler# " << sId << "\n";
      return Enum_CommRetVal::FAIL;
    }
    auto handler = mMsgMap[sId]->msgHandler;

    handler(std::make_unique<MSG_T>(cMsg));
    return Enum_CommRetVal::SUCCESS;
  }

  Enum_CommRetVal shipVec(const serviceId_t sId, std::vector<MSG_T> &cMsgVec)
  {
    if (cMsgVec.empty()) {
      return Enum_CommRetVal::SUCCESS;
    }
    if (mMsgVecMap.find(sId) == mMsgVecMap.end()) {
      std::cout << "\nThreadCommuncator::shipVec: Unable to ship to handler# " << sId << "\n";
      return Enum_CommRetVal::FAIL;
    }

    auto handler = mMsgVecMap[sId]->msgHandler;

    handler(std::make_unique<std::vector<MSG_T>>(cMsgVec));

    return Enum_CommRetVal::SUCCESS;
  }

  auto registerHandler(commHandler_t<MSG_T> handler) -> serviceId_t
  {
    static serviceId_t mSId = 0;
    mSId++;
    auto tmpServiceInfo = std::make_unique<serviceInfo>();
    tmpServiceInfo->msgHandler = handler;
    mMsgMap.insert(std::pair<const serviceId_t, std::unique_ptr<serviceInfo>>(mSId, move(tmpServiceInfo)));
    std::cout << "ThreadCommuncator: registered handler# " << mSId << "\n";
    return mSId;
  }

  auto registerHandler(commHandlerVec_t<MSG_T> handler) -> serviceId_t
  {
    static serviceId_t mSId = 0;
    mSId++;
    auto tmpServiceInfo = std::make_unique<serviceInfoVec>();
    tmpServiceInfo->msgHandler = handler;
    mMsgVecMap.insert(std::pair<const serviceId_t, std::unique_ptr<serviceInfoVec>>(mSId, move(tmpServiceInfo)));
    std::cout << "ThreadCommuncator: registered Vec handler# " << mSId << "\n";
    return mSId;
  }
};

}// namespace ThreadCom
#endif