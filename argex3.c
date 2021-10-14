////////////////////////////////////////////////////////////////////////
// Template for CLI programs using the GNU argp library.
////////////////////////////////////////////////////////////////////////

#include <argp.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////
//

#define SUMMARY \
"argex -- A program to demonstrate how to code command-line\
 options and arguments."

#define DESCRIPTION "This is my first argp program."

#define BUG_ADDRESS "<jordinas@gmail.org>"

#define ARGUMENTS	"ARG1 ARG2" // TODO: \n ...

////////////////////////////////////////////////////////////////////////
//

//argex [-v?V] [-a STRING1] [-b STRING2] [-o OUTFILE] [--alpha=STRING1]
//      [--bravo=STRING2] [--output=OUTFILE] [--verbose] [--help] [--usage]
//      [--version] ARG1 ARG2

struct parameters
{
	char *command;
	bool verbose; 	// The -v flag
	char *alpha; 	// Argument for -a
	char *bravo;	// Argument for -b
	char *output;   // Argument for -o
	char *args[2];  // ARG1 and ARG2
};

static error_t parameter_parse(int key, char *arg, struct argp_state *state)
{
	enum { OK=0 };
	register struct parameters *p = state->input;

	inline void unshift(char *s) {state->argv[--state->next] = s;}

	switch (key) {
		default: return ARGP_ERR_UNKNOWN;

		case ARGP_KEY_INIT:
			assert(state->name == 0);
			assert(state->next == 0);
			// Set parameters defaults
			p->output = 0;
			p->alpha = "";
			p->bravo = "";
			p->verbose = false;
			return OK;

		case ARGP_KEY_ARG:
			if (state->arg_num >= 2) {
				argp_usage(state);
			}
			p->args[state->arg_num] = arg;
			return OK;

		case ARGP_KEY_END:
			assert(state->next == state->argc);
			if (state->arg_num < 2) {
				argp_usage(state);
			}
			assert(state->arg_num == 2);
			return OK;

		case 'h':
			unshift("--help");
			return OK;
		case 'u':
			unshift("--usage");
			return OK;

		case 'v':
			p->verbose = true;
			return OK;
		case 'a':
			p->alpha = arg;
			return OK;
		case 'b':
			p->bravo = arg;
			return OK;
		case 'o':
			p->output = arg;
			return OK;
	}
	// not reached
}

////////////////////////////////////////////////////////////////////////
// INTERFACE-SECTION

static int xmain(void);

const char *argp_program_version     = BUG_ADDRESS;
const char *argp_program_bug_address = ARGUMENTS;

static struct parameters parameters;

int main(int argc, char **argv)
{
	static const char doc[]			= SUMMARY "\v" DESCRIPTION;
	static const char args_doc[]	= ARGUMENTS;	// "\n"...

	static const struct argp_option options[] = {
	//	 NAME       KEY  ARG        FLAGS DOC
		{"alpha",   'a', "STRING1", 0,    "Do something with STRING1 related to the letter A"},
		{"bravo",   'b', "STRING2", 0,    "Do something with STRING2 related to the letter B"},
		{"output",  'o', "OUTFILE", 0,    "Output to OUTFILE instead of to standard output"},
		{"verbose", 'v', 0,         0,    "Produce verbose output"},
		{0,			'h', 0,         OPTION_HIDDEN}, // help
		{0,			'u', 0,         OPTION_HIDDEN},	// usage
		{0}
	};

	static const struct argp argp = {options, parameter_parse, args_doc, doc};

	enum { flags = 0 };
	static int i;

	parameters.command = argv[0];

	if (0!=argp_parse(&argp, argc, argv, flags, &i, &parameters)) {
		return argp_err_exit_status;
	}
	assert(i == argc);

	return xmain();
}

////////////////////////////////////////////////////////////////////////
// PROGRAM-SECTION
////////////////////////////////////////////////////////////////////////

static int xmain(void)
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
	if (parameters.output) {
		outstream = fopen(parameters.output, "w");
	} else {
		outstream = stdout;
	}
	// Print argument values
	fprintf(outstream, "Running %s\n\n", parameters.command);
	fprintf(outstream, "alpha = %s\nbravo = %s\n\n", parameters.alpha, parameters.bravo);
	fprintf(outstream, "ARG[1] = %s\nARG[2] = %s\n\n", parameters.args[0], parameters.args[1]);

	// If in verbose mode, print song stanza
	if (parameters.verbose) {
		fprintf(outstream, waters);
	}
	return 0;
}

// vim:ai:sw=4:ts=4
