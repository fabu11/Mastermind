#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include "app.h"

void App::run(){
  while(running){
    input();
    update();
    render();
  }
}

void App::input(){
  SDL_Event e;
  const uint8_t * keystates = SDL_GetKeyboardState(NULL);
  while(SDL_PollEvent(&e)){
    ImGui_ImplSDL2_ProcessEvent(&e);
    switch (e.type) {
      case SDL_QUIT:
        running = false;
        break;
      case SDL_KEYDOWN:
        if(e.key.keysym.sym == SDLK_ESCAPE && !show_rules)
          show_pause = !show_pause;
        if(e.key.keysym.sym == SDLK_F11)
          fullscreen = !fullscreen;
        break;
    }
  }
  SDL_GetMouseState(&mousex, &mousey);
}

void App::update(){
  // ImGui New Frame
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
  // Fullscreen
  if(fullscreen){
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
  else {
    SDL_SetWindowFullscreen(window, 0);
  }
  // Im Gui Menu
  ImGuiIO& io = ImGui::GetIO();


  // ImGui Menu
  if (show_pause) {
    ImGuiWindowFlags window_flags = 0;
      window_flags |= ImGuiWindowFlags_NoResize;
      window_flags |= ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoCollapse;
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.8f, io.DisplaySize.y*0.6f), ImGuiCond_Always);
    ImGui::Begin("Settings", nullptr, window_flags);
    ImGui::Checkbox("Show Demo", &show_demo);
    if(ImGui::Button("Show Rules")){
      show_rules = true;

    }
    if(show_rules){
      ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
      ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.8f, io.DisplaySize.y*0.6f), ImGuiCond_Always);
      ImGui::Begin("Settings > Rules", nullptr, window_flags);
      ImGui::Text("adasfkajsdlfasdfasdfsadfasdfsdfaf\nasdfasdfasdfasdfasdf\nadsfadsf\n\ndasfasdfadsfasdfasdffsfasdf");
      if(ImGui::Button("Close")){
        show_rules = false;
      }
      ImGui::End();
    }
    ImGui::ColorEdit3("Background Color", (float*)&clear_color);
    ImGui::End();
  }
  if(show_demo){
    ImGui::ShowDemoWindow();
  }
}

void App::render(){
  ImGui::Render();
  SDL_SetRenderDrawColor(ren, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
  SDL_RenderClear(ren);
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
  SDL_RenderPresent(ren);

}

App::App(){
  SDL_Init(0);
  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &ren);
  if(!window){
    SDL_DestroyRenderer(ren);
    SDL_Quit();
  }
  if(!ren){
    SDL_DestroyWindow(window);
    SDL_Quit();
  }
  // Window Deco
  SDL_SetWindowTitle(window, "MASTERMIND original ©");
  SDL_Surface *window_icon = IMG_Load("assets/icon.png");
  SDL_SetWindowIcon(window, window_icon);
  // Font
  TTF_Init();
  // ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.IniFilename = nullptr;
  // ImFont *font = io.Fonts->AddFontFromFileTTF("assets/fonts/arialuni.ttf", 15.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
  // io.FontGlobalScale = 0.5f; 
  ImGui::StyleColorsLight();
  ImGui_ImplSDL2_InitForSDLRenderer(window, ren);
  ImGui_ImplSDLRenderer2_Init(ren);
  //Game Loop
  run();
}

App::~App(){
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();
}

int main(int argc, char* argv[]){
  App a;
  return 0;
}
