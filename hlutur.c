#include <stdio.h>
#include "hlutur.h"

HLUTUR hlutir[] = {
   {"an open field", "field"   , NULL  },
   {"a little cave", "cave"    , NULL  },
   {"a silver coin", "silver"  , field },
   {"a gold coin"  , "gold"    , cave  },
   {"a burly guard", "guard"   , field },
   {"yourself"     , "yourself", field }
};