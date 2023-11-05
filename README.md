# cl-renderer

OpenCL based rendering application.

![](docs/rescaled-2.jpg)

I've made this project to experiment, primarily, with ray marching/path tracing. Fractal renderering. Things like this.

This is a, very much, work in progress project.

## Getting Started

### Prequisites
- CMake https://cmake.org/
- OpenCL runtime.
- vcpkg https://github.com/microsoft/vcpkg (optional)
    - This will automatically be cloned and bootstrapped by this project's CMake configure step. Into the project's build folder.
    - You can manually clone and bootstrap vcpkg yourself if you like.
        - Use CMake argument `-D VCPKG_ROOT=<path to vcpkg root directory>`
        - Or set the `VCPKG_ROOT` environment variable to vcpkg root directory.

### Building
```
$ clone https://github.com/matthewgattis/cl-renderer.git
$ mkdir cl-renderer/build
$ cd cl-renderer/build
$ cmake -D CMAKE_BUILD_TYPE=Release ../
$ cmake --build .
```

### Render Test Image
- In the build folder run `project.py` python script to generate a new `device.json` and `project.json`.
- `device.json`
```json
{
    "platform": "NVIDIA CUDA",
    "device": "Quadro RTX 3000"
}
```
- Use application like `clinfo` to identify platforms/devices available on your system.
- `project.json`
```json
{
    "program": "program.cl",                    // Name of CL program file to load.
    "kernel": "mainimage",                      // Name of kernel entry point.
    "image_count": 1,                           // Total number of images to generate.
    "output": "frame_20231031_095855_%02d.png", // Name of output image.
    "width": 1280,                              // Image width.
    "height": 720,                              // Image height.
    "tile_size": 128,                           // Size of image to work on at any time.
    "samples": 64                               // Multi sample count.
}
```
- Adjust options as desired.
- The included `program.cl` will render the test image seen above. Copy it to the build folder as well.
- Run `cl-renderer` in same folder as config files.
```bash
$ python ../project.py
$ cp ../program.cl ./
$ ./cl-rendrerer
```
