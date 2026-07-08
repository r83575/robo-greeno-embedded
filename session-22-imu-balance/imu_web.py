import time
import math
from smbus2 import SMBus
from flask import Flask, jsonify, render_template_string

MPU_ADDR = 0x68
PWR_MGMT_1 = 0x6B
ACCEL_XOUT_H = 0x3B

bus = SMBus(1)
bus.write_byte_data(MPU_ADDR, PWR_MGMT_1, 0)

app = Flask(__name__)

def read_word(reg):
    high = bus.read_byte_data(MPU_ADDR, reg)
    low = bus.read_byte_data(MPU_ADDR, reg + 1)
    value = (high << 8) + low
    if value >= 0x8000:
        value -= 65536
    return value

def read_imu():
    ax = read_word(ACCEL_XOUT_H) / 16384.0
    ay = read_word(ACCEL_XOUT_H + 2) / 16384.0
    az = read_word(ACCEL_XOUT_H + 4) / 16384.0

    gx = read_word(ACCEL_XOUT_H + 8) / 131.0
    gy = read_word(ACCEL_XOUT_H + 10) / 131.0
    gz = read_word(ACCEL_XOUT_H + 12) / 131.0

    pitch = math.degrees(math.atan2(ax, math.sqrt(ay * ay + az * az)))
    roll = math.degrees(math.atan2(ay, math.sqrt(ax * ax + az * az)))

    tilt = max(abs(pitch), abs(roll))
    if tilt < 10:
        status = "SAFE"
    elif tilt < 15:
        status = "WARNING"
    else:
        status = "DANGER"

    return {
        "ax": ax, "ay": ay, "az": az,
        "gx": gx, "gy": gy, "gz": gz,
        "pitch": pitch,
        "roll": roll,
        "status": status
    }

HTML = """
<!doctype html>
<html>
<head>
  <meta charset="utf-8">
  <title>MPU6050 IMU Monitor</title>
  <style>
    body { font-family: Arial; text-align: center; background: #111; color: white; }
    .row { display: flex; justify-content: center; gap: 30px; margin-top: 15px; }
    .card { background: #222; padding: 15px 25px; border-radius: 10px; min-width: 180px; }
    .value { font-size: 28px; }
    #status { font-size: 42px; font-weight: bold; margin: 15px; }
    canvas { background: #000; border: 2px solid #555; margin-top: 20px; }
  </style>
</head>
<body>
  <h1>MPU6050 Real Time Monitor</h1>

  <div class="row">
    <div class="card">
      <h2>Pitch</h2>
      <div class="value"><span id="pitch">0</span>°</div>
    </div>
    <div class="card">
      <h2>Roll</h2>
      <div class="value"><span id="roll">0</span>°</div>
    </div>
  </div>

  <div id="status">SAFE</div>

  <canvas id="canvas" width="500" height="300"></canvas>

  <div class="row">
    <div class="card">
      <h2>Accel</h2>
      <div>X: <span id="ax">0</span></div>
      <div>Y: <span id="ay">0</span></div>
      <div>Z: <span id="az">0</span></div>
    </div>

    <div class="card">
      <h2>Gyro</h2>
      <div>X: <span id="gx">0</span></div>
      <div>Y: <span id="gy">0</span></div>
      <div>Z: <span id="gz">0</span></div>
    </div>
  </div>

<script>
const canvas = document.getElementById("canvas");
const ctx = canvas.getContext("2d");

function drawHorizon(roll, pitch) {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    const cx = canvas.width / 2;
    const cy = canvas.height / 2;
    const pitchOffset = pitch * 3;
    const angle = roll * Math.PI / 180;

    ctx.save();
    ctx.translate(cx, cy + pitchOffset);
    ctx.rotate(-angle);

    ctx.fillStyle = "#1d5fa7";
    ctx.fillRect(-600, -600, 1200, 600);

    ctx.fillStyle = "#6b4f2a";
    ctx.fillRect(-600, 0, 1200, 600);

    ctx.strokeStyle = "white";
    ctx.lineWidth = 4;
    ctx.beginPath();
    ctx.moveTo(-600, 0);
    ctx.lineTo(600, 0);
    ctx.stroke();

    ctx.restore();

    ctx.strokeStyle = "yellow";
    ctx.lineWidth = 3;
    ctx.beginPath();
    ctx.moveTo(cx - 60, cy);
    ctx.lineTo(cx - 15, cy);
    ctx.moveTo(cx + 15, cy);
    ctx.lineTo(cx + 60, cy);
    ctx.stroke();

    ctx.beginPath();
    ctx.arc(cx, cy, 5, 0, Math.PI * 2);
    ctx.fillStyle = "yellow";
    ctx.fill();
}

async function update() {
    const res = await fetch("/data");
    const data = await res.json();

    document.getElementById("pitch").textContent = data.pitch.toFixed(1);
    document.getElementById("roll").textContent = data.roll.toFixed(1);

    document.getElementById("ax").textContent = data.ax.toFixed(2);
    document.getElementById("ay").textContent = data.ay.toFixed(2);
    document.getElementById("az").textContent = data.az.toFixed(2);

    document.getElementById("gx").textContent = data.gx.toFixed(2);
    document.getElementById("gy").textContent = data.gy.toFixed(2);
    document.getElementById("gz").textContent = data.gz.toFixed(2);

    const status = document.getElementById("status");
    status.textContent = data.status;

    if (data.status === "SAFE") status.style.color = "lime";
    else if (data.status === "WARNING") status.style.color = "orange";
    else status.style.color = "red";

    drawHorizon(data.roll, data.pitch);
}

setInterval(update, 200);
</script>
</body>
</html>
"""

@app.route("/")
def index():
    return render_template_string(HTML)

@app.route("/data")
def data():
    return jsonify(read_imu())

app.run(host="0.0.0.0", port=5000)
