// Monty Hall paradox
//
// Compiled using gcc 11.2.1
//
// https://en.wikipedia.org/wiki/Monty_Hall_problem

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NDEBUG
#include <assert.h>

enum {
	internal_error = 0,
	ITERATIONS     = 3000 // default iterations
};

void simulate(int iterations)
{
	enum award { GOAT = 'g', CAR = 'c'};
	enum door  { Door1, Door2, Door3, DOORS };

	// random door: 0 .. DOORS-1
	enum door choose_door(void) {
		int d = (enum door)(rand() % DOORS);
		assert(Door1 <= d && d <= Door3);
		return d;
	}

	// mapping door => award
	enum award awards[DOORS];

	void init_awards(void) {
		for (enum door d = 0; d < DOORS; ++d) {
			awards[d] = GOAT;
		}
		awards[choose_door()] = CAR; // one car, two goats
	}

	// record scores for each strategy
	struct {
		int stay;
		int change;
	} scores = {0};

	// stay with the first door selected
	void stay_strategy(enum door contestant) {
		// a winner?
		if (awards[contestant] == CAR) {
			++scores.stay; // win!
		}
	}

	// change door after Monty offer
	void change_strategy(enum door contestant) {
		enum door monty;
		// Monty Hall chooses a door with a goat
		if (awards[contestant] == CAR) {
			monty = (contestant > Door1) ? Door1 : Door2;
		} else {
			switch (contestant) {
				default: assert(internal_error);
				case Door1: monty = (awards[Door2] == CAR) ? Door3 : Door2; break;
				case Door2: monty = (awards[Door1] == CAR) ? Door3 : Door1; break;
				case Door3: monty = (awards[Door1] == CAR) ? Door2 : Door1; break;
			}
		}
		assert(awards[monty] == GOAT);
		assert(contestant != monty);
		// the contestant changes door
		switch (contestant+monty) {
			default: assert(internal_error);
			case 1: contestant = Door3; break; // 0+1
			case 2: contestant = Door2; break; // 0+2
			case 3: contestant = Door1; break; // 1+2
		}
		assert(contestant != monty);
		// a winner?
		if (awards[contestant] == CAR) {
			++scores.change; // win!
		}
	}

	// start report
	printf("N = %d\n", iterations);

	// simulate
	for (int i = 0; i < iterations; ++i) {
		init_awards();
		enum door contestant = choose_door();
		stay_strategy(contestant);
		change_strategy(contestant);
	}
	assert(scores.change > scores.stay);

	// report scores
	printf("Stay strategy won %d times\n", scores.stay);
	printf("Change strategy won %d times\n", scores.change);
}

int main(int argc, char *argv[])
{
	// obtain number of iterations
	int n = (argc < 2) ? ITERATIONS : atoi(argv[1]);
	if (n <= 0) n = ITERATIONS; // perhaps atoi failed?

	// randomize
	srand(time(NULL));

	// do it!
	simulate(n);

	return 0;
}

// vim:ai:sw=4:ts=4:
