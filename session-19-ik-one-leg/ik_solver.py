#!/usr/bin/env python3
"""
Session 19 - One Leg Inverse Kinematics Solver

Coordinate system:
- Origin (0,0,0) is the hip/coxa joint.
- X: forward
- Y: sideways
- Z: vertical, negative means below the hip / toward the ground.

Units:
- Lengths are centimetres.
- Returned IK angles are degrees.
"""

from __future__ import annotations

import csv
import json
import math
from dataclasses import dataclass
from pathlib import Path
from typing import Optional, Tuple


@dataclass(frozen=True)
class LegGeometry:
    """Physical leg segment lengths in centimetres."""
    L1: float = 2.5    # Coxa: hip pivot to femur pivot
    L2: float = 10.0   # Femur: upper leg
    L3: float = 12.0   # Tibia: lower leg / shin


@dataclass(frozen=True)
class ServoOffsets:
    """Servo calibration offsets and direction multipliers."""
    hip_offset: float = 90.0
    femur_offset: float = 90.0
    tibia_offset: float = 90.0

    hip_direction: float = 1.0
    femur_direction: float = 1.0
    tibia_direction: float = -1.0

    hip_min: float = 0.0
    hip_max: float = 180.0
    femur_min: float = 0.0
    femur_max: float = 180.0
    tibia_min: float = 30.0
    tibia_max: float = 150.0


DEFAULT_GEOMETRY = LegGeometry()
DEFAULT_OFFSETS = ServoOffsets()


def clamp(value: float, lo: float, hi: float) -> float:
    """Clamp value into [lo, hi]."""
    return max(lo, min(hi, value))


def leg_ik(
    x: float,
    y: float,
    z: float,
    geometry: LegGeometry = DEFAULT_GEOMETRY,
) -> Optional[Tuple[float, float, float]]:
    """
    Convert foot target position (x, y, z) into geometric joint angles.

    Args:
        x, y, z: Target foot position in cm, relative to hip joint.
                 z should be negative for ground contact.
        geometry: Leg segment lengths.

    Returns:
        (hip_deg, femur_deg, tibia_deg), or None if target is unreachable.
    """
    L1, L2, L3 = geometry.L1, geometry.L2, geometry.L3

    hip_rad = math.atan2(y, x)
    hip_deg = math.degrees(hip_rad)

    horizontal = math.sqrt(x * x + y * y)
    reach = horizontal - L1
    D = math.sqrt(reach * reach + z * z)

    if D <= 1e-9:
        return None

    if D > (L2 + L3) or D < abs(L2 - L3):
        return None

    cos_tib = (L2 * L2 + L3 * L3 - D * D) / (2 * L2 * L3)
    cos_tib = clamp(cos_tib, -1.0, 1.0)
    tibia_rad = math.acos(cos_tib)
    tibia_deg = math.degrees(tibia_rad)

    phi1 = math.atan2(-z, reach)
    cos_phi2 = (L2 * L2 + D * D - L3 * L3) / (2 * L2 * D)
    cos_phi2 = clamp(cos_phi2, -1.0, 1.0)
    phi2 = math.acos(cos_phi2)

    femur_rad = phi1 - phi2
    femur_deg = math.degrees(femur_rad)

    return (hip_deg, femur_deg, tibia_deg)


def load_offsets(path: str | Path = "offsets.json") -> ServoOffsets:
    """Load servo offsets from JSON. If missing, return defaults."""
    path = Path(path)
    if not path.exists():
        return DEFAULT_OFFSETS

    data = json.loads(path.read_text(encoding="utf-8"))

    servo = data.get("servo_offsets", data)
    limits = data.get("servo_limits", {})
    directions = data.get("servo_directions", {})

    def limit_pair(name: str, default_min: float, default_max: float) -> tuple[float, float]:
        raw = limits.get(name, [default_min, default_max])
        return float(raw[0]), float(raw[1])

    hip_min, hip_max = limit_pair("hip", DEFAULT_OFFSETS.hip_min, DEFAULT_OFFSETS.hip_max)
    femur_min, femur_max = limit_pair("femur", DEFAULT_OFFSETS.femur_min, DEFAULT_OFFSETS.femur_max)
    tibia_min, tibia_max = limit_pair("tibia", DEFAULT_OFFSETS.tibia_min, DEFAULT_OFFSETS.tibia_max)

    return ServoOffsets(
        hip_offset=float(servo.get("hip", DEFAULT_OFFSETS.hip_offset)),
        femur_offset=float(servo.get("femur", DEFAULT_OFFSETS.femur_offset)),
        tibia_offset=float(servo.get("tibia", DEFAULT_OFFSETS.tibia_offset)),
        hip_direction=float(directions.get("hip", DEFAULT_OFFSETS.hip_direction)),
        femur_direction=float(directions.get("femur", DEFAULT_OFFSETS.femur_direction)),
        tibia_direction=float(directions.get("tibia", DEFAULT_OFFSETS.tibia_direction)),
        hip_min=hip_min,
        hip_max=hip_max,
        femur_min=femur_min,
        femur_max=femur_max,
        tibia_min=tibia_min,
        tibia_max=tibia_max,
    )


