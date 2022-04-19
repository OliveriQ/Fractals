#include <iostream>
#include <fstream>
#include <complex>
#include <cmath>

typedef std::complex<double> Complex;

class Tricorn
{
  public:
    Tricorn();

  private:
    const double MAX_ITER = 255;
    const double HEIGHT = 540;
    const double WIDTH = 960;

    std::ofstream fout{"output.ppm"};
    int spectrum[255];

  private:

    double square(double x);
    double magnitude(Complex z);

    Complex next_complex(Complex z);
    Complex map_pixel(double x, double y);

    void plot_out(int color);
    void plot_in();

};

double Tricorn::square(double x) { 
  return x * x; 
}

double Tricorn::magnitude(Complex z) {
  return sqrt(square(z.real()) + square(z.imag()));
}

Complex Tricorn::next_complex(Complex z) {
  double real = square(z.real()) - square(z.imag());
  double imag = 2 * z.real() * z.imag();
  
  return Complex(real, imag);
}

Complex Tricorn::map_pixel(double x, double y) {
  double c_real = (x - WIDTH / 2) * 4 / WIDTH;
  double c_imag = (y - HEIGHT / 2) * 4 / WIDTH;
  
  return Complex(c_real, c_imag);
}

void Tricorn::plot_out(int color) {
  fout << color << " ";
  fout << color << " ";
  fout << 100 << "\n";
}

void Tricorn::plot_in() {
  fout << 0 << " " << 0 << " " << 0 << "\n";
}

Tricorn::Tricorn() {

  fout << "P3\n";
  fout << WIDTH << " " << HEIGHT << "\n";
  fout << "255\n";

  for (int i = 0; i < MAX_ITER; ++i)
    spectrum[i] = i;

  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      Complex c = map_pixel(x, y);
      Complex z = (0, 0);

      int iterations = 0;
      while(magnitude(z) <= 2 && iterations < MAX_ITER) {
        Complex next = next_complex(z);
        double conj = next.imag() * -1;
        next = Complex(next.real(), conj);
        z = next + c;
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
