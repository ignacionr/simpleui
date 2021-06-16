#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <functional>
#include <list>
#include <algorithm>
#include <map>
#include <vector>

namespace sdl
{
  struct sys
  {
    using handler = std::function<bool(SDL_Event&)>;

    sys() {
      if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw std::runtime_error("Unable to initialize SDL.");
      }
      handlers_.push_back([this](SDL_Event &ev){
        if (ev.type == SDL_KEYDOWN) {
          auto const pos {key_handlers_.find(ev.key.keysym.scancode)};
          if (pos != key_handlers_.end()) {
            pos->second();
            return true;
          }
        }
        return false;
      });
    }
    ~sys() {
      // close SDL
	    SDL_Quit();
    }

    int poll(SDL_Event &ev) const {
      int count;
      for (count = SDL_PollEvent(&ev); count && std::any_of(handlers_.begin(), handlers_.end(), [&ev](auto h){ return h(ev);}); count = SDL_PollEvent(&ev))
       { ; }
      return count;
    }

    void on_quit(std::function<void()> action) {
      handlers_.push_back([action](SDL_Event &ev){
        if (ev.type == SDL_QUIT) {
          action();
          return true;
        }
        return false;
      });
    }

    void on_keydown(std::vector<SDL_Scancode> codes, std::function<void()> action) {
      for (auto sc: codes) {
        key_handlers_[sc] = action;
      }
    }
private:
    std::list<handler> handlers_;
    std::map<SDL_Scancode, std::function<void()>> key_handlers_;
  };

  struct surface {
    surface(std::string const &path) {
    	surface_ = IMG_Load(path.c_str());
      if (!surface_) {
        throw std::runtime_error(std::string("unable to read ") + path);
      }
    }
    ~surface() {
      // clears main-memory
    	SDL_FreeSurface(surface_);
    }
    SDL_Surface *ptr() const { return surface_; }
  private:
    SDL_Surface *surface_;
  };

  struct texture {
    texture(SDL_Texture *t): texture_{t} {}
    ~texture() {
      SDL_DestroyTexture(texture_);
    }
    SDL_Texture *ptr() const { return texture_; }
    SDL_Rect size() const {
      SDL_Rect dest;
    	SDL_QueryTexture(ptr(), NULL, NULL, &dest.w, &dest.h);
      return dest;
    }
  private:
    SDL_Texture *texture_;
  };

  struct renderer {
    renderer(SDL_Renderer* r): renderer_{r} {}
    ~renderer() {
      // destroy renderer
	    SDL_DestroyRenderer(renderer_);
    }

    texture from_surface(surface const &a_surface) {
      return SDL_CreateTextureFromSurface(renderer_, a_surface.ptr());
    }

    void clear() {
  		SDL_RenderClear(renderer_);
    }
    void copy(texture const &t, SDL_Rect const* src, SDL_Rect const *dst) {
		  SDL_RenderCopy(renderer_, t.ptr(), src, dst);
    }
    void present() {
  		SDL_RenderPresent(renderer_);
    }
  private:
    SDL_Renderer *renderer_;
  };

  struct window
  {
    window(std::string const &text) {
      	window_ = SDL_CreateWindow(text.c_str(),
									SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED,
									1000, 1000, 0);
    }
    ~window() {
      // destroy window
    	SDL_DestroyWindow(window_);
    }

    renderer create_renderer() {
      return SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    }
  private:
    SDL_Window* window_;
  };
  

  
} // namespace sdl
