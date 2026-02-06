import csv
import time
from datetime import datetime
from pathlib import Path

from arduino.app_utils import App, Bridge

DATA_DIR = Path("home/data")
DATA_DIR.mkdir(parents=True,exist_ok=True)
CONFIG_PATH = Path(__file__).resolve().parent[1] /"sketch"/"CONFIG.h"


def _build_log_path(now: datetime) -> Path:
    base_name = now.strftime("data_%Y%m%d_%H")
    candidate = DATA_DIR / f"{base_name}.csv"
    counter = 1
    while candidate.exists():
        candidate = DATA_DIR / f"{base_name}_{counter}.csv"
        counter += 1
    return candidate


LOG_PATH = _build_log_path(datetime.now())


def _read_config_lines() -> list[str] :
    if not CONFIG_PATH.exists():
        return [f"CONFIG file not found:{CONFIG_PATH}"]
    for line in CONFIG_PATH.read_text().splitlines():
        stripped = line.strip()
        if not stripped :
            continue
        config_lines.append(stripped)
    return config_lines

if not LOG_PATH.exists():
    with LOG_PATH.open("w", newline="") as log_file:
        
        log_file.write("# CONFIG.h snapshot before experiment\n")
        
        for config_line in read_config_lines():
            log_file.write(f"#{config_line}\n")
            
        log_file.write("\n")
        
        writer = csv.writer(log_file)
        writer.writerow(
            [
                "timestamp_ms",
                "latitude",
                "longitude",
                "heading",
                "rudder_angle",
                "awa",
                "filtered_awa",
                "unmanned_status",
            ]
        )

print(f"Logging data to {LOG_PATH}")


def log_data(
    timestamp_ms,
    latitude,
    longitude,
    heading,
    rudder_angle,
    awa,
    filtered_awa,
    unmanned_status,
):
    with LOG_PATH.open("a", newline="") as log_file:
        writer = csv.writer(log_file)
        writer.writerow(
            [
                timestamp_ms,
                latitude,
                longitude,
                heading,
                rudder_angle,
                awa,
                filtered_awa,
                unmanned_status,
            ]
        )


Bridge.provide("log_data", log_data)


def loop():
    """This function is called repeatedly by the App framework."""
    time.sleep(1)

App.run(user_loop=loop)