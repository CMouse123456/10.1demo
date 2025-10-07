import tkinter as tk
import random
import time
from tkinter import font

# 窗口设置
WIDTH, HEIGHT = 800, 600
NUM_SHAPES = 1000  # 图形数量，与 SDL2 版本保持一致

class TkinterRenderTest:
    def __init__(self, root):
        self.root = root
        self.root.title("Tkinter渲染性能测试")
        self.root.geometry(f"{WIDTH}x{HEIGHT}")
        self.root.resizable(False, False)
        
        # 创建画布
        self.canvas = tk.Canvas(self.root, width=WIDTH, height=HEIGHT, bg="black")
        self.canvas.pack()
        
        # 确保中文显示正常
        self.font = font.Font(family=["SimHei", "WenQuanYi Micro Hei", "Heiti TC"], size=12)
        
        # 初始化图形
        self.shapes = []
        self.shape_ids = []  # 存储画布中图形的 ID
        self.init_shapes()
        
        # 帧率计算变量
        self.fps = 0
        self.frame_count = 0
        self.start_time = time.time()
        self.fps_text_id = self.canvas.create_text(10, 10, text="", fill="white", anchor="nw", font=self.font)
        
        # 开始动画循环
        self.animate()
    
    def init_shapes(self):
        """初始化所有图形的属性"""
        for _ in range(NUM_SHAPES):
            x = random.randint(0, WIDTH)
            y = random.randint(0, HEIGHT)
            size = random.randint(10, 30)
            dx = random.uniform(-2, 2)
            dy = random.uniform(-2, 2)
            color = self.random_color()
            
            # 在画布上创建圆形并记录 ID
            shape_id = self.canvas.create_oval(
                x - size, y - size,
                x + size, y + size,
                fill=color, outline=""
            )
            
            self.shapes.append({
                "x": x, "y": y, "size": size,
                "dx": dx, "dy": dy, "color": color
            })
            self.shape_ids.append(shape_id)
    
    def random_color(self):
        """生成随机 RGB 颜色（Tkinter 格式为 #RRGGBB）"""
        r = random.randint(0, 255)
        g = random.randint(0, 255)
        b = random.randint(0, 255)
        return f"#{r:02x}{g:02x}{b:02x}"
    
    def update_positions(self):
        """更新所有图形的位置并处理边界碰撞"""
        for i in range(NUM_SHAPES):
            shape = self.shapes[i]
            # 更新位置
            shape["x"] += shape["dx"]
            shape["y"] += shape["dy"]
            
            # 边界检测
            if shape["x"] < shape["size"] or shape["x"] > WIDTH - shape["size"]:
                shape["dx"] *= -1
            if shape["y"] < shape["size"] or shape["y"] > HEIGHT - shape["size"]:
                shape["dy"] *= -1
            
            # 更新画布上的图形位置
            x, y, size = shape["x"], shape["y"], shape["size"]
            self.canvas.coords(
                self.shape_ids[i],
                x - size, y - size,
                x + size, y + size
            )
    
    def calculate_fps(self):
        """计算并更新帧率"""
        self.frame_count += 1
        elapsed = time.time() - self.start_time
        if elapsed >= 1:
            self.fps = int(self.frame_count / elapsed)
            self.frame_count = 0
            self.start_time = time.time()
            # 更新帧率文本
            self.canvas.itemconfig(
                self.fps_text_id,
                text=f"FPS: {self.fps} | 图形数量: {NUM_SHAPES}"
            )
    
    def animate(self):
        """动画循环"""
        self.update_positions()
        self.calculate_fps()
        self.root.after(1, self.animate)  # 每 1ms 刷新一次（不限制帧率）

if __name__ == "__main__":
    root = tk.Tk()
    app = TkinterRenderTest(root)
    root.mainloop()
    