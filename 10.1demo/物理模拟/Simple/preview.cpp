# include <SDL2/SDL.h>
# include <iostream>
using namespace std;

int main(){
    // 1. 申请硬件区域，以及系统窗口


    // 初始化SDL（Simple DriectMedia Layer）
    SDL_Init(SDL_INIT_VIDEO);
    /*
    1. 系统环境检测与适配
    SDL 首先会检测当前操作系统（如 Linux、Windows、macOS 等），因为不同系统与图形硬件的交互方式完全不同：
        Linux：可能通过 X11 协议、Wayland 协议或直接帧缓冲（Framebuffer）与图形系统通信；
            Windows：通过 Win32 API 或 DirectX 接口与显卡驱动交互；
            macOS：通过 Cocoa 框架或 Metal 接口与系统图形服务对接。
            这一步不直接操作硬件，但会确定 “与硬件通信的通道”（比如选择使用哪种系统接口调用显卡）。
    
    2. 图形驱动与硬件能力查询
        SDL 会通过系统接口向显卡驱动发送查询请求，获取硬件支持的核心能力：
            显示模式：查询显示器支持的分辨率（如 1920x1080、2560x1440）、刷新率（如 60Hz、144Hz）、颜色深度（如 32 位 RGBA）；
            渲染接口：检测显卡是否支持 OpenGL、Vulkan、Direct3D 等硬件加速渲染接口（后续创建渲染器时会用到）；
            显存信息：获取显卡可用显存大小（影响后续纹理、帧缓冲的分配）。
            例如，在 Linux 上，SDL 可能通过 libX11 库向 X Server 发送请求，再由 X Server 与显卡驱动（如 NVIDIA 驱动、AMD 的 Mesa 驱动）通信获取这些信息。
    3. 视频子系统资源初始化
        SDL 会为视频子系统分配必要的软件资源和硬件资源：
            软件层面：初始化内部数据结构（如存储显示模式列表、默认渲染参数的结构体），启动处理窗口事件（如移动、 resize）的线程；
            硬件层面：
            申请显卡的 “帧缓冲（Framebuffer）” 资源（一块显存区域，用于暂存即将显示的图像像素）；
            初始化 “显示控制器” 的通信通道（确保后续窗口数据能被正确发送到显示器）。
            这一步类似 “给显卡和显示器‘通电预热’”，告诉硬件 “准备好接收图像数据了”。
    4. 窗口系统连接
    SDL 会与操作系统的窗口管理器建立连接（如 Linux 的 GNOME/KDE、Windows 的 Explorer）：
        注册 “创建窗口” 的权限（确保后续创建的窗口能被系统认可并显示在桌面上）；
        协商窗口的基础属性（如是否允许置顶、全屏切换、鼠标 / 键盘事件传递方式）。
        这一步不直接操作硬件，但决定了后续窗口与硬件交互的 “规则”（比如全屏时如何独占显示器输出）。
    5. 硬件加速上下文准备（可选）
        如果显卡支持硬件加速（如 OpenGL），SDL 会提前创建对应的硬件加速上下文：
        向显卡驱动申请一个 “渲染上下文”（用于后续通过 OpenGL 等接口直接操作显卡渲染管线）；
        绑定上下文到当前进程，确保后续的渲染命令（如绘制图形、纹理）能被显卡直接执行。
        这一步相当于 “给显卡发了一把‘钥匙’”，让后续的图形绘制可以绕过 CPU 直接由显卡硬件加速处理。

    总结：硬件层面的最终状态
    调用 SDL_Init(SDL_INIT_VIDEO) 后，底层硬件会进入 “就绪状态”：
    显卡：已被初始化，显存分配了基础缓冲区，渲染管线准备接收命令；
    显示器：其支持的显示模式已被记录，显示控制器准备接收窗口数据；
    CPU 与硬件的通信通道：已建立（通过系统接口和驱动），后续的 SDL_CreateWindow、SDL_RenderClear 等操作可以直接通过这些通道控制硬件。
    
    简单说，这一步就是 “告诉图形硬件：‘我要开始画图了，准备好你的画笔和画布’”。
    */


    // 获取winow指针(申请内存空间)
    SDL_Window* window = SDL_CreateWindow(
        "1",
        SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
        800,600,
        SDL_WINDOW_SHOWN
    );

    // 2. 渲染
    // 2.1 准备(本质是初始化了上述其中一个后端，并为你准备好了绘图环境。)
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );


    // 2.2 开始渲染

    // 设置绘制颜色去清屏
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // setDrawColor
    SDL_RenderClear(renderer);

    //
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect = {100, 100, 200, 150};
    SDL_RenderDrawRect(renderer, &rect);
     
    //上面这几步都是在操作render这个指针指向的内存（window）,下面要显示

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        // ... 渲染逻辑 ...
        SDL_RenderPresent(renderer);
    }

}
