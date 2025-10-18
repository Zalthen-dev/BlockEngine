#include "TextEditor.h"

int TextEditor::GetLineCount() {
    int count = 1;
    for (char c : text)
        if (c == '\n') count++;

    return count;
}

void TextEditor::ExecuteCurrentScript() {
    if (text.empty()) return;
    int tasksBefore = g_tasks.size();

    runningTask = Task_Run(L, text);

    int tasksAfter = g_tasks.size();
    if (tasksAfter > tasksBefore) {
        executedScript = true;
    }
}

void TextEditor::StopCurrentScript() {
    if (runningTask) {
        runningTask->ShouldStop = true;
        runningTask = nullptr;
        executedScript = false;
    }
}

int InputTextCallback(ImGuiInputTextCallbackData* data) {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        std::string* str = reinterpret_cast<std::string*>(data->UserData);
        str->resize(data->BufTextLen);
        data->Buf = str->data();
    }

    return 0;
}

void TextEditor::Draw() {
    if (!visible) return;

    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(80, 80), ImGuiCond_Once);

    if (!ImGui::Begin(title.c_str(), &visible, ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New", "Ctrl+N")) {}

            ImGui::Separator();
            if (ImGui::MenuItem("Open", "Ctrl+N")) {}

            ImGui::Separator();
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save as..", "Ctrl+Shift+S")) {}

            ImGui::Separator();
            if (ImGui::MenuItem("Quit")) visible = false;

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
            if (ImGui::MenuItem("Execute Script", "Ctrl+F")) if (!executedScript) ExecuteCurrentScript();
            if (ImGui::MenuItem("Stop Script", "Ctrl+G")) if (executedScript) StopCurrentScript();

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if (IsKeyDown(KEY_LEFT_CONTROL)) {
        if (IsKeyPressed(KEY_F)) {
            if (!executedScript) ExecuteCurrentScript();
        }

        if (IsKeyPressed(KEY_G)) {
            if (executedScript) StopCurrentScript();
        }
    }

    ImVec2 availRegion = ImGui::GetContentRegionAvail();
    availRegion.y = std::max(availRegion.y, 60.f);

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
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg));
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

    ImGui::End();
}
