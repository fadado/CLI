// Monty Hall paradox
//
// Compiled using gcc 11.2.1.
//
// https://en.wikipedia.org/wiki/Monty_Hall_problem

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// comment next line to enable assertions
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
		static const enum door others[DOORS][2] = {
			{ Door2, Door3 },
			{ Door1, Door3 },
			{ Door1, Door2 }
		};

		// Monty Hall chooses a door with a goat
		enum door monty;
		if (wins(contestant)) {
			monty = others[contestant][rand()%2];
			assert(Door1 <= monty && monty <= Door3);
			assert(monty != contestant);
		} else {
			inline enum door the_goat(const enum door a[2]) {
				return looses(a[0]) ? a[0] : a[1];
			}
			monty = the_goat(others[contestant]);
			assert(Door1 <= monty && monty <= Door3);
			assert(monty != contestant);
		}
		assert(looses(monty));

		// the contestant changes door
		contestant = (enum door)(3 - (contestant + monty));
		assert(Door1 <= contestant && contestant <= Door3);
		assert(contestant != monty);

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
	assert(scores.change + scores.stay == iterations);

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
