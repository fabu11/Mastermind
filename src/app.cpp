#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstddef>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <iostream>
#include "app.h"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "SDL_stdinc.h"
#include "SDL_video.h"

void App::run(){
  setup();
  while(running){
    input();
    update();
    render();
  }
}

void App::setup(){
  // Create Gameboard
  board.setDest(WIDTH/2, HEIGHT / 2, 50 , 120);
  board.setSource(0, 0, 50 , 120);
  board.setImage("assets/board.png", ren);
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
        if(e.key.keysym.sym == SDLK_ESCAPE && !show_rules && !show_about)
          show_pause = !show_pause;
        if(e.key.keysym.sym == SDLK_F11)
          fullscreen = !fullscreen;
        if(e.key.keysym.sym == SDLK_BACKSPACE)
          show_debug = !show_debug;
        break;
      case SDL_MOUSEWHEEL:
        if(show_pause)
          continue;
        if(e.wheel.y > 0){
          board_scale ++;
        }
        if(e.wheel.y < 0){
          if(board_scale != 1)
            board_scale --;
        }
        break;
      case SDL_MOUSEMOTION:
        if(dragging){
          int offsetX = mousex - drag_start.x;
          int offsetY = mousey - drag_start.y;
          int w, h;
          SDL_GetWindowSizeInPixels(window, &w, &h);
          if(!(offsetX < 0 || offsetX > w - board.getDest().w))
            board.setDest(offsetX, board.getDest().y);
          if(!(offsetY < 0 || offsetY > h - board.getDest().h))
            board.setDest(board.getDest().x, offsetY);
        }
        if (e.motion.x >= board.getDest().x && e.motion.x <= board.getDest().x + board.getDest().w &&
            e.motion.y >= board.getDest().y && e.motion.y <= board.getDest().y + board.getDest().h && !show_pause) {
          SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND)); 
          hover_board = true;
        } else {
          SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW)); 
          hover_board = false;
        }
        break;
      case SDL_MOUSEBUTTONUP:
        dragging = false;
        drag_start = ImVec2(0, 0);
        break;
      case SDL_MOUSEBUTTONDOWN:
        ImGuiIO io = ImGui::GetIO();
        if(e.button.button == SDL_BUTTON_RIGHT){
          popup_window_pos = ImVec2(mousex, mousey);
          show_popup_window = true;
        }
        else if (show_popup_window && e.button.button == SDL_BUTTON_LEFT && !io.WantCaptureMouse){
          show_popup_window = false;
        }
        if(e.button.button == SDL_BUTTON_LEFT && hover_board){
          dragging = true;
          drag_start = ImVec2(mousex - board.getDest().x, mousey - board.getDest().y);
        }
        break;
    }
  }
  SDL_GetMouseState(&mousex, &mousey);
}

