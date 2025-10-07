#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <stack>

// 表示一个绘制点
struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

// 表示一条连续的轨迹
struct Stroke {
    std::vector<Point> points;
    
    void addPoint(int x, int y) {
        points.push_back(Point(x, y));
    }
};

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window* window = SDL_CreateWindow(
        "画笔工具 - 空格键绘制，Z键撤销",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1200, 800,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;
    bool quit = false;
    bool is_drawing = false;
    
    // 存储所有完成的笔画（用于撤销功能）
    std::stack<Stroke> strokeStack;
    
    // 当前正在绘制的笔画
    Stroke currentStroke;

    while (!quit) {
        // 处理事件
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                    
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                            
                        case SDLK_SPACE:
                            // 开始绘制 - 按下空格键
                            is_drawing = true;
                            std::cout << "开始绘制" << std::endl;
                            
                            // 记录按下空格键时的鼠标位置作为起点
                            int startX, startY;
                            SDL_GetMouseState(&startX, &startY);
                            currentStroke.addPoint(startX, startY);
                            break;
                            
                        case SDLK_z:
                            // 撤销功能 - 按Z键
                            if (!strokeStack.empty()) {
                                strokeStack.pop();
                                std::cout << "撤销一步，剩余笔画数: " << strokeStack.size() << std::endl;
                            } else {
                                std::cout << "无法撤销，没有更多笔画" << std::endl;
                            }
                            break;
                    }
                    break;
                    
                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_SPACE && is_drawing) {
                        // 结束绘制 - 释放空格键
                        is_drawing = false;
                        
                        // 将当前笔画保存到栈中
                        if (currentStroke.points.size() > 1) {
                            strokeStack.push(currentStroke);
                            std::cout << "完成一笔画，总笔画数: " << strokeStack.size() << std::endl;
                        }
                        
                        // 重置当前笔画
                        currentStroke.points.clear();
                    }
                    break;
                    
                case SDL_MOUSEMOTION:
                    // 如果正在绘制，记录鼠标移动轨迹
                    if (is_drawing) {
                        currentStroke.addPoint(event.motion.x, event.motion.y);
                    }
                    break;
            }
        }
        
        // 渲染
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 黑色背景
        SDL_RenderClear(renderer);
        
        // 绘制所有已完成的笔画
        std::stack<Stroke> tempStack = strokeStack; // 复制栈以进行遍历
        std::vector<Stroke> strokes;
        
        // 将栈中的笔画转移到向量中以便按顺序绘制
        while (!tempStack.empty()) {
            strokes.push_back(tempStack.top());
            tempStack.pop();
        }
        
        // 按从旧到新的顺序绘制笔画
        for (int i = strokes.size() - 1; i >= 0; i--) {
            const Stroke& stroke = strokes[i];
            if (stroke.points.size() > 1) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色
                for (size_t j = 1; j < stroke.points.size(); j++) {
                    SDL_RenderDrawLine(renderer, 
                                      stroke.points[j-1].x, stroke.points[j-1].y,
                                      stroke.points[j].x, stroke.points[j].y);
                }
            }
        }
        
        // 绘制当前正在进行的笔画
        if (is_drawing && currentStroke.points.size() > 1) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 红色表示当前绘制
            for (size_t j = 1; j < currentStroke.points.size(); j++) {
                SDL_RenderDrawLine(renderer, 
                                  currentStroke.points[j-1].x, currentStroke.points[j-1].y,
                                  currentStroke.points[j].x, currentStroke.points[j].y);
            }
        }
        
        SDL_RenderPresent(renderer);
        
        // 控制帧率
        SDL_Delay(16); // 约60FPS
    }

    // 清理资源
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    
    return 0;
}