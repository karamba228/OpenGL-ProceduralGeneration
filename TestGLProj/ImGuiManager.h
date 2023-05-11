#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl3.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class ImGuiManager {
public:
    ImGuiManager();
    ~ImGuiManager();

    void shutdown();
    void update();
    void disableImGuiControlls();
    void enableImGuiControlls();
    void startFrame(const char* name);
    void endFrame();
    bool isUsingMouse();
    bool isUsingKeyboard();
    
private:
    void *mouseFunc;
    static bool show_terrain_window;
};