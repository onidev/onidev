#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <onidev/game.h>

class Player:
    public od::AutoIndex<od::Object2d, Player>
{
public:
    Player(float x, float y):
        od::AutoIndex<od::Object2d, Player>(x, y) {}
    
    od::Shape * collisionMask(int p) const override {
        static od::Rect rect(0, 0, 16, 16);
        return &rect;
    }
    
    void step(float dt) override;
    void draw() const override {
        glBegin(GL_QUADS);
            glVertex2f(pos.x, pos.y);
            glVertex2f(pos.x + 16, pos.y);
            glVertex2f(pos.x + 16, pos.y + 16);
            glVertex2f(pos.x, pos.y + 16);
        glEnd();
    }
};

void Player::step(float dt)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_LEFT] ) pos.x -= dt;
    if(state[SDL_SCANCODE_RIGHT]) pos.x += dt;
    if(state[SDL_SCANCODE_UP]   ) pos.y -= dt;
    if(state[SDL_SCANCODE_DOWN] ) pos.y += dt;
}

int main(int argc, char **argv)
{	
    SDL_Window* window;
    SDL_GLContext context;
	
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Quit();
        return -1;
    }
	
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	int width = 640, height = 480;
    window = SDL_CreateWindow("Simple game example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if(window == 0)
    {
        SDL_Quit();
        return -1;
    }
    
    context = SDL_GL_CreateContext(window);
    if(context == 0)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glClearColor(0.6, 0.6, 0.6, 0);
	
    od::InstanceContext<od::Object2d> ic;
    od::instanceCreate<Player>(64, 64);
	
	bool loop = true;
    while(loop)
    {
        SDL_Event ev;
		while(SDL_PollEvent(&ev))
		{
			if(ev.type == SDL_QUIT)
				loop = false;

			if(ev.type == SDL_KEYDOWN)
			{
				switch (ev.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					loop = false;
					break;
				}
			}
		}
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        ic.update(1.f);
        ic.render();
        
        SDL_GL_SwapWindow(window);
    }
	
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
