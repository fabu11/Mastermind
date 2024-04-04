#include "object.h"
#include "SDL_render.h"
#include <SDL_image.h>
#include <SDL.h>

void Object::draw(SDL_Renderer * ren){
  SDL_Rect dest = getDest();
  SDL_Rect src = getSource();
  SDL_RendererFlip f = flipObject ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE; 
  SDL_RenderCopyEx(ren, getTexture(), &src, &dest, 0, NULL, f);
}

void Object::setDest(int x, int y, int w, int h){
    this->dest.x=x;
    this->dest.y=y;
    this->dest.w=w;
    this->dest.h=h;
}

void Object::setSource(int x, int y, int w, int h){
    this->src.x=x; // sets starting location
    this->src.y=y;
    this->src.w=w; // actual width of image
    this->src.h=h;
}

void Object::setDest(int x, int y) {
  dest.x=x;
  dest.y=y;
}

void Object::setImage(std::string filename, SDL_Renderer*ren){
    SDL_Surface * surface = IMG_Load(filename.c_str());
    this->texture = SDL_CreateTextureFromSurface(ren, surface);
}
