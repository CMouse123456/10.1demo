#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <random>
#include <vector>
#include <chrono>
#include <string>

// 窗口设置（与其他版本保持一致）
const int WIDTH = 800;
const int HEIGHT = 600;
const int NUM_SHAPES = 1000;  // 图形数量

// 圆形结构体
struct Circle {
    float x, y;          // 位置
    float size;          // 大小
    float dx, dy;        // 移动速度
    SDL_Color color;     // 颜色
};

// 绘制圆形轮廓的函数（中点圆算法，仅画边缘）
void drawCircleOutline(SDL_Renderer* renderer, float x, float y, float radius) {
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);
    int iradius = static_cast<int>(radius);
    
    int dx = iradius;
    int dy = 0;
    int err = 0;
    
    while (dx >= dy) {
        // 绘制8个对称点（仅轮廓）
        SDL_RenderDrawPoint(renderer, ix + dx, iy + dy);
        SDL_RenderDrawPoint(renderer, ix + dy, iy + dx);
        SDL_RenderDrawPoint(renderer, ix - dy, iy + dx);
        SDL_RenderDrawPoint(renderer, ix - dx, iy + dy);
        SDL_RenderDrawPoint(renderer, ix - dx, iy - dy);
        SDL_RenderDrawPoint(renderer, ix - dy, iy - dx);
        SDL_RenderDrawPoint(renderer, ix + dy, iy - dx);
        SDL_RenderDrawPoint(renderer, ix + dx, iy - dy);
        
        if (err <= 0) {
            dy++;
            err += 2 * dy + 1;
        }
        if (err > 0) {
            dx--;
            err -= 2 * dx + 1;
        }
    }
}

// 初始化随机圆形
std::vector<Circle> initCircles() {
    std::vector<Circle> circles;
    circles.reserve(NUM_SHAPES);
    
    // 随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist_pos_x(0, WIDTH);
    std::uniform_int_distribution<int> dist_pos_y(0, HEIGHT);
    std::uniform_int_distribution<int> dist_size(10, 30);
    std::uniform_real_distribution<float> dist_dir(-2.0f, 2.0f);
    std::uniform_int_distribution<int> dist_color(0, 255);
    
    for (int i = 0; i < NUM_SHAPES; i++) {
        Circle circle;
        circle.x = dist_pos_x(gen);
        circle.y = dist_pos_y(gen);
        circle.size = dist_size(gen);
        circle.dx = dist_dir(gen);
        circle.dy = dist_dir(gen);
        circle.color = {
            static_cast<Uint8>(dist_color(gen)),
            static_cast<Uint8>(dist_color(gen)),
            static_cast<Uint8>(dist_color(gen)),
            255
        };
        
        circles.push_back(circle);
    }
    
    return circles;
}

int main(int argc, char* argv[]) {
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL初始化失败: %s", SDL_GetError());
        return 1;
    }
    
    // 初始化TTF
    if (TTF_Init() < 0) {
        SDL_Log("TTF初始化失败: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    
    // 创建窗口
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 C++圆形轮廓渲染性能测试",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        SDL_Log("窗口创建失败: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    // 创建硬件加速渲染器（关闭垂直同步，便于对比帧率）
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED  // 去掉垂直同步，与Tkinter保持一致
    );
    
    if (!renderer) {
        SDL_Log("渲染器创建失败: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    // 加载中文字体（Ubuntu系统）
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/wqy/wqy-microhei.ttc", 12);
    if (!font) {
        SDL_Log("字体加载失败: %s", TTF_GetError());
        font = TTF_OpenFont("/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc", 12);
    }
    
    if (!font) {
        SDL_Log("警告: 无法加载中文字体，将使用默认字体");
    }
    
    // 初始化圆形
    std::vector<Circle> circles = initCircles();
    
    // 帧率计算变量
    int fps = 0;
    int frame_count = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // 主循环控制
    bool running = true;
    SDL_Event event;
    
    while (running) {
        // 事件处理
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        
        // 清屏（黑色背景）
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // 更新并绘制所有圆形轮廓
        for (auto& circle : circles) {
            // 更新位置
            circle.x += circle.dx;
            circle.y += circle.dy;
            
            // 边界检测
            if (circle.x < circle.size || circle.x > WIDTH - circle.size) {
                circle.dx *= -1;
            }
            if (circle.y < circle.size || circle.y > HEIGHT - circle.size) {
                circle.dy *= -1;
            }
            
            // 设置颜色并绘制轮廓
            SDL_SetRenderDrawColor(
                renderer, 
                circle.color.r, 
                circle.color.g, 
                circle.color.b, 
                circle.color.a
            );
            drawCircleOutline(renderer, circle.x, circle.y, circle.size);
        }
        
        // 计算帧率
        frame_count++;
        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = current_time - start_time;
        
        if (elapsed.count() >= 1.0) {
            fps = static_cast<int>(frame_count / elapsed.count());
            frame_count = 0;
            start_time = current_time;
        }
        
        // 显示帧率文本
        if (font) {
            std::string text = "FPS: " + std::to_string(fps) + " | 图形数量: " + std::to_string(NUM_SHAPES);
            SDL_Color textColor = {255, 255, 255, 255};
            
            SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, text.c_str(), textColor);
            if (textSurface) {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                if (textTexture) {
                    SDL_Rect renderQuad = {10, 10, textSurface->w, textSurface->h};
                    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
                    SDL_DestroyTexture(textTexture);
                }
                SDL_FreeSurface(textSurface);
            }
        }
        
        // 刷新渲染
        SDL_RenderPresent(renderer);
    }
    
    // 清理资源
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}
    