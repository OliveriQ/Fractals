package newton

import (
	"log"
	"math"
	"os"
	"strconv"
)

const (
	MaxIterations uint8   = 255
	Height        uint16  = 600
	Width         uint16  = 600
	Off           float64 = 0.001
)

// map pixel to the range [-2, 2]
func map_pixel(x, y float64) complex128 {
	c_real := (4/float64(Width))*float64(x) - 2
	c_imag := (4/float64(Height))*float64(y) - 2

	return complex(c_real, c_imag)
}

//f(x) = x^3 - 1
//(a + bi)^3 = a^3 - 3*a*b^2 + (3*a^2*b - b^3)i
func f(z complex128) complex128 {
	c_real := real(z)*real(z)*real(z) - 3*real(z)*imag(z)*imag(z) - 1
	c_imag := 3*real(z)*real(z)*imag(z) - imag(z)*imag(z)*imag(z)

	return complex(c_real, c_imag)
}

//f'(x) = 3x^2
//(a + bi)^2 = a^2 - b^2 + (2ab)i
func df(z complex128) complex128 {
	c_real := 3 * (real(z)*real(z) - imag(z)*imag(z))
	c_imag := 3 * (2 * real(z) * imag(z))

	return complex(c_real, c_imag)
}

// for writing to ppm file
func write(file *os.File, text string) {
	_, err := file.WriteString(text)

	if err != nil {
		log.Fatal(err)
	}
}

func Plot() {
	file, err := os.Create("newton/newton.ppm")

	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	write(file, "P3\n")
	write(file, strconv.Itoa(int(Width))+" ")
	write(file, strconv.Itoa(int(Height))+"\n")
	write(file, "255\n")

	roots := [3]complex128{}
	roots[0] = complex(1, 0)
	roots[1] = complex(-0.5, math.Sqrt(3)/2)
	roots[2] = complex(-0.5, -math.Sqrt(3)/2)

	for y := 0; y < int(Height); y++ {
		for x := 0; x < int(Width); x++ {
			z := map_pixel(float64(x), float64(y))

			// apply newton's method
			for idx := 0; idx < int(MaxIterations); idx++ {
				z = z - f(z)/df(z)
			}

			for idx, root := range roots {
				if math.Abs(real(z)-real(root)) < Off {
					if math.Abs(imag(z)-imag(root)) < Off {
						switch idx {
						case 0:
							write(file, strconv.Itoa(255)+" ")
							write(file, strconv.Itoa(0)+" ")
							write(file, strconv.Itoa(0)+"\n")
						case 1:
							write(file, strconv.Itoa(0)+" ")
							write(file, strconv.Itoa(255)+" ")
							write(file, strconv.Itoa(0)+"\n")
						case 2:
							write(file, strconv.Itoa(0)+" ")
							write(file, strconv.Itoa(0)+" ")
							write(file, strconv.Itoa(255)+"\n")
						}
					}
				}
			}
		}
	}
}