void _setWindowSize(bool fs, ImGuiIO& io){
  ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
  if(!fs)
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y*0.6f), ImGuiCond_Always);
  else
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.3f, io.DisplaySize.y*0.5f), ImGuiCond_Always);
}
void App::update(){
  // Im Gui io
  ImGuiIO& io = ImGui::GetIO();
  // Board Scaling
  // Calculate the x-coordinate to center the board horizontally
  board.setDest(board.getDest().x, board.getDest().y, 50 * board_scale, 120 * board_scale);
  // ImGui New Frame
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGuiWindowFlags window_flags = 0;
  window_flags |= ImGuiWindowFlags_NoResize;
  window_flags |= ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoCollapse;
  ImGui::NewFrame();
  if (show_popup_window) {
    ImGui::SetNextWindowPos(ImVec2(popup_window_pos.x, popup_window_pos.y));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGuiWindowFlags window_flags2 = window_flags;
    window_flags2 |= ImGuiWindowFlags_NoTitleBar;
    window_flags2 |= ImGuiWindowFlags_NoDecoration;
    ImGui::Begin("Popup", NULL, window_flags2);
    if(ImGui::Selectable("Hey There!")){
      std::cout << "Clicked\n";
      show_popup_window = false;
    }
    ImGui::PopStyleVar(1); 
    ImGui::End();
  }
  // Fullscreen
  if(fullscreen){
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
  else {
    SDL_SetWindowFullscreen(window, 0);
  }
  // ImGui Menu
  if (show_pause) {
    _setWindowSize(fullscreen, io);
    // https://github.com/ocornut/imgui/issues/3875#issuecomment-790762144
    ImGui::PushFont(title_font); 
    ImGui::Begin("Settings", nullptr, window_flags);
    ImGui::PopFont();
    ImGui::PushFont(default_font);
    ImGui::Checkbox("Show Demo", &show_demo);
    ImGui::Checkbox("Fullscreen", &fullscreen);
    ImGui::ColorEdit3("Background Color", (float*)&clear_color);
    if(ImGui::Button("Show Rules"))
      show_rules = true;
    ImGui::SameLine(0, 20); if(ImGui::Button("About")) {show_about = true;}
    ImGui::NewLine();
    if(_ButtonCenteredOnLine("Quit", 0.5f))
      ImGui::OpenPopup("are_you_sure");
    // 
    // Show Quit Popup
    if (ImGui::BeginPopup("are_you_sure"))
    {
      ImGui::Text("Are you sure?");
      if(ImGui::Selectable("No"))
        running=true;
      if(ImGui::Selectable("Yes"))
        running=false;
      ImGui::EndPopup(); 
    }
    // Show Rules Menu
    if(show_rules){
      ImGui::PopFont();
      ImGui::PushFont(title_font);
      _setWindowSize(fullscreen, io);
      ImGui::Begin("Settings > Rules", nullptr, window_flags);
      ImGui::PopFont();
      ImGui::PushFont(h2);
      ImGui::Text("GAME NO.1 (1296 Permutations)");
      ImGui::PopFont();
      ImGui::PushFont(default_font);
      ImGui::TextWrapped("The Codemaker will randomly select 4 Code Pegs behind the shield. "
          "Use any combination of the 6 colors. Use 2 or more Code Pegs of the same color "
          "if you wish. The Codebreaker will try to duplicate the exact colors and positions "
          "of the secret code. Each time the Codebreaker places a row of Code Pegs, the "
          "Codemaker must give information by placing the black and white Key Pegs in the "
          "Key Peg holes alongside the Code Pegsm or by leaving holes vacant. When the Codebreaker "
          "duplicates the secret code, the Codemaker places 4 black Key Pegs and reveals the hidden code.");
      ImGui::PopFont();
      ImGui::PushFont(h2);
      ImGui::Text("A Black Key Peg");
      ImGui::PopFont();
      ImGui::PushFont(default_font);
      ImGui::SameLine();
      ImGui::TextWrapped("is placed IN ANY of the 4 Key Peg holes for every Code Peg which is the same color "
          "and in exactly the same position as one of the Code Pegs in the shielded holes");
      ImGui::PopFont();
      ImGui::PushFont(h2);
      ImGui::Text("A White Key Peg");
      ImGui::PopFont();
      ImGui::PushFont(default_font);
      ImGui::SameLine();
      ImGui::TextWrapped("is placed IN ANY of the 4 Key Peg holes for every Code Peg that is the same color "
          "but not in the same position as one of the Code Pegs placed by the Codemaker. For example, "
          "one white Key Peg is placed if the Codebreaker places one red Code Peg, and the Codebreaker has placed 2 or more red Code Pegs in their secret code"
          "in different positions.");
      ImGui::PopFont();
      ImGui::PushFont(h2);
      ImGui::Text("A Vacant Key Peg");
      ImGui::PopFont();
      ImGui::PushFont(default_font);
      ImGui::SameLine();
      ImGui::TextWrapped("is left for every Code Peg placed by the Codebreaker that is incorrect");
      if(_ButtonCenteredOnLine("Close", 0.5f))
        show_rules = false;
      ImGui::End();
    }
    // Show About Menu
    if(show_about){
      ImGui::PopFont();
      ImGui::PushFont(title_font);
      _setWindowSize(fullscreen, io);
      ImGui::Begin("Settings > About", nullptr, window_flags);
      ImGui::PopFont();
      ImGui::PushFont(default_font);
      ImGui::Text("GitHub: https://github.com/fabu11/Mastermind");
      ImGui::NewLine();
      ImGui::Text("Version: %s", version);
      if(_ButtonCenteredOnLine("Close", 0.5f))
        show_about = false;
      ImGui::End();
    }
    ImGui::End();
    ImGui::PopFont();
  }
  //Debug Menu
  if(show_debug){
    ImGui::Begin("Debug Menu");
    ImGui::Text("MouseX: %d MouseY: %d", mousex, mousey);
    ImGui::Text("Scale: %d", board_scale);
    ImGui::SeparatorText("Dragging");
    ImGui::Text("Hover Board: %d", hover_board);
    ImGui::Text("Dragging: %d", dragging);
    ImGui::Text("Current Pos: (%d, %d)", board.getDest().x, board.getDest().y);
    ImGui::Text("Drag Start X: %f, Drag Start Y: %f", drag_start.x, drag_start.y);
    ImGui::Text("offset X: %f offset Y: %f", mousex - drag_start.x, mousey - drag_start.y);
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
  drawObj(board);
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
  SDL_SetWindowResizable(window, SDL_TRUE);
  SDL_SetWindowTitle(window, "MASTERMIND original ©");
  SDL_Surface *window_icon = IMG_Load("assets/icon.png");
  SDL_SetWindowIcon(window, window_icon);
  // Font
  TTF_Init();
  // ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  // ImGui Fonts
  // io.IniFilename = nullptr;
  default_font = io.Fonts->AddFontFromFileTTF("assets/fonts/Inter-Regular.ttf", 16);
  title_font = io.Fonts->AddFontFromFileTTF("assets/fonts/Inter-SemiBold.ttf", 45);
  h2 = io.Fonts->AddFontFromFileTTF("assets/fonts/Inter-SemiBold.ttf", 20);
  // style
  ImGui::StyleColorsLight();
  // ImGui Style
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowPadding.x = 10;
  style.WindowPadding.y = 10;
  style.FramePadding.x = 10;
  style.WindowRounding = 6;
  style.ChildRounding = 6;
  style.FrameRounding = 5;
  style.ItemSpacing.y = 16;
  style.WindowTitleAlign.x = 0.50f;
  // Finish init
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

bool _ButtonCenteredOnLine(const char* label, float alignment = 0.5f)
{
    ImGuiStyle& style = ImGui::GetStyle();

    float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;

    float off = (avail - size) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

    return ImGui::Button(label);
}

void App::drawObj(Object o){
  SDL_RendererFlip f = o.getFlipObject() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
  SDL_Rect dest = o.getDest();
  SDL_Rect src = o.getSource();
  SDL_RenderCopyEx(ren, o.getTexture(), &src, &dest, 0, NULL, f);
}

int main(int argc, char* argv[]){
  App a;
  return 0;
}
