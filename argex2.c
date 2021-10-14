////////////////////////////////////////////////////////////////////////
// Template for CLI programs using the GNU argp library.
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>

#include "cli.h"

#define SUMMARY \
"argex -- A program to demonstrate how to code command-line\
 options and arguments."

#define DESCRIPTION "This is my first argp program."

#define BUG_ADDRESS "<jordinas@gmail.org>"

#define ARGUMENTS	"ARG1 ARG2"

////////////////////////////////////////////////////////////////////////
//
CLI_VERSION("argex 1.0")
CLI_DOCUMENTATION(SUMMARY, DESCRIPTION, BUG_ADDRESS)
CLI_ARGUMENTS(ARGUMENTS)

CLI_OPTIONS
	//NAME     	KEY  ARG        FLAGS DOC
	{"alpha",  	'a', "STRING1", 0,    "Do something with STRING1 related to the letter A"},
	{"bravo",  	'b', "STRING2", 0,    "Do something with STRING2 related to the letter B"},
	{"output", 	'o', "OUTFILE", 0,    "Output to OUTFILE instead of to standard output"},
	{"verbose",	'v', 0,         0,    "Produce verbose output"},
	{0,			'h', 0,         OPTION_HIDDEN}, // short help
	{0,			'u', 0,         OPTION_HIDDEN},	// short usage

CLI_PARAMETERS
	char *command;
	bool verbose; 	// The -v flag
	char *alpha; 	// Argument for -a
	char *bravo;	// Argument for -b
	char *output; 	// Argument for -o
	char *args[2];  // ARG1 and ARG2

CLI_PARSER(self, key, arg, state)
	case ARGP_KEY_INIT:
		assert(state->name == 0);
		assert(state->next == 0);
		self->output  = 0;	// set defaults
		self->alpha   = "";
		self->bravo   = "";
		self->verbose = false;
		return 0;

	case ARGP_KEY_ARG:
		if (state->arg_num >= 2) {
			argp_usage(state);
		}
		self->args[state->arg_num] = arg;
		return 0;

	case ARGP_KEY_END:
		assert(state->next == state->argc);
		if (state->arg_num < 2) {
			argp_usage(state);
		}
		assert(state->arg_num == 2);
		return 0;

	case 'h':
		unshift("--help");
		return 0;
	case 'u':
		unshift("--usage");
		return 0;

	case 'v': self->verbose = true;
		return 0;
	case 'a':
		self->alpha = arg;
		return 0;
	case 'b':
		self->bravo = arg;
		return 0;
	case 'o':
		self->output = arg;
		return 0;

CLI_GOAL(0)

////////////////////////////////////////////////////////////////////////
// xmain, pamameters based entry point

#include <stdio.h>

static int xmain(struct parameters *p)
{
	FILE *outstream;

	char waters[] =
		"a place to stay\n"
		"enough to eat\n"
		"somewhere old heroes shuffle safely down the street\n"
		"where you can speak out loud\n"
		"about your doubts and fears\n"
		"and what's more no-one ever disappears\n"
		"you never hear their standard issue kicking in your door\n"
		"you can relax on both sides of the tracks\n"
		"and maniacs don't blow holes in bandsmen by remote control\n"
		"and everyone has recourse to the law\n"
		"and no-one kills the children anymore\n"
		"and no-one kills the children anymore\n"
		"--\"the gunners dream\", Roger Waters, 1983\n";

	// Where do we send output?
	if (p->output) {
		outstream = fopen(p->output, "w");
	} else {
		outstream = stdout;
	}
	// Print argument values
	fprintf(outstream, "Running %s\n\n", p->command);
	fprintf(outstream, "alpha = %s\nbravo = %s\n\n", p->alpha, p->bravo);
	fprintf(outstream, "ARG[1] = %s\nARG[2] = %s\n\n", p->args[0], p->args[1]);

	// If in verbose mode, print song stanza
	if (p->verbose) {
		fprintf(outstream, waters);
	}
	return 0;
}

// vim:ai:sw=4:ts=4
