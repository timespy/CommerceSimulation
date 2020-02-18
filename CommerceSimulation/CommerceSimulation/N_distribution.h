#pragma once
#include<math.h>
#define PI 3.1415

class N_Distribution {
private:
	double sigma;
	double mu;

public:
	N_Distribution(double mu, double sigma) {
		this->sigma = sigma;
		this->mu = mu;
	}

	double density(double x) {
		return (1 / (sqrt(2 * PI) * sigma)) * exp(-(pow(x - mu, 2) / (2 * pow(sigma, 2))));
	}

	double probability(double left, double right) {
		double ret = 0.0;
		for (double x = left; x <= right; x += 0.001) {
			ret += density(x);
		}
		return ret;
	}
};