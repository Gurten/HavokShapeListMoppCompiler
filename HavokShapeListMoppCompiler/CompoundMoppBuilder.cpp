/**
	Author: Gurten
*/

#include "CompoundMoppBuilder.h"

#include <Common/Base/hkBase.h>

#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>
#include <Physics/Collide/Shape/hkpShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>



CompoundMoppBuilder::CompoundMoppBuilder()
	: m_moppCode(nullptr)
	, m_shapes()
{
}


CompoundMoppBuilder::~CompoundMoppBuilder()
{
	//Havok objects do not need to be deallocated.
	/*
	for (auto shape : m_shapes)
	{
		if(shape)
			delete shape;
	}

	if(m_moppCode)
		delete m_moppCode;
	*/
}

CompoundMoppBuilder * CompoundMoppBuilder::AddShape(hkpShape * shape)
{
	m_shapes.push_back(shape);

	return this;
}


hkpMoppCode * CompoundMoppBuilder::Build()
{
	hkpMoppCompilerInput input;
	hkpListShape listShape(&m_shapes[0], m_shapes.size());

	//Input settings, may need tweaking
	input.m_enablePrimitiveSplitting = false;


	m_moppCode = hkpMoppUtility::buildCode(&listShape, input);
	return m_moppCode;
}
