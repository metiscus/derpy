#include <assert.h>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include "Object.h"

int object_instance_id = 1000;
std::map<int, ObjectProperties> object_blueprints;


const int  ObjectFileVersion = 1;
const char CommentCharacter = ';';
void object_load_objects(char* objectFile)
{
    std::ifstream infile(objectFile);
    if(infile.is_open())
    {
       int fileVersion;
       infile>>fileVersion;
       if(fileVersion != ObjectFileVersion)
       {
            printf("Mismatching Object file versions.\n");
       }
    }

    while(infile.is_open()&&!infile.eof())
    {
        std::string line;
        std::getline(infile, line);
        if(line.length() == 0 || line[0] == CommentCharacter)
        {
            continue;
        }
        else {
            std::stringstream ss;
            ss<<line;

            ObjectProperties obj;
            ss>>obj.id;
            ss>>obj.volume;
            ss>>obj.material;
            ss>>obj.value;
           
            // prepare to read object attributes
            ss>>obj.attributes.count;
            for(int i=0; i<obj.attributes.count; ++i)
            {
                ss>>obj.attributes.values[i];
            }

            std::string word;
            std::string name;
            while(ss>>word)
            {
                name += word + " ";
            }
            name = name.substr(0, name.length()-1);
            strncpy(obj.name, name.c_str(), 256);

            printf("Adding ObjectProperties (%d) blueprint\n", obj.id);
            object_blueprints[obj.id] = obj;
        }
    }
}

void object_write_objects(char* objectFile)
{
    std::ofstream outfile(objectFile);
    if(outfile.is_open())
    {
        // write out version
        outfile<<ObjectFileVersion<<"\n";

        // write out the objects
        std::map<int, ObjectProperties>::iterator itr;
        for(itr=object_blueprints.begin(); itr!=object_blueprints.end(); ++itr)
        {
            ObjectProperties& obj = itr->second;
            outfile<<obj.id<<" ";
            outfile<<obj.volume<<" ";
            outfile<<obj.material<<" ";
            outfile<<obj.value<<" ";
            
            // prepare to write object attributes
            outfile<<obj.attributes.count;
            for(int i=0; i<obj.attributes.count; ++i)
            {
                outfile<<obj.attributes.values[i];
            }

            outfile<<obj.name<<" ";

            outfile<<"\n";
        }

        outfile.close();
    }
    else {
        printf("Failed to open %s to write objects.\n", objectFile);
    }
}

ObjectProperties object_lookup_properties(int objectId)
{
    assert(object_blueprints.count(objectId) >=1);

    return object_blueprints[objectId];
}

Object* object_create_instance(int objectId)
{
    assert(object_blueprints.count(objectId) >= 1);

    Object* instance = (Object*)calloc(sizeof(Object),1);
    if(instance) 
    {
        instance->instanceId = object_instance_id++;
        instance->baseObject = object_blueprints[objectId];
        instance->attributes = instance->baseObject.attributes;
    }
    return instance;
}

void object_delete_instance(Object* ptr)
{
    free(ptr);
}