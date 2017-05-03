/**
	Author: Gurten
*/

#ifndef  COMPOUNDMOPPBUILDER_H
#define COMPOUNDMOPPBUILDER_H

#include <Common/Base/hkBase.h>

//Includes used by this class
#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>
#include <Physics/Collide/Shape/hkpShape.h>

#include <vector>

class CompoundMoppBuilder
{
public:
	CompoundMoppBuilder();
	virtual ~CompoundMoppBuilder();

	CompoundMoppBuilder* AddShape(hkpShape * shape);

	hkpMoppCode* Build();

private:
	std::vector<hkpShape*> m_shapes;
	hkpMoppCode* m_moppCode;

};

#endif // ! COMPOUNDMOPPBUILDER_H