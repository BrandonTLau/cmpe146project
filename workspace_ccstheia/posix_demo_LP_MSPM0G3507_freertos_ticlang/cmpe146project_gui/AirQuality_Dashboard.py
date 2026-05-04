import serial
import tkinter as tk
import collections
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure

class AirQualityDashboard:
    def __init__(self, root):
        self.root = root
        self.root.title("AQI Live Telemetry - Debug Mode")
        self.data = collections.deque([0]*50, maxlen=50)
        
        self.label_val = tk.Label(root, text="Awaiting UART Stream...", font=("Arial", 25))
        self.label_val.pack(pady=20)
        
        self.fig = Figure(figsize=(5, 3))
        self.ax = self.fig.add_subplot(111)
        self.ax.set_ylim(0, 1100)
        self.line, = self.ax.plot(self.data, color='green')
        self.canvas = FigureCanvasTkAgg(self.fig, master=root)
        self.canvas.get_tk_widget().pack()

        self.ser = None
        self.connect()

    def connect(self):
        try:
            # Connect to COM4 at 115200 Baud
            self.ser = serial.Serial('COM4', 115200, timeout=0.1)
            print("--- PORT OPENED: COM4 ---")
            self.update_plot()
        except Exception as e:
            print(f"Connection Error: {e}")
            self.root.after(2000, self.connect)

    def update_plot(self):
        if self.ser and self.ser.in_waiting > 0:
            # Read all available bytes in the buffer
            raw_bytes = self.ser.read(self.ser.in_waiting)
            print(f"DATA DETECTED: {raw_bytes}")
            
            try:
                decoded = raw_bytes.decode('utf-8').strip()
                # If we have multiple numbers in one chunk, take the last one
                parts = decoded.split('\n')
                last_val = parts[-1] if parts[-1].isdigit() else (parts[-2] if len(parts)>1 and parts[-2].isdigit() else None)
                
                if last_val:
                    val = int(last_val)
                    self.data.append(val)
                    self.label_val.config(text=f"Current AQI: {val}")
                    self.line.set_ydata(self.data)
                    self.canvas.draw()
            except:
                pass
        
        self.root.after(50, self.update_plot)

if __name__ == "__main__":
    root = tk.Tk()
    app = AirQualityDashboard(root)
    root.mainloop()