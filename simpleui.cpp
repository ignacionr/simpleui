#include <SDL2/SDL_timer.h>

#include <iostream>

#include "sdlhelp.h"

int main(int argc, char *argv[])
{
  sdl::sys sys;
  sdl::window win{"GAME"};
  auto rend {win.create_renderer()};
  auto tex {rend.from_surface({"./funny-cat.jpeg"})};

	// let us control our image position
	// so that we can move it with our keyboard.
  auto dest {tex.size()};

	// adjust height and width of our image box.
	dest.w /= 6;
	dest.h /= 6;

	// sets initial x-position of object
	dest.x = (1000 - dest.w) / 2;

	// sets initial y-position of object
	dest.y = (1000 - dest.h) / 2;

	int speed = 300;

  SDL_Event event;
  bool close;
  sys.on_quit([&close](){ close = true; });
  sys.on_keydown({SDL_SCANCODE_W, SDL_SCANCODE_UP}, [&dest, speed](){
    dest.y -= speed / 30;
  });
  sys.on_keydown({SDL_SCANCODE_A, SDL_SCANCODE_LEFT}, [&dest, speed](){
    dest.x -= speed / 30;
  });
  sys.on_keydown({SDL_SCANCODE_S, SDL_SCANCODE_DOWN}, [&dest, speed](){
    dest.y += speed / 30;
  });
  sys.on_keydown({SDL_SCANCODE_D, SDL_SCANCODE_RIGHT}, [&dest, speed](){
    dest.x += speed / 30;
  });
	for (close = false;!close;) {

		while (sys.poll(event)) {
		}

		// right boundary
		if (dest.x + dest.w > 1000)
			dest.x = 1000 - dest.w;

		// left boundary
		if (dest.x < 0)
			dest.x = 0;

		// bottom boundary
		if (dest.y + dest.h > 1000)
			dest.y = 1000 - dest.h;

		// upper boundary
		if (dest.y < 0)
			dest.y = 0;

		// clears the screen
    rend.clear();
    rend.copy(tex, NULL, &dest);
    rend.present();

		// calculates to 60 fps
		SDL_Delay(1000 / 60);
	}

	return 0;
}
