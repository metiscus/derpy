#ifndef MATERIAL_H_
#define MATERIAL_H_

#ifdef __cplusplus
extern "C"{
#endif

typedef struct MaterialProperties_
{
    float density;
    char name [256];
} MaterialProperties;

void material_load_materials(char* filename);
MaterialProperties material_lookup_properties(int materialId);

#ifdef __cplusplus
}
#endif

#endif // MATERIAL_H_
