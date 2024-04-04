#ifndef OBJECT_H
#define OBJECT_H

#include "SDL_render.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Object {
    private:
        SDL_Rect dest;
        SDL_Rect src;
        SDL_Texture * texture;
        bool flipObject = false;
    public:
        Object(){}
        SDL_Rect getDest() const {return dest;}
        void setDest(int x, int y);
        SDL_Rect getSource() const {return src;}
        SDL_Texture * getTexture() const {return texture;}
        void draw(SDL_Renderer *ren);
        void setDest(int x, int y, int w, int h);
        void setSource(int x, int y, int w, int h);
        void setImage(std::string filename, SDL_Renderer * ren);
        int getDestX(){ return dest.x; }
        int getDestY(){ return dest.y; }
        int getDestW(){ return dest.w; }
        int getDestH(){ return dest.h; }
        void setFlipObject(bool s){flipObject = s;}
        bool getFlipObject(){return flipObject;}
};

#endif

