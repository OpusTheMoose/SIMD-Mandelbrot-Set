![fractal](https://github.com/user-attachments/assets/236155a1-143f-454b-87c7-7cfbc06abfa3)

## Mandelbrot Fractal Render Using SIMD

A C++ application that renders the Mandelbrot set by making use of SIMD. Libraries used include
- OpenGL (for rendering)
- SFML
- OpenMP
- HSVtoRGB (modified for SIMD)

### Algorithm

The [algorithm ](https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set#Unoptimized_na%C3%AFve_escape_time_algorithm) is simple, and was fully vectorized using SEE and AVX instructions. This gave a signficant 3-6x improvement in framerate.
A smooth coloring algorithm is then applied, which creates the color gradient for the set. It's then converted from HSV to RGB for rendering. Inital source code can be found [here](https://gist.github.com/fairlight1337/4935ae72bcbcc1ba5c72).

###Performance

To measure performance, I use a position of (-0.5, 0.0i) with a zoom of 4.0. The program was compiled on an AMD Ryzen 5 5600, with 12 threads. 
Single thread performance averages around 20 FPS, with multi-threaded averaging 95 FPS.
For the non-SIMD version, single thread performance averages 8 FPS, with multi-threaded averaging 33 FPS.
