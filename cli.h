
#include <argp.h>

#define CLI_VERSION(V) const char *argp_program_version = V;
#define CLI_DOCUMENTATION(S,D,A) \
	static const char doc[] = S "\v" D;\
	const char *argp_program_bug_address = A;

// TODO: concat with "\n"
#define CLI_ARGUMENTS(A) static const char args_doc[] = A;

#define CLI_OPTIONS static const struct argp_option options[] = {

#define CLI_PARAMETERS {0}}; struct parameters {

#define CLI_PARSER(P,K,A,S)\
};\
static error_t parameter_parse(int K, char *A, struct argp_state *S)\
{\
	register struct parameters *P = S->input;\
	inline void unshift(char *x) {state->argv[--state->next] = x;}\
	switch (key) { default: return ARGP_ERR_UNKNOWN;


#define CLI_GOAL(F) \
}}\
static int xmain(struct parameters *);\
int main(int argc, char **argv)\
{\
	static const struct argp argp = {options, parameter_parse, args_doc, doc};\
	static int i;\
	static struct parameters parms;\
	parms.command = argv[0];\
	if (0!=argp_parse(&argp, argc, argv, F, &i, &parms))\
		return argp_err_exit_status;\
	assert(i == argc);\
	return xmain(&parms);\
}

