#include "shared_memory.h"

#include <stdatomic.h>

static struct
{
  _Atomic SharedDataType params[NS_SD(__NUM_OF__)];
}NS_SH(MEMORY);

#ifdef DEBUG
char __assert_size_shared_data_type[sizeof(NS_SH(MEMORY).params[0])==sizeof(SharedDataType)?1:-1];
#endif /* ifdef DEBUG */

void NS_SH(update)(const enum NS_SD() param, const SharedDataType value)
{
  atomic_store(&NS_SH(MEMORY).params[param], value);
}

SharedDataType NS_SH(get)(const enum NS_SD() param)
{
  return atomic_load(&NS_SH(MEMORY).params[param]);
}
