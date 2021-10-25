// Monty Hall paradox
//
// Compiled using gcc 11.2.1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#define NDEBUG
#include <assert.h>

enum {
	ITERATIONS = 3000,
	DOORS = 3,
	GOAT  = 'g',
	CAR   = 'c',
	internal_error = 0
};

void simulate(int iterations)
{
	enum door { D1, D2, D3 };
	enum door doors[DOORS];

	enum door choose_door(void) {
		return rand() % DOORS; // 0 .. DOORS-1
	}
	void init_doors(void) {
		for (enum door d = 0; d < DOORS; ++d) {
			doors[d] = GOAT;
		}
		doors[choose_door()] = CAR; // one car, two goats
	}

	struct {
		int stay;
		int change;
	} scores = {0};

	void stay_strategy(enum door contestant_door) {
		// a winner?
		if (doors[contestant_door] == CAR) {
			++scores.stay; // win!
		}
	}
	void change_strategy(enum door contestant_door) {
		enum door monty_door;
		if (doors[contestant_door] == CAR) {
			// D2 or D3 are equal choices
			monty_door = (contestant_door != D1) ? D1 : D2;
			assert(contestant_door != monty_door);
		} else {
			// Monty Hall chooses a door with a goat
			switch (contestant_door) {
				default: assert(internal_error);
				case D1: monty_door = (doors[D2] == CAR) ? D3 : D2; break;
				case D2: monty_door = (doors[D1] == CAR) ? D3 : D1; break;
				case D3: monty_door = (doors[D1] == CAR) ? D2 : D1; break;
			}
			assert(contestant_door != monty_door);
			// the contestant changes door
			switch (contestant_door+monty_door) {
				default: assert(internal_error);
				case 1: contestant_door = D3; break; // 0+1
				case 2: contestant_door = D2; break; // 0+2
				case 3: contestant_door = D1; break; // 1+2
			}
			assert(contestant_door != monty_door);
			// a winner?
			if (doors[contestant_door] == CAR) {
				++scores.change; // win!
			}
		}
	}

	// start report
	printf("N = %d\n", iterations);

	// simulate
	for (int i = 0; i < iterations; i++) {
		init_doors();
		enum door contestant_door = choose_door();
		stay_strategy(contestant_door);
		change_strategy(contestant_door);
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
	if (n <= 0) n = ITERATIONS; // perhaps atoi failed

	// randomize
	srand(time(NULL));

	// do it!
	simulate(n);

	return 0;
}

// vim:ai:sw=4:ts=4:
