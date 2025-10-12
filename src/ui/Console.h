#pragma once

#include <vector>
#include <stdio.h>

#include "Gui.h"

#include "../datatypes/Task.h"

class Console : public Gui {
public:
    void ClearLog();
    void Reset();

    void OnLog(const std::string& message);

    void Log(std::string text, ImVec4 color = ImVec4(1,1,1,1));
    void Warn(std::string text);
    void Error(std::string text);

    void ExecCommand(std::string text);
    void Draw();

protected:
    std::string title = "BlockEngine Console";
    char inputBuf[256] = "";

    std::vector<std::string> history;
    std::vector<ImVec4> colors;

    int historyPos = -1;
};
