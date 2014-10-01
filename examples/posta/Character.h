#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "Inventory.h"
#include "Object.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct Character_
{
	char *name;

	int hp;
	int hp_max;
	Inventory *inventory;

} Character;

Character* character_create(char* name, int hp);
void       character_destroy(Character* ptr);
void       character_debug(Character* ptr);

#ifdef __cplusplus
}
#endif

#endif //CHARACTER_H_