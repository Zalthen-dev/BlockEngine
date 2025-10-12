#pragma once

#include <vector>
#include <string>
#include <algorithm>

class ILogListener {
public:
    virtual ~ILogListener() = default;
    virtual void OnLog(const std::string& message) = 0;
};

class Logger {
public:
    static void AddListener(ILogListener* listener);
    static void RemoveListener(ILogListener* listener);
    static void Log(const std::string& message);
private:
    static inline std::vector<ILogListener*> listeners;
};
