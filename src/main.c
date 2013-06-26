#include "../std/std.h"

#include "../inc/debug.h"
#include "../inc/ast.h"
#include "../inc/sym.h"
#include "../inc/parser.h"
#include "../inc/analyzer.h"
#include "../inc/emitter.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"

int main (int argc, char** argv) {
    debugInit(stdout);

    char* Input;
    char* Output;

    if (argc <= 1) {
        puts("No input file");
        return 1;
    }

    Input = strdup(argv[1]);

    /*No output file*/
    if (argc <= 2)
        Output = filext(Input, "asm");

    else
        Output = strdup(argv[2]);

    FILE* File = fopen(Output, "w");

    if (File == 0) {
        printf("Error opening file, '%s'.\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    /* */

    architecture arch = {4};

    /*Initialize symbol "table",
      make new built in data types and add them to the global namespace*/
    sym* Global = symInit();
    sym* Types[4];
    Types[builtinVoid] = symCreateType(Global, "void", 0, 0);
    Types[builtinBool] = symCreateType(Global, "bool", arch.wordsize, typeEquality | typeAssignment | typeCondition);
    Types[builtinChar] = symCreateType(Global, "char", 1, typeIntegral);
    Types[builtinInt] = symCreateType(Global, "int", arch.wordsize, typeIntegral);

    int errors = 0;
    int warnings = 0;
    /*Has compilation failed? We could be in a mode where warnings are
      treated as errors, so this isn't completely obvious from errors
      and warnings.*/
    bool fail = false;
    ast* Tree = 0;

    /*Parse the module*/

    {
        debugSetMode(debugCompressed);
        debugMsg("Parsing");
        parserResult res = parser(Input, Global);
        debugSetMode(debugFull);
        debugMsg("");
        errors += res.errors;
        warnings += res.warnings;
        Tree = res.tree;
    }

    /*Semantic analysis*/

    {
        debugMsg("Analyzing");
        analyzerResult res = analyzer(Tree, Types);
        debugMsg("");
        errors += res.errors;
        warnings += res.warnings;
    }

    fail = errors != 0;

    /*Emit the assembly*/

    if (!fail) {
        debugWait();
        debugMsg("Emitting");
        emitter(Tree, File, &arch);
        debugMsg("");
    }

    /*Clean up*/

    astDestroy(Tree);
    symEnd(Global);

    fclose(File);

    free(Input);
    free(Output);

    if (fail)
        puts("Compilation unsuccessful.");

    else
        puts("Compilation successful.");

    return fail;
}
