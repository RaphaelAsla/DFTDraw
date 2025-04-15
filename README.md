# DFTDraw

<p align="center">
  <img src="images/shape.png" width="500" />
</p>

Used the Discrete Fourier Transform to approximate shapes, using waves. <br>
The spinning circles (epicycles) represent different frequencies, and when added together, they form the shape. <br>
The more frequencies, the better. In our case, though, we always have the same number of frequencies as sample points, so the result is always an exact replica of the shape.

## Compilation
```bash
mkdir build && cd build
cmake .. && make
./dft
```
