////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////

#include <argp.h>
#include <assert.h>

#define SUMMARY \
"argex -- A program to demonstrate how to code command-line\
 options and arguments."

#define DESCRIPTION "This is my first argp program."

#define ARGUMENTS	"ARG1 ARG2"

const char *argp_program_version     = "argex 1.0";
const char *argp_program_bug_address = "<jordinas@gmail.org>";

static char args_doc[]	= ARGUMENTS;
static char doc[]		= SUMMARY "\v" DESCRIPTION;

static struct argp_option options[] = {
//	 NAME       KEY  ARG        FLAGS DOC
	{"alpha",   'a', "STRING1", 0,    "Do something with STRING1 related to the letter A"},
	{"bravo",   'b', "STRING2", 0,    "Do something with STRING2 related to the letter B"},
	{"output",  'o', "OUTFILE", 0,    "Output to OUTFILE instead of to standard output"},
	{"verbose", 'v', 0,         0,    "Produce verbose output"},
	{0}
};

////////////////////////////////////////////////////////////////////////

typedef enum {false, true} bool;

//argex [-v?V] [-a STRING1] [-b STRING2] [-o OUTFILE] [--alpha=STRING1]
//      [--bravo=STRING2] [--output=OUTFILE] [--verbose] [--help] [--usage]
//      [--version] ARG1 ARG2
struct arguments {
	bool verbose;             // The -v flag
	char *string1, *string2;  // Arguments for -a and -b
	char *outfile;            // Argument for -o
	char *args[2];            // ARG1 and ARG2
};

static error_t callback(int key, char *arg, struct argp_state *state)
{
	register struct arguments *args = state->input;

	switch (key) {
		default: return ARGP_ERR_UNKNOWN;

		case ARGP_KEY_INIT:
			assert(state->next == 0);
			// Set args defaults
			args->outfile = 0;
			args->string1 = "";
			args->string2 = "";
			args->verbose = false;
			break;

		case ARGP_KEY_ARG:
			if (state->arg_num >= 2) {
				argp_usage(state);
			}
			args->args[state->arg_num] = arg;
			break;

		case ARGP_KEY_END:
			assert(state->next == state->argc);
			if (state->arg_num < 2) {
				argp_usage(state);
			}
			break;

		case 'v':
			args->verbose = true;
			break;
		case 'a':
			args->string1 = arg;
			break;
		case 'b':
			args->string2 = arg;
			break;
		case 'o':
			args->outfile = arg;
			break;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	extern int xmain(struct arguments *);

	static struct argp argp = {options, callback, args_doc, doc};
	static struct arguments args;
	static int i;
	enum { flags = 0 };

	if (argp_parse(&argp, argc, argv, flags, &i, &args)) {
		return argp_err_exit_status;
	}
	assert(i == argc);
	return xmain(&args);
}

////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////

#include <stdio.h>

extern int xmain(struct arguments *args)
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
	if (args->outfile) {
		outstream = fopen(args->outfile, "w");
	} else {
		outstream = stdout;
	}
	// Print argument values
	fprintf(outstream, "alpha = %s\nbravo = %s\n\n", args->string1, args->string2);
	fprintf(outstream, "ARG1 = %s\nARG2 = %s\n\n", args->args[0], args->args[1]);

	// If in verbose mode, print song stanza
	if (args->verbose) {
		fprintf(outstream, waters);
	}
	return 0;
}

// vim:ai:sw=4:ts=4
