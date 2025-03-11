#!/usr/bin/env python3

import time

INTERFACE = "wlan0"  # Change to your network interface (e.g., eth0, enp0s3)

def get_bytes():
    """Read received (rx) and transmitted (tx) bytes."""
    with open(f"/sys/class/net/{INTERFACE}/statistics/rx_bytes", "r") as f:
        rx = int(f.read().strip())
    with open(f"/sys/class/net/{INTERFACE}/statistics/tx_bytes", "r") as f:
        tx = int(f.read().strip())
    return rx, tx

def main():
    # Get initial bytes and timestamp
    rx_prev, tx_prev = get_bytes()
    time_prev = time.time()

    while True:
        # Wait for a short interval
        time.sleep(0.5)  # Adjust this for faster/slower updates

        # Get new bytes and timestamp
        rx_now, tx_now = get_bytes()
        time_now = time.time()

        # Calculate time difference
        time_delta = time_now - time_prev

        # Calculate download and upload speed in KB/s
        rx_speed = (rx_now - rx_prev) / 1024 / time_delta
        tx_speed = (tx_now - tx_prev) / 1024 / time_delta

        # Output the speeds
        print(f" {rx_speed:.2f} KB/s  {tx_speed:.2f} KB/s", flush=True)

        # Update previous values
        rx_prev, tx_prev = rx_now, tx_now
        time_prev = time_now

if __name__ == "__main__":
    main()
