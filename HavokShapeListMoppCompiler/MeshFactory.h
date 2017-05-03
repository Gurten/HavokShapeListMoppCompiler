#pragma once

#include <Common/Base/hkBase.h>

//Includes used by this class
#include <Physics/Collide/Shape/hkpShape.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>



class MeshFactory
{
public:
	MeshFactory();
	~MeshFactory();


	hkpShape * Build();


};

