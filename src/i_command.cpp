//
// Created by romaonishuk on 04.03.20.
//

#include "i_command.hpp"


// ------ QuitCommand -----
ExecutionResult QuitCommand::Execute()
{
    m_manager->Stop();
    return ExecutionResult::kSuccess;
}