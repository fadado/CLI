// Monty Hall paradox
//
// Compiled using gcc 11.2.1
//
// https://en.wikipedia.org/wiki/Monty_Hall_problem

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// comment next line to enable assertions
#define NDEBUG
#include <assert.h>

// default iterations; changeable by program argument
enum { ITERATIONS=3000 };

// play many times the game and record scores for the two strategies
static void simulate(unsigned iterations)
{
	// ordinal type for the three doors
	enum door { Door1, Door2, Door3, DOORS };

	enum door choose_door(void) {
		enum door d = (rand() % DOORS);
		assert(Door1 <= d && d <= Door3);
		return d; // random door
	}

	// nominal type for the two awards
	enum award { GOAT='g', CAR='c' };

	// mapping door => award
	enum award awards[DOORS];

	void init_awards(void) {
		// one car, two goats
		awards[Door1] = awards[Door2] = awards[Door3] = GOAT;
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
	void stay_strategy(enum door contestant) {
		// open the contestant door
		if (wins(contestant)) { ++scores.stay; }
	}

	// change door after Monty offer
	void change_strategy(enum door contestant) {
		// `contestant` is the door the contestant choosed
		// `closed[][]` maps a door to the two related free doors
		static const enum door closed[DOORS][DOORS-1] = {
			[Door1] = { Door2, Door3 },
			[Door2] = { Door1, Door3 },
			[Door3] = { Door1, Door2 },
		};

		// Monty Hall chooses a door with a goat
		enum door monty;
		if (wins(contestant)) {
			// the two free doors have a goat: any is ok.
			monty = closed[contestant][rand()%2];
		} else {
			// select the free door with a goat
			enum door d = closed[contestant][0];
			monty = looses(d) ? d : closed[contestant][1];
		}
		assert(Door1 <= monty && monty <= Door3);
		assert(monty != contestant);
		assert(looses(monty));

		// the contestant changes door
		contestant = (enum door)(DOORS-(contestant+monty));
		assert(Door1 <= contestant && contestant <= Door3);
		assert(contestant != monty);

		// open the contestant door
		if (wins(contestant)) { ++scores.change; }
	}

	// start report
	printf("N = %d\n", iterations);

	// simulate
	for (unsigned i = 0; i < iterations; ++i) {
		init_awards();
		// the contestant chooses a door
		enum door contestant = choose_door();
		// try each strategy
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
	simulate((unsigned)n);

	return 0;
}

// vim:ai:sw=4:ts=4:
