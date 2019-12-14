//
// Created by romaonishuk on 28.10.19.
//

#ifndef LEXI_LOGGER_HPP
#define LEXI_LOGGER_HPP

#include <iostream>

// TODO(rmn): Description
// TODO(rmn): pass uniref to Log
// TODO(rmn): different output locations
class Logger
{
public:
    enum class LogLevel {
        kInfo
    };

    constexpr auto LogLevelToString(const LogLevel lvl) const noexcept {
        switch (lvl)
        {
            case LogLevel::kInfo:
                return "INFO";
            default:
                return "UB";
        }
    }

    static Logger &Get() noexcept {
        static Logger self;
        return self;
    }

    template<typename T, typename... Tx>
    void Log(T current_data, Tx... other) {
        Log(current_data);
        Log(other...);
    }

    template<typename T>
    void Log(T data) {
        std::cout << data;
    }

private:
    Logger() = default;
    ~Logger() = default;
};


#endif //LEXI_LOGGER_HPP
