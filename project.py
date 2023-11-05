import json
from datetime import datetime

dts = datetime.now().strftime("%Y%m%d_%H%M%S")

project = {
    'program': 'program.cl',
    'kernel': 'mainimage',
    'image_count': 1,
    'output': 'frame_' + dts + '_%02d.png',
    'width': 1280,
    'height': 720,
    'tile_size': 128,
    'samples': 64
}

device = {
    'platform': 'NVIDIA CUDA',
    'device': 'Quadro RTX 3000'
}

a = json.dumps(project, indent=4)
with open('project.json', 'w') as f:
    f.write(a)

a = json.dumps(device, indent=4)
with open('device.json', 'w') as f:
    f.write(a)

