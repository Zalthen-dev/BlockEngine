#pragma once

#include "Gui.h"

class TextEditor : public Gui {
    int GetLineCount();
    void OnLog(const std::string& message);

    void Draw();
protected:
    std::string text;
    std::string title = "BlockEngine Script Editor";

    std::vector<std::string> history;
};
