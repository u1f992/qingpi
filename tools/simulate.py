"""
Vin
│
U1───┐
│    │
x    │
     U3───Vout
x    │
│    │
U2───┘
│
GND
"""

import json


R_ab = 1_000
R_w = 50


def u1_r_aw(d: int):
    return ((256 - d) / 256) * R_ab + R_w


def u2_r_wb(d: int):
    return (d / 256) * R_ab + R_w


def u3_r_wb_w(d: int):
    return (d / 256) * R_ab


def v_out_ratio(u1_u2_d: int, u3_d: int):
    return (u2_r_wb(u1_u2_d) + u3_r_wb_w(u3_d)) / (
        u1_r_aw(u1_u2_d) + R_ab + u2_r_wb(u1_u2_d)
    )


def filter_duplicates_v_out(data: list[dict[str, int | float]]):
    result = []
    seen = set()
    for d in data:
        if d["v_out"] not in seen:
            result.append(d)
            seen.add(d["v_out"])
    return result


ret = filter_duplicates_v_out(
    sorted(
        [
            {
                "u1_u2_d": u1_u2_d,
                "u3_d": u3_d,
                "v_out": round(v_out_ratio(u1_u2_d, u3_d) * 1.8, 5),
            }
            for u1_u2_d in range(0, 256)
            for u3_d in range(0, 256)
        ],
        key=lambda x: x["v_out"],
    )
)

with open("result.json", mode="w") as f:
    f.write(json.dumps(ret))
