#include "RunnableManager.hpp"

#include <memory>

RunnableManager::RunnableManager(std::shared_ptr<ThreadCom::ThreadCommunicator<ThreadCom::commMsg>> threadComm) : mThreadComm(std::move(threadComm))
{
}