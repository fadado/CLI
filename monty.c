// Monty Hall paradox
//
// Compiled using gcc 11.2.1.
//
// https://en.wikipedia.org/wiki/Monty_Hall_problem

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//#define NDEBUG
#include <assert.h>

enum {
	internal_error = 0,
	ITERATIONS     = 3000 // default iterations
};

void simulate(int iterations)
{
	enum award { GOAT = 'g', CAR = 'c'};
	enum door  { Door1, Door2, Door3, DOORS };

	// choose a random one
	inline enum door one_of(enum door a, enum door b) {
		return (rand() % 2) ? a : b;
	}

	// random door: 0 .. DOORS-1
	inline enum door choose_door(void) {
		enum door d = rand() % DOORS;
		assert(Door1 <= d && d <= Door3);
		return d;
	}

	// mapping door => award
	enum award awards[DOORS];

	inline void init_awards(void) {
		// one car, two goats
		awards[0] = awards[1] = awards[2] = GOAT;
		awards[choose_door()] = CAR;
	}

	inline bool wins(enum door d) { return awards[d] == CAR; }
	inline bool looses(enum door d) { return awards[d] == GOAT; }

	// record scores for each strategy
	struct {
		int stay;
		int change;
	} scores = {0};

	// stay with the first door selected
	inline void stay_strategy(enum door contestant) {
		// a winner?
		if (wins(contestant)) { ++scores.stay; }
	}

	// change door after Monty offer
	void change_strategy(enum door contestant) {
		enum door monty;

		// Monty Hall chooses a door with a goat
		if (wins(contestant)) {
			switch (contestant) {
				default: assert(internal_error);
				case Door1: monty = one_of(Door2, Door3); break;
				case Door2: monty = one_of(Door1, Door3); break;
				case Door3: monty = one_of(Door1, Door2); break;
			}
			assert(Door1 <= monty && monty <= Door3);
			assert(monty != contestant);
		} else {
			assert(looses(contestant));
			switch (contestant) {
				default: assert(internal_error);
				case Door1:
					monty = wins(Door2) ? Door3 : Door2; 
					break;
				case Door2:
					monty = wins(Door1) ? Door3 : Door1;
					break;
				case Door3:
					monty = wins(Door1) ? Door2 : Door1;
					break;
			}
			assert(Door1 <= monty && monty <= Door3);
			assert(monty != contestant);
		}
		assert(looses(monty));

		// the contestant changes door
		contestant = (enum door)(DOORS - (contestant + monty));
		assert(Door1 <= contestant && contestant <= Door3);
		assert(contestant != monty);

		//assert(wins(contestant) || wins(monty));

		// a winner?
		if (wins(contestant)) { ++scores.change; }
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
	//???assert(scores.change + scores.stay == ITERATIONS);

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
