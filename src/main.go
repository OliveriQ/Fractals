package main

import (
	"fractals/src/mandelbrot"
	"fractals/src/newton"
)

func main() {
	mandelbrot.Plot()
	newton.Plot()
}
