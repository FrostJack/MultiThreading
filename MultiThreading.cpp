// MultiThreading.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <iomanip>
#include <vector>
#include <random>
#include "pcg_random.hpp"
#include <thread>
#include <math.h>

const int N = 1000000;

const int K = 4;

const int thread_N = N / K;

void Monte(int& k, pcg64& rng, int n) {
	std::uniform_real_distribution<double> unif(-1, 1);
	int _k = 0;
	double x, y;
	for (int i = 0; i < n; ++i) {
		x = unif(rng);
		y = unif(rng);
		if (x * x + y * y <= 1)
			++_k;
	}
	k += _k;
}


int main()
{
	std::vector<pcg64> rngs;
	rngs.reserve(K);
	pcg64 rng(1);
	pcg64 rng_checkpoint = rng;
	for (int i = 0; i < K; ++i) {
		rngs.push_back(rng);
		rng.discard(thread_N*2);
	}

	int k = 0;
	std::vector<std::thread> ts;
	ts.reserve(K);
	for (int i = 0; i < K; ++i) {
		ts.push_back(std::thread(Monte, std::ref(k), std::ref(rngs[i]), thread_N));
	}

	clock_t start = clock();
	for (auto &tcur : ts) {
		tcur.join();
	}

	double pi_estim = ((double)k / (double)N) * 4;

	std::cout << "Calculation of pi with " << N << " points within " << K << " threads was completed within: "
		<< (float)(clock() - start) / CLOCKS_PER_SEC << std::endl;

	start = clock();
	int k2 = 0;
	Monte(k2, rng_checkpoint, N);
	double pi_estim_2 = ((double)k2 / (double)N) * 4;

	std::cout << "Calculation of pi with " << N << " points within single thread was completed within: "
		<< (float)(clock() - start) / CLOCKS_PER_SEC << std::endl;

	std::cout << "Multithread result: " << std::setprecision(9) << pi_estim << "; Single thread result: " << pi_estim_2 << " ; Pi: " << M_PI << std::endl;
    return 0;
}

