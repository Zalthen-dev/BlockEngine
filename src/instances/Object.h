#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>

#include "../core/Signal.h"

struct Object {
    //-- Properties --//
    std::string ClassName;
    std::string Name;

    //-- Events --//
    Signal Changed;

    //-- Methods --//
    Object(const std::string& className = "Object");
    virtual ~Object();

    virtual bool IsA(const std::string& className) const;
    virtual void FirePropertyChanged(const std::string& propertyName);
};
