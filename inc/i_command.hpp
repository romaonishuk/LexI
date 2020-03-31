//
// Created by romaonishuk on 27.10.19.
//

#ifndef LEXI_I_COMMAND_HPP
#define LEXI_I_COMMAND_HPP

enum class ExecutionResult
{
    kSuccess = 0,
    kFailure = 1
};

class ICommand
{
public:
    virtual ExecutionResult Execute() = 0;
    virtual ExecutionResult UnExecute() = 0;
    virtual bool IsReversible() = 0;

    virtual ~ICommand() = default;
};

#include "event_manager.hpp"

class QuitCommand: public ICommand
{
public:
    explicit QuitCommand(EventManager* manager): m_manager(manager) {}

    ExecutionResult Execute() override;

    ExecutionResult UnExecute() override { return ExecutionResult::kFailure; }

    bool IsReversible() override { return false; }

private:
    EventManager* m_manager;
};

#endif  // LEXI_I_COMMAND_HPP
