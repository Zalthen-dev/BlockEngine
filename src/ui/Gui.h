#pragma once

#include <cstdint>
#include <string>

//#include "imgui.h"
//#include "rlImGui.h"
#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/rlImGui/rlImGui.h"
// clang complains if i dont use direct path

#include "../core/Logger.h"

class Gui;
extern std::vector<Gui*> g_guis;

class Gui : public ILogListener {
public:
    virtual void Draw() {};

    Gui() {
        Logger::AddListener(this);
        g_guis.push_back(this);
    }

    ~Gui() {
        Logger::RemoveListener(this);
        g_guis.erase(std::remove(g_guis.begin(), g_guis.end(), this), g_guis.end());
    }

    void SetVisible(bool v) { visible = v; }
    bool IsVisible() { return visible; }

    void SetTitle(std::string t) { title = t; }
    std::string GetTitle() { return title; }

protected:
    bool visible;
    std::string title = "Gui Title";
};


