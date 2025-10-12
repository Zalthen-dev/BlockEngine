#include "Logger.h"

void Logger::AddListener(ILogListener* listener) {
    listeners.push_back(listener);
}

void Logger::RemoveListener(ILogListener* listener) {
    listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
}

void Logger::Log(const std::string& message) {
    printf("%s\n", message.c_str());

    for (ILogListener* listener : listeners) {
        listener->OnLog(message);
    }
}

