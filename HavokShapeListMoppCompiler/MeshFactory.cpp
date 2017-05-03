#include "MeshFactory.h"
#include <stdint.h>
#include <vector>

#include <Physics/Collide/Shape/hkpShape.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>
#include <Physics/Collide/Shape/Compound/Collection/Mesh/hkpMeshMaterial.h>


using index_t = uint32_t;

hkpExtendedMeshShape* CreateMeshShape(const index_t* indices,
	uint32_t indexStriding,
	uint32_t numFaces,
	const hkReal* vertices,
	uint32_t vertexStriding,
	uint32_t numVertices,
	hkUchar* materialIndices,
	const hkpMeshMaterial	*materials) {

	hkpExtendedMeshShape* meshShape = new hkpExtendedMeshShape(0.01f);
	{
		hkpExtendedMeshShape::TrianglesSubpart part;

		part.m_vertexBase = vertices;
		part.m_numVertices = numVertices;
		part.m_vertexStriding = vertexStriding;
		part.m_flipAlternateTriangles = 1;
		part.m_indexBase = indices;
		part.m_numTriangleShapes = numFaces;
		part.m_indexStriding = indexStriding;
		//#ifdef USE_INDICES32
		part.m_stridingType = hkpExtendedMeshShape::INDICES_INT32;
		//#else
		//		part.m_stridingType = hkpExtendedMeshShape::INDICES_INT16;
		//#endif
		part.m_extrusion = hkVector4(0, -1.0f, 0);

		if (materials) {
			part.m_materialIndexStridingType = hkpExtendedMeshShape::MATERIAL_INDICES_INT8;
			part.m_materialIndexBase = materialIndices;
			part.m_materialIndexStriding = sizeof(hkUchar);

			part.m_materialBase = materials;
			part.m_materialStriding = sizeof(hkpMeshMaterial);
			part.m_numMaterials = 1;
		}

		meshShape->addTrianglesSubpart(part);
	}
	return meshShape;
}

MeshFactory::MeshFactory()
{
}


MeshFactory::~MeshFactory()
{
}

hkpShape * MeshFactory::Build()
{
	float* coords = new float[9]{ 0,0,0, 0,0,1, 0,1,0 };
	index_t* indices = new index_t[3] {0,1,2};
	hkUchar * materialIndices = new hkUchar[1]{ 0 };
	hkpMeshMaterial* meshMaterial = new hkpMeshMaterial();

	return CreateMeshShape(indices, 3 * sizeof(index_t), 1, coords, 3*sizeof(float), 3, materialIndices, meshMaterial);
}

