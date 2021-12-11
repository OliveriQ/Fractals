#include <iostream>
#include <fstream>
#include <complex>
#include <cmath>

#define MAX_ITER 256
#define HEIGHT 540
#define WIDTH 960

typedef std::complex<double> Complex;

class Mandelbrot
{
  public:
    Mandelbrot();

  private:
    std::ofstream fout{"output.ppm"};
    int spectrum[MAX_ITER];

  private:

    double square(double x);
    double vectorDist(Complex z);

    Complex nextComplex(Complex z);
    Complex mapPixel(double x, double y);

    void plot_out(int color);
    void plot_in();

};

double Mandelbrot::square(double x) { 
  return x * x; 
}

double Mandelbrot::vectorDist(Complex z) {
  return sqrt(square(z.real()) + square(z.imag()));
}

Complex Mandelbrot::nextComplex(Complex z) {
  double real = square(z.real()) - square(z.imag());
  double imag = 2 * z.real() * z.imag();
  
  return Complex(real, imag);
}

Complex Mandelbrot::mapPixel(double x, double y) {
  double c_real = (x - WIDTH / 2) * 4 / WIDTH;
  double c_imag = (y - HEIGHT / 2) * 4 / WIDTH;
  
  return Complex(c_real, c_imag);
}

void Mandelbrot::plot_out(int color) {
  fout << color << " ";
  fout << color << " ";
  fout << 100 << "\n";
}

void Mandelbrot::plot_in() {
  fout << 0 << " " << 0 << " " << 0 << "\n";
}

Mandelbrot::Mandelbrot() {

  fout << "P3\n";
  fout << WIDTH << " " << HEIGHT << "\n";
  fout << "255\n";

  for (int i = 0; i < MAX_ITER; ++i)
    spectrum[i] = i;

  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      Complex c = mapPixel(x, y);
      Complex z = (0, 0);

      int iterations = 0;
      while(vectorDist(z) <= 2 && iterations < MAX_ITER) {
        z = nextComplex(z) + c;
        iterations++;
      }

      if (iterations < MAX_ITER) 
        plot_out(spectrum[iterations]);
      
      else plot_in();
      

      fout << std::endl;
    }
  }

  fout.close();
}
