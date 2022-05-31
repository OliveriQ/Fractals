package mandelbrot

import (
	"log"
	"math"
	"os"
	"strconv"
)

const (
	MaxIterations uint8  = 255
	Height        uint16 = 600
	Width         uint16 = 600
)

// map pixel to the range [-2, 2]
func map_pixel(x, y float64) complex128 {
	c_real := (4/float64(Width))*float64(x) - 2
	c_imag := (4/float64(Height))*float64(y) - 2

	return complex(c_real, c_imag)
}

// (a + bi)^2 = a^2 - b^2 + 2abi
func next_complex(z complex128) complex128 {
	c_real := math.Pow(real(z), 2) - math.Pow(imag(z), 2)
	c_imag := 2 * real(z) * imag(z)

	return complex(c_real, c_imag)
}

// calculated magnitude relative to origin
func magnitude(z complex128) float64 {
	return math.Sqrt(math.Pow(real(z), 2) + math.Pow(imag(z), 2))
}

// for writing to ppm file
func write(f *os.File, text string) {
	_, err := f.WriteString(text)

	if err != nil {
		log.Fatal(err)
	}
}

func Plot() {
	f, err := os.Create("../mandelbrot.ppm")

	if err != nil {
		log.Fatal(err)
	}

	defer f.Close()

	write(f, "P3\n")
	write(f, strconv.Itoa(int(Width))+" ")
	write(f, strconv.Itoa(int(Height))+"\n")
	write(f, "255\n")

	spectrum := [255]uint8{}

	for idx := 0; idx < int(MaxIterations); idx++ {
		spectrum[idx] = uint8(idx)
	}

	for y := 0; y < int(Height); y++ {
		for x := 0; x < int(Width); x++ {
			c := map_pixel(float64(x), float64(y))
			z := complex(0, 0)

			var iters uint8 = 0
			for magnitude(z) <= 2 && iters < MaxIterations {
				z = next_complex(z) + c
				iters++
			}

			if iters < MaxIterations {
				write(f, strconv.Itoa(int(spectrum[iters]))+" ")
				write(f, strconv.Itoa(int(spectrum[iters]))+" ")
				write(f, strconv.Itoa(100)+"\n")

			} else {
				write(f, strconv.Itoa(0)+" ")
				write(f, strconv.Itoa(0)+" ")
				write(f, strconv.Itoa(0)+"\n")
			}
		}
	}

}
