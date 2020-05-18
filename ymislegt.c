#include <stdio.h>
#include <string.h>
#include "hlutur.h"
#include "ymislegt.h"

HLUTUR *parsaHlut(const char *noun)
{
   HLUTUR *hlutur, *found = NULL;
   for (hlutur = hlutir; hlutur < ekkiFleiriHlutir; hlutur++)
   {
      if (noun != NULL && strcmp(noun, hlutur->tagg) == 0)
      {
         found = hlutur;
      }
   }
   return found;
}

int listaHlutiStadsetningar(HLUTUR *stadur)
{
   int count = 0;
   HLUTUR *hlutur;
   for (hlutur = hlutir; hlutur < ekkiFleiriHlutir; hlutur++)
   {
      if (hlutur != player && hlutur->stadur == stadur)
      {
         if (count++ == 0)
         {
            printf("Your eyes register:\n");
         }
         printf("%s\n", hlutur->lysing);
      }
   }
   return count;
}