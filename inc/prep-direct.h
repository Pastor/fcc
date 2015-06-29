/** Direct preprocessor*/
#pragma once

#include "../std/std.h"
#include "hashmap.h"


typedef struct prepDirectResult {
	int    errors;
	int    warnings;
	char  *filename;
} prepDirectResult;

typedef struct prepDirectCtx {
	vector  *paths;
    hashmap  defines;
} prepDirectCtx;


void prepDirectInit(prepDirectCtx *ctx, const vector * paths);

void prepDirectSet(prepDirectCtx *ctx, const char * const def);

void prepDirectEnd(prepDirectCtx *ctx);

void prepDirectResultDestroy (prepDirectResult* result);

prepDirectResult preprocess(const char* filename, const char* initialPath);
