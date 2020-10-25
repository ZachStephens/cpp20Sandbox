#ifndef RUNNABLE_MANAGER
#define RUNNABLE_MANAGER

#include <memory>
#include <functional>

#include "ThreadCommunicator/ThreadCommunicator.hpp"

class RunnableManager
{
protected:
  std::shared_ptr<ThreadCom::ThreadCommunicator<ThreadCom::commMsg>> mThreadComm;

public:
  RunnableManager(std::shared_ptr<ThreadCom::ThreadCommunicator<ThreadCom::commMsg>> threadComm);
  ThreadCom::serviceId_t mServiceId;

  virtual void run() = 0;
  virtual ~RunnableManager(){};
};

#endif