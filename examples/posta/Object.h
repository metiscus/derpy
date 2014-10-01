#ifndef OBJECT_H_
#define OBJECT_H_

#include "Material.h"

#ifdef __cplusplus
extern "C"{
#endif

enum ObjectType
{
    Obj_Unknown,
    Obj_Weapon,
    Obj_Armor,
    Obj_Consumable,
    Obj_Currency,

    Obj_Count
};

/*
    struct ObjectProperties 
        - defines the properties of all instances of a particular object

    struct Object
        - an instance of a particular ObjectProperties (ObjectProperties)
*/

#define Object_Max_Attributes 10
typedef struct ObjectAttributes_
{
    int count;
    int values[Object_Max_Attributes];
} ObjectAttributes;

typedef struct ObjectProperties_
{
    int id;
    int volume;
    int material; // enum MaterialType
    int value;
    char name[256];

    ObjectAttributes attributes;
} ObjectProperties;

typedef struct Object_
{
    ObjectProperties baseObject;
    int instanceId;
    ObjectAttributes attributes;
} Object;

Object*          object_create_instance(int objectId);
void             object_delete_instance(Object* ptr);
void             object_load_objects(char* objectFile);
void             object_write_objects(char* objectFile);
ObjectProperties object_lookup_properties(int objectId);


#ifdef __cplusplus
}
#endif

#endif // OBJECT_H_
