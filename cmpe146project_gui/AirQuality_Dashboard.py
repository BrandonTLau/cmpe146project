import tkinter as tk
import collections
import threading
import queue
import serial
 
class AirQualityDashboard:
    def __init__(self, root):
        self.root = root
        self.root.title("S-RAQMS Live Telemetry")
        self.root.configure(bg="#1e1e1e")
 
        self.aqi_data    = collections.deque([0]*30, maxlen=30)
        self.uptime_ms   = 0
        self.current_aqi = 0
        self.secure_data = "---"
        self.checksum    = "---"
        self.data_queue  = queue.Queue()
        self.serial_port = None
 
        tk.Label(root, text="S-RAQMS Live Telemetry",
                 font=("Courier", 18, "bold"),
                 fg="#00FF99", bg="#1e1e1e").pack(pady=(15, 5))
 
        self.label_aqi = tk.Label(root, text="AQI: ---",
                                  font=("Courier", 28, "bold"),
                                  fg="white", bg="#1e1e1e")
        self.label_aqi.pack(pady=5)
 
        self.canvas_width  = 560
        self.canvas_height = 200
        self.canvas = tk.Canvas(root, width=self.canvas_width,
                                height=self.canvas_height,
                                bg="#111111", highlightthickness=0)
        self.canvas.pack(padx=20, pady=10)
 
        info_frame = tk.Frame(root, bg="#1e1e1e")
        info_frame.pack(fill="x", padx=20, pady=5)
 
        self.label_uptime = tk.Label(info_frame, text="Uptime:      ---",
                                     font=("Courier", 11), fg="#aaaaaa", bg="#1e1e1e", anchor="w")
        self.label_uptime.pack(fill="x")
 
        self.label_secure = tk.Label(info_frame, text="Secure Data: ---",
                                     font=("Courier", 11), fg="#aaaaaa", bg="#1e1e1e", anchor="w")
        self.label_secure.pack(fill="x")
 
        self.label_crc = tk.Label(info_frame, text="Checksum:    ---",
                                  font=("Courier", 11), fg="#aaaaaa", bg="#1e1e1e", anchor="w")
        self.label_crc.pack(fill="x")
 
        self.label_status = tk.Label(root, text="Connecting to COM3...",
                                     font=("Courier", 10), fg="#ffaa00", bg="#1e1e1e")
        self.label_status.pack(pady=(5, 15))
 
        threading.Thread(target=self.read_serial, daemon=True).start()
        self.update_ui()
 
    def read_serial(self):
        try:
            self.serial_port = serial.Serial("COM3", 115200, timeout=2)
            self.data_queue.put(("status", "Connected to COM3 at 115200 baud"))
        except Exception as e:
            self.data_queue.put(("status", f"ERROR: {e}"))
            return
 
        buffer = {}
        while True:
            try:
                raw = self.serial_port.readline()
                if not raw:
                    continue
                line = raw.decode("utf-8", errors="ignore").strip()
                if not line:
                    continue
 
                if "System Uptime:" in line:
                    buffer = {}
                    try:
                        buffer["uptime"] = int(line.split("System Uptime:")[1].strip().replace("ms","").strip())
                    except:
                        pass
 
                elif "Secure Data:" in line:
                    try:
                        buffer["secure"] = line.split("Secure Data:")[1].strip()
                    except:
                        pass
 
                elif "Checksum:" in line:
                    try:
                        buffer["checksum"] = line.split("Checksum:")[1].strip()
                    except:
                        pass
 
                elif "AQI:" in line and "Secure" not in line:
                    try:
                        buffer["aqi"] = int(line.split("AQI:")[1].strip())
                    except:
                        pass
                    if "aqi" in buffer:
                        self.data_queue.put(("packet", dict(buffer)))
                    buffer = {}
 
            except Exception:
                continue
 
    def update_ui(self):
        while not self.data_queue.empty():
            kind, payload = self.data_queue.get()
 
            if kind == "status":
                color = "#00FF99" if "Connected" in payload else "#ff4444"
                self.label_status.config(text=payload, fg=color)
 
            elif kind == "packet":
                self.current_aqi = payload.get("aqi", self.current_aqi)
                self.uptime_ms   = payload.get("uptime", self.uptime_ms)
                self.secure_data = payload.get("secure", self.secure_data)
                self.checksum    = payload.get("checksum", self.checksum)
 
                self.aqi_data.append(self.current_aqi)
 
                if self.current_aqi <= 50:
                    color = "#00FF99"
                elif self.current_aqi <= 100:
                    color = "#ffff00"
                elif self.current_aqi <= 150:
                    color = "#ff9900"
                else:
                    color = "#ff4444"
 
                self.label_aqi.config(text=f"AQI: {self.current_aqi}", fg=color)
                self.label_uptime.config(text=f"Uptime:      {self.uptime_ms} ms")
                self.label_secure.config(text=f"Secure Data: {self.secure_data}")
                self.label_crc.config(text=f"Checksum:    {self.checksum}")
                self.draw_graph()
 
        self.root.after(100, self.update_ui)
 
    def draw_graph(self):
        self.canvas.delete("all")
        w, h  = self.canvas_width, self.canvas_height
        pad   = 10
        max_v = 500
        data  = list(self.aqi_data)
        n     = len(data)
        if n < 2:
            return
 
        for level, lbl in [(100,"100"),(200,"200"),(300,"300"),(400,"400")]:
            y = h - pad - (level / max_v) * (h - 2*pad)
            self.canvas.create_line(pad, y, w-pad, y, fill="#333333", dash=(4,4))
            self.canvas.create_text(pad+2, y-8, text=lbl,
                                    fill="#555555", font=("Courier",8), anchor="w")
 
        pts = []
        for i, v in enumerate(data):
            x = pad + i * (w - 2*pad) / (n - 1)
            y = h - pad - (min(v, max_v) / max_v) * (h - 2*pad)
            pts.extend([x, y])
 
        if len(pts) >= 4:
            self.canvas.create_line(*pts, fill="#00FF99", width=2, smooth=True)
 
        self.canvas.create_oval(pts[-2]-4, pts[-1]-4,
                                pts[-2]+4, pts[-1]+4,
                                fill="#00FF99", outline="white")
 
 
if __name__ == "__main__":
    root = tk.Tk()
    app  = AirQualityDashboard(root)
    root.mainloop()