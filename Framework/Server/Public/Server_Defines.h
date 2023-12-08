#pragma once

#include <process.h>
#include <map>
#include <set>


namespace Server
{
	enum LAYER_TYPE
	{
		LAYER_TERRAIN,
		LAYER_SKYBOX,
		LAYER_CAMERA,
		LAYER_BACKGROUND,
		LAYER_TREE,
		LAYER_PROP,
		LAYER_BUILDING,
		LAYER_PLAYER,
		LAYER_MONSTER,
		LAYER_PROJECTILE,
		LAYER_EFFECT,
		LAYER_UI,
		LAYER_END
	};
	enum OBJ_TYPE
	{
		OBJ_BACKGROUND,
		OBJ_BUILDING,
		OBJ_CAMERA,
		OBJ_CHARACTER,
		OBJ_PLAYER,
		OBJ_MONSTER,
		OBJ_PROP,
		OBJ_MESH_EFFECT,
		OBJ_TEXTURE_EFFECT,
		OBJ_TERRAIN,
		OBJ_DUMMY,
		OBJ_END
	};
	enum LEVELID { LEVEL_STATIC, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_TOOL, LEVEL_LOADING, LEVEL_DUMMY, LEVEL_END };
}


using namespace Server;