def ik_to_servo(
    hip_deg: float,
    femur_deg: float,
    tibia_deg: float,
    offsets: ServoOffsets = DEFAULT_OFFSETS,
) -> Tuple[float, float, float]:
    """
    Convert geometric IK angles to physical servo angles.

    Adjust offsets/directions in offsets.json after real hardware calibration.
    """
    servo_hip = offsets.hip_offset + offsets.hip_direction * hip_deg
    servo_femur = offsets.femur_offset + offsets.femur_direction * femur_deg
    servo_tibia = offsets.tibia_offset + offsets.tibia_direction * tibia_deg

    servo_hip = clamp(servo_hip, offsets.hip_min, offsets.hip_max)
    servo_femur = clamp(servo_femur, offsets.femur_min, offsets.femur_max)
    servo_tibia = clamp(servo_tibia, offsets.tibia_min, offsets.tibia_max)

    return (servo_hip, servo_femur, servo_tibia)


def move_foot_to(leg, x: float, y: float, z: float) -> bool:
    """
    Full pipeline helper:
    foot target -> IK angles -> servo angles -> leg.move_to()

    'leg' must expose move_to((hip, femur, tibia)).
    """
    angles = leg_ik(x, y, z)
    if angles is None:
        print(f"Position ({x}, {y}, {z}) unreachable")
        return False

    servo_angles = ik_to_servo(*angles)
    leg.move_to(servo_angles)
    return True


def solve_position(name: str, x: float, y: float, z: float) -> dict:
    """Solve one named target and return a row dict for CSV logging."""
    angles = leg_ik(x, y, z)
    row = {
        "name": name,
        "x_cm": x,
        "y_cm": y,
        "z_cm": z,
        "reachable": angles is not None,
        "hip_ik_deg": "",
        "femur_ik_deg": "",
        "tibia_ik_deg": "",
        "hip_servo_deg": "",
        "femur_servo_deg": "",
        "tibia_servo_deg": "",
        "note": "",
    }

    if angles is None:
        row["note"] = "unreachable"
        return row

    servos = ik_to_servo(*angles)
    row.update({
        "hip_ik_deg": round(angles[0], 3),
        "femur_ik_deg": round(angles[1], 3),
        "tibia_ik_deg": round(angles[2], 3),
        "hip_servo_deg": round(servos[0], 3),
        "femur_servo_deg": round(servos[1], 3),
        "tibia_servo_deg": round(servos[2], 3),
        "note": "ok",
    })
    return row


def write_test_csv(path: str | Path = "ik_test_results.csv") -> None:
    """Generate a CSV with 10 reachable and 5 unreachable test targets."""
    targets = [
        ("HOME", 12.5, 0.0, -8.0),
        ("STAND", 12.5, 0.0, -10.0),
        ("LIFT", 12.5, 0.0, -4.0),
        ("EXTEND", 17.0, 0.0, -8.0),
        ("RETRACT", 8.5, 0.0, -8.0),
        ("FORWARD_LEFT", 14.0, 3.0, -8.0),
        ("FORWARD_RIGHT", 14.0, -3.0, -8.0),
        ("HIGH_FORWARD", 16.0, 0.0, -5.0),
        ("LOW_CLOSE", 9.0, 2.0, -11.0),
        ("SIDE_STEP", 12.0, 5.0, -8.0),
        ("UNREACH_FAR", 30.0, 0.0, -30.0),
        ("UNREACH_TOO_FAR_X", 40.0, 0.0, -8.0),
        ("UNREACH_TOO_HIGH", 1.0, 0.0, 0.0),
        ("UNREACH_TOO_CLOSE", 2.5, 0.0, 0.0),
        ("UNREACH_FAR_SIDE", 0.0, 35.0, -10.0),
    ]

    rows = [solve_position(name, x, y, z) for name, x, y, z in targets]
    fieldnames = list(rows[0].keys())

    with Path(path).open("w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows)


def main() -> None:
    write_test_csv()
    print("Wrote ik_test_results.csv")


if __name__ == "__main__":
    main()
