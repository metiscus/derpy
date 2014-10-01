#include "Character.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Character* character_create(char* name, int hp)
{
	assert(name);

	Character *ret = (Character*)calloc(sizeof(Character), 1);
	if(ret)
	{
		ret->name   = strdup(name);
		ret->hp     = hp;
		ret->hp_max = hp;
		ret->inventory = inventory_create();
	}

	return ret;
}

void character_destroy(Character* ptr)
{
	if(ptr)
	{
		inventory_destroy(ptr->inventory);
		free(ptr->name);
		free(ptr);
	}
}

void character_debug(Character* ptr)
{
	assert(ptr);
	if(ptr)
	{
		printf("character_debug\n");
		printf("name='%s'\n", ptr->name);
		printf("hp=%d, hp_max=%d\n", ptr->hp, ptr->hp_max);

		int ii;
		for(ii=0; ii<ptr->inventory->itemCount; ++ii)
		{
			Object* obj = inventory_get_object_at_index(ptr->inventory, ii);
			if(obj)
			{
				printf("Object instance %d of base object id %d name='%s' at slot %d\n", obj->instanceId, obj->baseObject.id, obj->baseObject.name, ii);
			}
		}
	}
}