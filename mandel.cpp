#include <iostream>
#include <complex>
#include <vector>
#include <chrono>
#include <functional>

#include "window.h"
#include "save_image.h"
#include "utils.h"
#include "math.h"
// clang++ -std=c++11 -stdlib=libc++ -O3 save_image.cpp utils.cpp mandel.cpp -lfreeimage

// Use an alias to simplify the use of complex type
using Complex = std::complex<double>;

// Convert a pixel coordinate to the complex domain
Complex scale(window<int> &scr, window<double> &fr, Complex c) {
	Complex aux(c.real() / (double)scr.width() * fr.width() + fr.x_min(),
		c.imag() / (double)scr.height() * fr.height() + fr.y_min());
	return aux;
}

// Check if a point is in the set or escapes to infinity, return the number if iterations
int escape(Complex c,int a, int iter_max, const std::function<Complex( Complex, Complex, int)> &func) {
	Complex z(0);
	int iter = 0;

	while (abs(z) < 2.0 && iter < iter_max) {
		z = func(z, c, a);
		iter++;
	}

	return iter;
}

// Loop over each pixel from our image and check if the points associated with this pixel escape to infinity
void get_number_iterations(window<int> &scr, int a, window<double> &fract, int iter_max, std::vector<int> &colors,
	const std::function<Complex( Complex, Complex, int)> &func) {
	int k = 0, progress = -1;
	for(int i = scr.y_min(); i < scr.y_max(); ++i) {
		for(int j = scr.x_min(); j < scr.x_max(); ++j) {
			Complex c((double)j, (double)i);
			c = scale(scr, fract, c);
			colors[k] = escape(c,a, iter_max, func);
			k++;
		}
		if(progress < (int)(i*100.0/scr.y_max())){
			progress = (int)(i*100.0/scr.y_max());
			std::cout << progress << "%\n";
		}
	}
}

void fractal(window<int> &scr, window<double> &fract, int a, int iter_max, std::vector<int> &colors,
	const std::function<Complex( Complex, Complex, int)> &func, const char *fname, bool smooth_color) {
	auto start = std::chrono::steady_clock::now();
	get_number_iterations(scr, a, fract, iter_max, colors, func);
	auto end = std::chrono::steady_clock::now();
	std::cout << "Time to generate " << fname << " = " << std::chrono::duration <double, std::milli> (end - start).count() << " [ms]" << std::endl;

	// Save (show) the result as an image
	plot(scr, colors, iter_max, fname, smooth_color);
}

/*void mandelbrot() {
	// Define the size of the image
	window<int> scr(0, 1200, 0, 1200);
	// The domain in which we test for points
	window<double> fract(-2.2, 1.2, -1.7, 1.7);

	// The function used to calculate the fractal
	auto func = [] (Complex z, Complex c, int a) -> Complex {return z * z + c; };

	int iter_max = 500;
	const char *fname = "mandelbrot.png";
	bool smooth_color = true;
	std::vector<int> colors(scr.size());

	// Experimental zoom (bugs ?). This will modify the fract window (the domain in which we calculate the fractal function) 
	//zoom(1.0, -1.225, -1.22, 0.15, 0.16, fract); //Z2
	
	fractal(scr, fract, iter_max, colors, func, fname, smooth_color);
}
*/
void triple_mandelbrot() {
	int a,b;
	std::cout << "please input a number"<< std :: endl;    //the nth-tuple mandelbrot
	std::cin >> a ;

	std::cout << "please input resolution"<< std :: endl;
	std::cin >> b ;
	// Define the size of the image
	window<int> scr(0, b, 0, b);
	// The domain in which we test for points
	window<double> fract(-1.5, 1.5, -1.5, 1.5); //increased domain

	// The function used to calculate the fractal
	auto func = [] (Complex z, Complex c, int a) -> Complex {return (pow(z,a) + c); };

	int iter_max = 500; // increase iter max?
	const char *fname = "triple_mandelbrot1.png";
	bool smooth_color = true;
	std::vector<int> colors(scr.size());

	fractal(scr, fract, a, iter_max, colors, func, fname, smooth_color);
}



int main() {

	//mandelbrot();
	triple_mandelbrot();

	return 0;
}
