#include "../inc/prep-direct.h"
#include "../inc/hashmap.h"

#include "string.h"
#include "stdlib.h"

typedef struct defExpression {
	char  *val;
} defExpression;

static void
defExpressionDestroy(defExpression *def)
{
  free(def);
}


void
prepDirectInit(prepDirectCtx *ctx, const vector * paths)
{
  ctx->paths = (vector *)paths;
  hashmapInit(&ctx->defines, 256);
}

void
prepDirectEnd(prepDirectCtx *ctx)
{
  hashmapFreeObjs(&ctx->defines, (hashmapKeyDtor)free, (hashmapValueDtor)defExpressionDestroy);
  hashmapFree(&ctx->defines);
}

void
prepDirectSet(prepDirectCtx *ctx, const char * const def)
{
  defExpression *expr = (defExpression *)malloc(sizeof(defExpression));
  char *key;
  char *p;

  p = strchr(def, '=');
  key = strdup(def);
  if (p != 0) {
	int id = p - def;
	key[id] = 0;
	expr->val = &key[id + 1];
  } else {
	expr->val = 0;
  }

  hashmapAdd(&ctx->defines, key, expr);
}

prepDirectResult
preprocess(const char* filename, const char* initialPath)
{
  /** Stub */
  (void)initialPath;
  return (prepDirectResult) { .errors = 0, .warnings = 0, .filename = strdup(filename) };
}

void
prepDirectResultDestroy (prepDirectResult* result)
{
  /** Free filename and remove file */
  (void )result;
}

