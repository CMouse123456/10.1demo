#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

const int WIDTH = 800;
const int HEIGHT = 1200;
const float G = 9.82;
const float PPM = 50.0;
const float TIMESTEP = 0.016;
const float JOYSTICKFORCE = 10;

// logger logic

struct PhysicsData
{
    double x,y,vx,vy;
};



class DataLogger{
private:
    std::vector<PhysicsData> physics_data;
public:
    void add_physics_data(double pos_x, double pos_y, double vel_x, double vel_y) {
        physics_data.push_back({pos_x, pos_y, vel_x, vel_y});
    }
    
    void save_to_csv(const std::string& filename) {
        std::ofstream file(filename);
        
        // 写入表头
        file << "x,y,vx,vy\n";
        
        // 写入数据
        for (const auto& entry : physics_data) {
            file << entry.x << ","
                 << entry.y << ","
                 << entry.vx << ","
                 << entry.vy << "\n";
        }
        
        file.close();
        std::cout << "数据已保存到: " << filename << std::endl;
    }
    
    size_t size_of_phicis_data() const { return physics_data.size(); };
};


class Particle{
public:
    double x,y,vx,vy,ax,ay;
    double r = 20.0;
    double mass = 1.0;
    SDL_Color color;


    // constructs
    Particle(){
        x,y,vx,vy,ax,ay = 0.0;
    }
    Particle(double startX, double startY){
        vx,vy,ax,ay = 0.0;
        x = startX; y = startY;
    }
    Particle(double startX, double startY, double startVX, double startVY){
        x = startX; y = startY; vx = startVX; vy = startVY; ax = 0.0; ay = 0.0;
    }

    // phasical analysis
    void apply_force(double fx, double fy){
        ax = fx + ax; ay = fy + ay;
    }

    
    void update(double dt, double JOYSTICKFORCE){
        apply_force(0, mass * G);
        
        vx = vx + ax * dt;
        vy = vy + ay * dt;

        x += (vx * dt) * PPM;   // change into pixcle
        y += (vy * dt) * PPM;   // change into pixcle

        ax = ay = 0.0;  // reset the accelerate 
    }

    // tools
    void handle_boundary_collision(){
        // check

    }
    
    void draw_self(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, 222,244,234, 255);
        
        // collision logic
        // ...

        for (double i = y; i <= y + r; i ++){
            for (double j = x; j < x + r; j ++){
                SDL_RenderDrawPoint(renderer, j, i);
            }
        }
    }

    
};


// display logic
void draw_background(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);
}

int main(){
    // data_logger
    DataLogger datalogger;

    // window logic
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "BALL_SIMULATION",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        HEIGHT,WIDTH,
        SDL_WINDOW_SHOWN
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );



    // particle logic
    Particle particle = Particle(600.0 - (particle.r / 2), 0.0);  // draw in the middle of the window

    // The event monitor
    bool quit = false;
    SDL_Event e;
    while (!quit){
        while (SDL_PollEvent(&e) != 0){
            if (e.type == SDL_QUIT){
                quit = true;
            }else if(e.type == SDL_KEYDOWN){
                // R to reset
                if(e.key.keysym.sym == SDLK_r){
                    // reset logic

                }
                else if (e.key.keysym.sym == SDLK_ESCAPE){
                    // keyboard quit logic
                    quit = true;
                }

            }
        }
    
        // draw it !
        draw_background(renderer);
        particle.draw_self(renderer);
        particle.update(TIMESTEP, JOYSTICKFORCE);
        SDL_RenderPresent(renderer);

        // log the data
        // datalogger.add_physics_data(particle.x, particle.y, particle.vx, particle.vy);
        SDL_Delay(16);
    }

    // write
    // datalogger.save_to_csv("ball_simulation.csv");

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

    
    
}