# Game of Life
Conway's game of life using GLFW.

## Dependencies

- glew
- glfw
- opengl

```
sudo apt install libglfw3-dev libglew-dev libgl1-mesa-dev
```

## Building

```
mkdir build
cd build
cmake ..
make
```

## Running

```
./main [number of jobs] [file wih a 64x28 of initial state]
```
