#include <iostream>
#include <fstream>
#include <complex>
#include <cmath>


typedef std::complex<double> Complex;

class Newton 
{
  public:
    Newton();

  private:
    Complex roots[3];

    const double MAX_ITER = 40;
    const double HEIGHT = 800;
    const double WIDTH = 800;

    std::ofstream fout{"output.ppm"};

  private:
  
    Complex f(Complex z);
    Complex df(Complex z);
    Complex map_pixel(double x, double y);

    void plot(int R, int G, int B);

};

//f(x) = x^3 - 1
//(a + bi)^3 = a^3 - 3*a*b^2 + (3*a^2*b - b^3)i
Complex Newton::f(Complex z) {
  double real = z.real()*z.real()*z.real()-3*z.real()*z.imag()*z.imag()-1;
  double imag = 3*z.real()*z.real()*z.imag() - z.imag()*z.imag()*z.imag();

  return Complex(real, imag);
}

//f'(x) = 3x^2 
//(a + bi)^2 = a^2 - b^2 + (2ab)i
Complex Newton::df(Complex z) {
  double real = 3*(z.real()*z.real() - z.imag()*z.imag());
  double imag = 3*(2*z.real()*z.imag());

  return Complex(real, imag);
}

Complex Newton::map_pixel(double x, double y) {
  double c_real = (4.0 / WIDTH) * x - 2;
  double c_imag = (4.0 / HEIGHT) * y - 2;
  
  return Complex(c_real, c_imag);
}

void Newton::plot(int R, int G, int B) {
  fout << R << " " << G << " " << B << "\n";
}

Newton::Newton() {
  roots[0] = Complex(1, 0);
  roots[1] = Complex(-0.5, sqrt(3) / 2);
  roots[2] = Complex(-0.5, -sqrt(3) / 2);


  fout << "P3\n";
  fout << WIDTH << " " << HEIGHT << "\n";
  fout << "255\n";

  const double off = 0.001;

  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      //map pixel to range [-2, 2]
      Complex z = map_pixel(x, y);

      //apply newtons method
      for (int i = 0; i < MAX_ITER; ++i) {
        z = z - f(z) / df(z);
      }

      for (int i = 0; i < 3; ++i) {
        Complex root = roots[i];
        if (abs(z.real()-root.real())<off && abs(z.imag()-root.imag())<off) {
          switch (i) {
            case 0:
              plot(255, 0, 0); 
              break;
            case 1:
              plot(0, 255, 0); 
              break;
            case 2:
              plot(0, 0, 255); 
              break;
          }
        }
      }
    }
  }

  fout.close();
}