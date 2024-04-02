#include <SDL.h>
#include <SDL_ttf.h>
#include <imgui.h>

#define WIDTH (800)
#define HEIGHT (600)

class App {
  public:
    App();
    ~App();
  private:
    // SDL2
    SDL_Window *window;
    SDL_Renderer *ren;
    bool fullscreen = false;
    // Game Loop
    bool running = true;
    void run();
    void input();
    void update();
    void render();
    // Mouse
    int mousex, mousey;
    // ImGui Menus
    float displayW, displayH;
    bool show_pause = true;
    bool show_rules = false;
    bool show_demo = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};
