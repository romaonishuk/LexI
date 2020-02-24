//
// Created by romaonishuk on 27.10.19.
//

#ifndef LEXI_I_COMMAND_HPP
#define LEXI_I_COMMAND_HPP

namespace Command
{
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
};

#endif //LEXI_I_COMMAND_HPP
