#include "TextEditor.h"

int TextEditor::GetLineCount() {
    int count = 1;
    for (char c : text)
        if (c == '\n') count++;

    return count;
}


void TextEditor::OnLog(const std::string& message) {
    // keep, will be used for when text editor has built-in console
}

int InputTextCallback(ImGuiInputTextCallbackData* data) {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        // ImGui requests buffer resize
        std::string* str = reinterpret_cast<std::string*>(data->UserData);
        str->resize(data->BufTextLen);
        data->Buf = str->data();
    }

    return 0;
}

void TextEditor::Draw() {
    if (!visible) return;

    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(60, 60), ImGuiCond_Once);

    if (!ImGui::Begin(title.c_str(), &visible, ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New", "Ctrl+N")) {}

            ImGui::Separator();
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save as..", "Ctrl+Shift+S")) {}

            ImGui::Separator();
            if (ImGui::MenuItem("Quit ")) {}

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Shift+Z")) {}

            ImGui::Separator();
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
            if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
            if (ImGui::MenuItem("Paste", "Ctrl+P")) {}

            ImGui::Separator();
            if (ImGui::MenuItem("Clear Output", "Ctrl+P")) {}

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImVec2 availRegion = ImGui::GetContentRegionAvail();
    availRegion.y = std::max(availRegion.y - 100, 60.f);

    ImGui::BeginChild("TextEditorArea", availRegion, true, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Columns(2, "text_colums", false);
    ImGui::SetColumnWidth(0, 30);

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

    std::string lineCountText = "";
    int lineCount = GetLineCount();
    for (int i = 0; i < lineCount; i++) {
        lineCountText.append(std::to_string(i+1) + "\n");
    }

    ImGui::TextUnformatted(lineCountText.c_str());

    float scrollY = ImGui::GetScrollY();
    ImGui::SetCursorPosY(-scrollY);

    ImGui::NextColumn();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.12f, 0.12f, 0.12f, 1.f));
    ImGui::PushItemWidth(-FLT_MIN);

    ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CallbackResize;
    ImVec2 textAreaSize = ImGui::GetContentRegionAvail();

    float charHeight = ImGui::GetTextLineHeight();
    float minHeight = charHeight * lineCount + 20.f;
    textAreaSize.y = std::max(textAreaSize.y, minHeight);

    ImGui::InputTextMultiline("##TextEditor", text.data(), text.size() + 1, textAreaSize, flags, InputTextCallback, (void*)&text);

    ImGui::PopStyleColor();
    ImGui::PopItemWidth();

    ImGui::Columns(1);
    ImGui::PopFont();

    ImGui::EndChild();

    ImGui::BeginChild("EmbeddedConsole", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    if (ImGui::BeginChild("OutputScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar)) {
        for (size_t i = 0; i < history.size(); i++) {
            ImGui::Text("%s", history[i].c_str());
        }

        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);

    }

    ImGui::EndChild();
    ImGui::EndChild();

    ImGui::End();
}
