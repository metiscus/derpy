local ffi = require("ffi");
require("bindings_tmp");

local materialListFile = ffi.new("char[20]");
ffi.copy(materialListFile, "data/Materials.lst");
ffi.C.material_load_materials(materialListFile);

local objectListFile = ffi.new("char[20]");
ffi.copy(objectListFile, "data/Objects.lst");
ffi.C.object_load_objects(objectListFile);


ffi.copy(objectListFile, "Bob Tutins");
local character = ffi.C.character_create(objectListFile, 1);
local inv = character.inventory;

local object = ffi.C.object_create_instance(0);
ffi.C.inventory_add_object(inv, object);
ffi.C.character_debug(character);
 