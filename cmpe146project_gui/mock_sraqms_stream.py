import time
import random
import binascii
import os

# --- Configuration ---
# Set to True to output raw numbers to test your graph drawing easily.
# Set to False to output the exact encrypted UART format from your design doc.
TEST_PLAINTEXT_MODE = True 

# Time between simulated UART messages (in seconds)
# Your doc says it logs every 60s, but 1s is better for testing the UI!
UPDATE_INTERVAL_SEC = 1.0 

def generate_mock_data():
    uptime_ms = 0
    # Start with a baseline AQI and add a "random walk" to simulate a real sensor graph
    current_aqi = 50.0 

    print("Starting S-RAQMS mock data stream...")
    print("Press Ctrl+C to stop.\n")

    try:
        while True:
            # Simulate sensor drift (Moving Average Filter behavior)
            current_aqi += random.uniform(-5.0, 6.0)
            current_aqi = max(0, min(500, current_aqi)) # Keep within standard AQI bounds

            if TEST_PLAINTEXT_MODE:
                # Easy format for testing matplotlib/pyqtgraph lines
                print(f"Uptime: {uptime_ms} ms | Raw AQI: {current_aqi:.2f}")
            
            else:
                # Strict format matching your UART Task requirements
                # 1. Generate fake 16-byte encrypted hex string
                fake_encrypted_packet = os.urandom(16).hex().upper()
                
                # 2. Calculate actual CRC-32 of that hex string
                crc = binascii.crc32(fake_encrypted_packet.encode()) & 0xFFFFFFFF
                
                # 3. Print exact UART format
                print(f"System Uptime: {uptime_ms}")
                print(f"Secure Data: {fake_encrypted_packet}")
                print(f"Checksum: {crc}")
                print("-" * 30)

            # Flush standard output so your GUI can read it in real-time if using subprocess
            import sys
            sys.stdout.flush()

            time.sleep(UPDATE_INTERVAL_SEC)
            uptime_ms += int(UPDATE_INTERVAL_SEC * 1000)

    except KeyboardInterrupt:
        print("\nMock data stream stopped.")

if __name__ == "__main__":
    generate_mock_data()
