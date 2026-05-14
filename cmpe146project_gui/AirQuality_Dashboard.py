import tkinter as tk
import collections
import subprocess
import threading
import queue
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure

class AirQualityDashboard:
    def __init__(self, root):
        self.root = root
        self.root.title("AQI Live Telemetry - Debug Mode")
        self.data = collections.deque([0]*50, maxlen=50)
        
        self.label_val = tk.Label(root, text="Awaiting Mock Stream...", font=("Arial", 25))
        self.label_val.pack(pady=20)
        
        self.fig = Figure(figsize=(5, 3))
        self.ax = self.fig.add_subplot(111)
        # Adjusted Y-limit to match the 0-500 mock AQI generation bounds
        self.ax.set_ylim(0, 500) 
        self.line, = self.ax.plot(self.data, color='green')
        self.canvas = FigureCanvasTkAgg(self.fig, master=root)
        self.canvas.get_tk_widget().pack()

        self.mock_process = None
        self.data_queue = queue.Queue()
        self.connect()

    def connect(self):
        try:
            # Start the mock script as a subprocess instead of a serial connection
            self.mock_process = subprocess.Popen(
                ['python', 'mock_sraqms_stream.py'], 
                stdout=subprocess.PIPE, 
                text=True,
                bufsize=1 # Line buffered
            )
            print("--- MOCK STREAM OPENED ---")
            
            # Start a background thread to read the stream without freezing the GUI
            threading.Thread(target=self.read_stream, daemon=True).start()
            
            # Start the GUI update loop
            self.update_plot()
            
        except Exception as e:
            print(f"Connection Error: {e}")
            self.root.after(2000, self.connect)

    def read_stream(self):
        # Continuously read lines from the mock script
        for line in iter(self.mock_process.stdout.readline, ''):
            self.data_queue.put(line.strip())

    def update_plot(self):
        # Process all available lines in the queue
        while not self.data_queue.empty():
            line = self.data_queue.get()
            print(f"DATA DETECTED: {line}")
            
            try:
                # Parse the mock script output format: "Uptime: X ms | Raw AQI: Y.YY"
                if "Raw AQI:" in line:
                    val_str = line.split("Raw AQI:")[1].strip()
                    val = float(val_str)
                    
                    self.data.append(val)
                    self.label_val.config(text=f"Current AQI: {val:.2f}")
                    self.line.set_ydata(self.data)
                    self.canvas.draw()
            except Exception as e:
                print(f"Parsing Error: {e}")
        
        self.root.after(50, self.update_plot)

if __name__ == "__main__":
    root = tk.Tk()
    app = AirQualityDashboard(root)
    root.mainloop()
