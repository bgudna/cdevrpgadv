#include<stdio.h>
#include<string.h>
#include "hlutur.h"
#include "ymislegt.h"

void goLook(const char *noun){
    if (noun != NULL && strcmp(noun, "around") == 0) {
        printf("You be at %s\n", player->stadur->lysing);
        listaHlutiStadsetningar(player->stadur);
    } else {
        printf("You are lost and confused.\n");
    }
}

void goGo(const char *noun){
    
    HLUTUR *hlutur = parsaHlut(noun);
   if (hlutur == NULL)
   {
      printf("there is mystery in your requests.\n");
   }
   else if (hlutur == player->stadur)
   {
      printf("useless, this is where you are now.\n");
   }
   else
   {
      printf("granted.\n");
      player->stadur = hlutur;
      goLook("around");
   }
}
