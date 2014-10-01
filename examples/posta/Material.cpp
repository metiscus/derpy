#include <assert.h>
#include <fstream>
#include <map>
#include <string>
#include <string.h>
#include <sstream>

#include "Material.h"

int material_id = 0;

std::map<int, MaterialProperties> material_property_table;

void material_load_materials(char* filename)
{
	assert(filename);
	if(filename)
	{
		std::ifstream infile(filename);
		while(infile.is_open() && !infile.eof())
		{
			std::string line;
			std::getline(infile, line);

			if(line.length()==0 || line[0]==';')
			{
				continue;
			}

			std::stringstream ss(line);
			MaterialProperties prop;
			ss>>prop.density;

			std::string word;
            std::string name;
            while(ss>>word)
            {
                name += word + " ";
            }
            name = name.substr(0, name.length()-1);
            strncpy(prop.name, name.c_str(), 256);

            printf("Adding material (%d) '%s'\n", material_id, prop.name);

            material_property_table[material_id++] = prop;
		}
	}
}

MaterialProperties material_lookup_properties(int materialId)
{
    return material_property_table[materialId];
}
