#include <SDL2/SDL.h>
#include <GL/gl.h>
#include "player.h"
#include "block.h"

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
    ic.setSpacePartitioning<Block, od::CollisionGrid>(128, 128, width/128+1, height/128+1);
    const od::CollisionGrid & sp = *static_cast<const od::CollisionGrid*>(ic.getSpacePartitioning(Block::ObjectIndex));
    
    // optimized selection demo
    std::vector<od::Rect> selected;    
    
    od::instanceCreate<Player>(16*2, 16*2);
    
    for(int i=0; i<640; i+=16)
    {
        od::instanceCreate<Block>(i, 0);
        od::instanceCreate<Block>(i, 480 - 16);
    }
    for(int j=16; j<480; j+=16)
    {
        od::instanceCreate<Block>(0, j);
        od::instanceCreate<Block>(640 - 16, j);
    }
    for(int i=16*3; i<624; i+=16*3)
    for(int j=16*3; j<480; j+=16*3)
        od::instanceCreate<Block>(i, j-8);
    
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
			else if(ev.type == SDL_MOUSEBUTTONDOWN)
			{
                if(ev.button.button == SDL_BUTTON_LEFT)
                {
                    int x = ev.button.x / sp.cellWidth();
                    int y = ev.button.y / sp.cellHeight();
                    
                    selected.clear();
                    auto t = ic.retrieveInstances<Block>(x * sp.cellWidth(), y * sp.cellHeight(),
                                (x+1) * sp.cellWidth(), (y+1) * sp.cellHeight(), od::CollisionGrid::Intersection);
                    for(Block * ii: t)
                    {
                        selected.push_back(ii->boundingBox());
                    }
                }
			}
		}
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        ic.update(1.f);
        ic.render();
        
        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        for(size_t i=0; i<sp.width(); ++i)
        for(size_t j=0; j<sp.height(); ++j)
        {
            const size_t wid = sp.cellWidth();
            const size_t hei = sp.cellHeight();
            
            glVertex2f(i*wid, j*hei);
            glVertex2f((i+1)*wid, j*hei);
            
            glVertex2f((i+1)*wid, j*hei);
            glVertex2f((i+1)*wid, (j+1)*hei);
            
            glVertex2f((i+1)*wid, (j+1)*hei);
            glVertex2f(i*wid, (j+1)*hei);
            
            glVertex2f(i*wid, (j+1)*hei);
            glVertex2f(i*wid, j*hei);
        }
        glColor3f(1, 0, 0);
        for(auto& r: selected)
        {
            glVertex2f(r.x1, r.y1);
            glVertex2f(r.x2, r.y1);
            
            glVertex2f(r.x2, r.y1);
            glVertex2f(r.x2, r.y2);
            
            glVertex2f(r.x2, r.y2);
            glVertex2f(r.x1, r.y2);
            
            glVertex2f(r.x1, r.y2);
            glVertex2f(r.x1, r.y1);
        }
        glEnd();
        
        SDL_GL_SwapWindow(window);
    }
	
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

