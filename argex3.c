// Template for CLI programs using the GNU argp library.

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <argp.h>

////////////////////////////////////////////////////////////////////////
// SYSTEM INTERFACE
////////////////////////////////////////////////////////////////////////

//
// PARAMETERS DATA
//

/* Parameters encoding in command line
argex [-v?V] [-a STRING1] [-b STRING2] [-o OUTFILE] [--alpha=STRING1]
      [--bravo=STRING2] [--output=OUTFILE] [--verbose] [--help] [--usage]
      [--version] ARG1 ARG2 */

static struct parameters {
	char *command;
	bool verbose; 	// The -v flag
	char *alpha; 	// Argument for -a
	char *bravo;	// Argument for -b
	char *output;   // Argument for -o
	char *args[2];  // ARG1 and ARG2
} parameters;

//
// PARAMETERS PARSER
//

static error_t
parameters_parse(int key, char *arg, struct argp_state *state)
{
	enum { OK=0 };
	register struct parameters *p = state->input;

	inline void unshift(char *s) {state->argv[--state->next] = s;}

	switch (key) {
		default: return ARGP_ERR_UNKNOWN;

		case ARGP_KEY_INIT:
			assert(state->name == NULL);
			assert(state->next == 0);
			// Set defaults
			assert(p->command != NULL);
			p->output = NULL;
			p->alpha = p->bravo = "";
			p->verbose = false;
			return OK;

		case ARGP_KEY_ARG:
			if (state->arg_num >= 2) { argp_usage(state); }
			p->args[state->arg_num] = arg;
			return OK;

		case ARGP_KEY_END:
			assert(state->next == state->argc);
			if (state->arg_num < 2) { argp_usage(state); }
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

//
// DOC-STRINGS & OPTIONS
//

#define VERSION "argex 1.0"

#define SUMMARY \
"argex -- A program to demonstrate how to code command-line\
 options and arguments."

#define DESCRIPTION "This is my first argp program."

#define BUG_ADDRESS "<jordinas@gmail.org>"

#define OPTIONS \
/*	 NAME       KEY  ARG        FLAGS DOC	*/\
	{"alpha",   'a', "STRING1", 0,    "Do something with STRING1 related to the letter A"},\
	{"bravo",   'b', "STRING2", 0,    "Do something with STRING2 related to the letter B"},\
	{"output",  'o', "OUTFILE", 0,    "Output to OUTFILE instead of to standard output"},\
	{"verbose", 'v', 0,         0,    "Produce verbose output"},\
	{0,			'h', 0,         OPTION_HIDDEN}, /* help */\
	{0,			'u', 0,         OPTION_HIDDEN},	/* usage */\
	{0}

#define ARGUMENTS	"ARG1 ARG2" // TODO: \n ...

//
// MAIN (don't edit!)
//

const char *argp_program_version     = VERSION;
const char *argp_program_bug_address = BUG_ADDRESS;

int main(int argc, char **argv)
{
#	define catch(x) switch (x) {case 0:break; default:goto on_error;}
	extern int xmain(void);
	static const char doc[] = SUMMARY "\v" DESCRIPTION;
	static const char args_doc[] = ARGUMENTS;
	static const struct argp_option options[] = { OPTIONS };
	static const struct argp argp = {options, parameters_parse, args_doc, doc};
	static const int flags = 0;
	int i;
	parameters.command = argv[0];
	catch(argp_parse(&argp, argc, argv, flags, &i, &parameters));
	assert(i == argc);
	return xmain();
on_error:
	return argp_err_exit_status;
#	undef catch
}

#undef ARGUMENTS
#undef BUG_ADDRESS
#undef DESCRIPTION
#undef OPTIONS
#undef SUMMARY
#undef VERSION

////////////////////////////////////////////////////////////////////////
// PROGRAM
////////////////////////////////////////////////////////////////////////

int xmain(void)
{
	static const char waters[] =
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
	FILE *fp;
	if (parameters.output) {
		fp = fopen(parameters.output, "w");
	} else {
		fp = stdout;
	}
	// Print argument values
	fprintf(fp, "Running %s\n\n", parameters.command);
	fprintf(fp, "alpha = %s\nbravo = %s\n\n", parameters.alpha, parameters.bravo);
	fprintf(fp, "ARG[1] = %s\nARG[2] = %s\n\n", parameters.args[0], parameters.args[1]);

	// If in verbose mode, print song stanza
	if (parameters.verbose) {
		fprintf(fp, waters);
	}
	return 0;
}

// vim:ai:sw=4:ts=4
