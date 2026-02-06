import csv
import os
import time
from datetime import datetime
from pathlib import Path

from arduino.app_utils import App, Bridge

def _resolve_data_dir() -> Path:
    candidates: list[Path] = []

    env_data_dir = os.getenv("DATA_DIR")
    if env_data_dir:
        candidates.append(Path(env_data_dir))

    candidates.extend(
        [
            Path("/home/data"),
            Path(__file__).resolve().parents[1] / "data",
            Path("/tmp/pilote-automatique-data"),
        ]
    )

    for candidate in candidates:
        try:
            candidate.mkdir(parents=True, exist_ok=True)
            return candidate
        except PermissionError:
            continue

    raise PermissionError(
        "Unable to create a writable data directory. "
        "Set DATA_DIR to a writable path."
    )


DATA_DIR = _resolve_data_dir()
CONFIG_PATH = Path(__file__).resolve().parents[1] / "sketch" / "CONFIG.h"


def _build_log_path(now: datetime) -> Path:
    base_name = now.strftime("data_%Y%m%d_%H")
    candidate = DATA_DIR / f"{base_name}.csv"
    counter = 1
    while candidate.exists():
        candidate = DATA_DIR / f"{base_name}_{counter}.csv"
        counter += 1
    return candidate


LOG_PATH = _build_log_path(datetime.now())


def _read_config_lines() -> list[str]:
    if not CONFIG_PATH.exists():
        return [f"CONFIG file not found: {CONFIG_PATH}"]

    config_lines: list[str] = []
    for line in CONFIG_PATH.read_text(encoding="utf-8").splitlines():
        stripped = line.strip()
        if stripped:
            config_lines.append(stripped)

    return config_lines


if not LOG_PATH.exists():
    with LOG_PATH.open("w", newline="", encoding="utf-8") as log_file:
        log_file.write("# CONFIG.h snapshot before experiment\n")

        for config_line in _read_config_lines():
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
    with LOG_PATH.open("a", newline="", encoding="utf-8") as log_file:
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
