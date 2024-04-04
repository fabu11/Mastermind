#include <SDL.h>
#include <SDL_ttf.h>
#include <imgui.h>
#include "object.h"
#include "gameboard.h"
#include <vector>

#define WIDTH (800)
#define HEIGHT (600)

bool _ButtonCenteredOnLine(const char* label, float alignment);

class App {
  public:
    App();
    ~App();
    bool getFullscreen(){ return fullscreen; } 
  private:
    // App data
    const char* version = "0.0.1";
    // SDL2
    SDL_Window *window;
    SDL_Renderer *ren;
    bool fullscreen = false;
    // Game Loop
    bool running = true;
    void setup();
    void run();
    void input();
    void update();
    void render();
    // Mouse
    int mousex, mousey;
    // ImGui Menus
    float displayW, displayH;
    bool show_pause = false;
    bool show_rules = false;
    bool show_demo = false;
    bool show_debug = true;
    bool show_about = false;
    bool show_popup_window = false;
    ImVec2 popup_window_pos;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    // fonts
    ImFont *title_font; 
    ImFont *default_font; 
    ImFont *h2;
    // Render
    void drawObj(Object o);
    //Game board
    Object board;
    int board_scale = 5; 
    bool hover_board = false;
    bool dragging = false;
    ImVec2 drag_start;
};
