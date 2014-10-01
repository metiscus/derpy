#ifndef INVENTORY_H_
#define INVENTORY_H_

#include "Object.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct Inventory_
{
	int itemCount;
	void* data;
} Inventory;

Inventory* inventory_create();
void       inventory_destroy(Inventory* ptr);
int        inventory_add_object(Inventory* ptr, Object* obj);
Object*    inventory_get_object_at_index(Inventory* ptr, int index);
void       inventory_remove_object(Inventory* ptr, Object* obj);
Object*    inventory_remove_object_at_index(Inventory* ptr, int index);

typedef void (*InventoryIterator)(int, Object*);

void       inventory_for_each(Inventory* ptr, InventoryIterator itr);

#ifdef __cplusplus
}
#endif

#endif // INVENTORY_H_