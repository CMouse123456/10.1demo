import tkinter as tk
from PIL import Image, ImageTk, ImageDraw, ImageFont
import random
import time

# 窗口设置（与其他版本保持一致）
WIDTH, HEIGHT = 800, 600
NUM_SHAPES = 1000  # 图形数量

class PillowRenderTest:
    def __init__(self, root):
        self.root = root
        self.root.title("Pillow 纯软件渲染测试")
        self.root.geometry(f"{WIDTH}x{HEIGHT}")
        self.root.resizable(False, False)
        
        # 创建PIL图像（纯软件像素缓冲区）
        self.image = Image.new("RGB", (WIDTH, HEIGHT), color="black")
        self.draw = ImageDraw.Draw(self.image)
        self.tk_image = ImageTk.PhotoImage(image=self.image)
        
        # 创建Tkinter标签显示图像
        self.label = tk.Label(root, image=self.tk_image)
        self.label.pack()
        
        # 加载字体（修复核心错误）
        self.font = self.load_font()
        
        # 初始化圆形
        self.shapes = []
        self.init_shapes()
        
        # 帧率计算变量
        self.fps = 0
        self.frame_count = 0
        self.start_time = time.time()
        self.fps_text = ""
        
        # 开始动画循环
        self.animate()
    
    def load_font(self):
        """加载字体，确保兼容性"""
        try:
            # Ubuntu系统常见中文字体路径
            return ImageFont.truetype(
                "/usr/share/fonts/truetype/wqy/wqy-microhei.ttc", 
                12  # 字体大小
            )
        except:
            try:
                # 尝试其他中文字体
                return ImageFont.truetype(
                    "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc", 
                    12
                )
            except:
                # 找不到中文字体时使用默认字体
                return ImageFont.load_default()
    
    def init_shapes(self):
        """初始化所有圆形的属性"""
        for _ in range(NUM_SHAPES):
            x = random.randint(0, WIDTH)
            y = random.randint(0, HEIGHT)
            size = random.randint(10, 30)
            dx = random.uniform(-2, 2)
            dy = random.uniform(-2, 2)
            color = (
                random.randint(0, 255),
                random.randint(0, 255),
                random.randint(0, 255)
            )
            
            self.shapes.append({
                "x": x, "y": y, "size": size,
                "dx": dx, "dy": dy, "color": color
            })
    
    def draw_circle_outline(self, x, y, radius, color):
        """纯软件绘制圆形轮廓（中点圆算法）"""
        x = int(x)
        y = int(y)
        radius = int(radius)
        
        dx = radius
        dy = 0
        err = 0
        
        while dx >= dy:
            # 绘制8个对称点（像素级操作）
            self.draw.point((x + dx, y + dy), fill=color)
            self.draw.point((x + dy, y + dx), fill=color)
            self.draw.point((x - dy, y + dx), fill=color)
            self.draw.point((x - dx, y + dy), fill=color)
            self.draw.point((x - dx, y - dy), fill=color)
            self.draw.point((x - dy, y - dx), fill=color)
            self.draw.point((x + dy, y - dx), fill=color)
            self.draw.point((x + dx, y - dy), fill=color)
            
            if err <= 0:
                dy += 1
                err += 2 * dy + 1
            if err > 0:
                dx -= 1
                err -= 2 * dx + 1
    
    def update_positions(self):
        """更新所有圆形的位置"""
        for shape in self.shapes:
            shape["x"] += shape["dx"]
            shape["y"] += shape["dy"]
            
            # 边界检测
            if shape["x"] < shape["size"] or shape["x"] > WIDTH - shape["size"]:
                shape["dx"] *= -1
            if shape["y"] < shape["size"] or shape["y"] > HEIGHT - shape["size"]:
                shape["dy"] *= -1
    
    def calculate_fps(self):
        """计算帧率"""
        self.frame_count += 1
        elapsed = time.time() - self.start_time
        if elapsed >= 1:
            self.fps = int(self.frame_count / elapsed)
            self.frame_count = 0
            self.start_time = time.time()
            self.fps_text = f"FPS: {self.fps} | 图形数量: {NUM_SHAPES}"
    
    def animate(self):
        """动画循环：纯软件渲染每一帧"""
        # 清空图像（黑色背景）
        self.draw.rectangle([(0, 0), (WIDTH, HEIGHT)], fill="black")
        
        # 更新位置和绘制所有圆形
        self.update_positions()
        for shape in self.shapes:
            self.draw_circle_outline(
                shape["x"], shape["y"], shape["size"], shape["color"]
            )
        
        # 绘制帧率文本（使用正确的字体对象）
        if self.fps_text and self.font:
            self.draw.text((10, 10), self.fps_text, font=self.font, fill=(255, 255, 255))
        
        # 更新Tkinter显示
        self.tk_image.paste(self.image)
        self.label.config(image=self.tk_image)
        
        # 计算帧率
        self.calculate_fps()
        
        # 继续循环
        self.root.after(1, self.animate)

if __name__ == "__main__":
    root = tk.Tk()
    app = PillowRenderTest(root)
    root.mainloop()
