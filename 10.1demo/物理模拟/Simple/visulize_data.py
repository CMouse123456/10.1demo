import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def plot_physics_data(csv_file):
    # 读取数据
    df = pd.read_csv(csv_file)
    
    plt.plot(df["vy"])

    plt.show()

if __name__ == "__main__":
    plot_physics_data("ball_simulation.csv")