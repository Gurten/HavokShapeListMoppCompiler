/**
Author: Gurten

Compilation notes:
This program depends on Havok 6.5 (20090216) which may be downloaded from here:
https://github.com/nitaigao/engine-game-factions/tree/master/etc/vendor/havok

Add the path to the '/Source' directory of Havok to the list in Project>Properties>VC++ Directories>Include Directories
Add the path to the '/Lib/win32_net_9-0/debug_multithreaded' directory of Havok to the list in Project>Properties>VC++ Directories>Library Directories
Add the following to the end of the line: hkaInternal.lib;hkaRagdoll.lib;hkBase.lib;hkCompat.lib;hkgBridge.lib;hkgCommon.lib;hkGeometryUtilities.lib;hkInternal.lib;hkpCollide.lib;hkpConstraintSolver.lib;hkpDynamics.lib;hkpInternal.lib;hkpUtilities.lib;hkSceneData.lib;hksCommon.lib;hkSerialize.lib;hkVisualize.lib;VdbClient.lib
to the list in Project>Properties>Linker>Input>Additional Dependencies.

You must also link with 'legacy_stdio_definitions.lib' which should be included with VS. 

You need jsoncpp.lib https://github.com/open-source-parsers/jsoncpp.git
*/

/*
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 
*/

// #define NDEBUG
#include <cassert>

#include <json\reader.h>
#include <json\value.h>
#include <json\json.h>
#include <fstream>
#include <iostream>
#include "stdafx.h"
#include "CompoundMoppBuilder.h"
#include "MeshFactory.h"

#include <Common/Base/Types/Geometry/hkStridedVertices.h>
#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/World/hkpWorldCinfo.h>
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h>
#include <Common/Base/Memory/hkThreadMemory.h>
#include <Physics/Utilities/Collide/ShapeUtils/MoppCodeStreamer/hkpMoppCodeStreamer.h>
#include <Common/Base/System/Io/OArchive/hkOArchive.h>
#include "HavokShapeListMoppCompiler.h"
#include <Physics/Collide/Shape/Convex/Triangle/hkpTriangleShape.h>




//http://stackoverflow.com/questions/30412951/unresolved-external-symbol-imp-fprintf-and-imp-iob-func-sdl2
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

static void HK_CALL errorReport(const char* msg, void*)
{
	// ignore this frequent message
	//if (strstr(msg, "Building Mopp code at runtime can be slow"))
	//	return;

//#ifdef _DEBUG
	std::cerr << std::string(msg) << std::endl;
//#else
//	WriteErrorReport(msg);
//#endif
}


std::vector<char> HavokShapeListMoppCompiler::MoppFromJson(std::string jsonString)
{
	Json::Value jsonRoot;
	Json::Reader jsonReader;

	assert(jsonReader.parse(jsonString, jsonRoot));

	CompoundMoppBuilder builder;

	for (auto val : jsonRoot)
	{

		auto data = val["Data"];
		auto extent = data["Extents"];
		hkVector4 extentVector(extent[0].asFloat(), extent[1].asFloat(), extent[2].asFloat());

		std::vector<float> vertCoords;
		int n_verts = 0;
		for (auto v : data["Vertices"])
		{
			vertCoords.push_back(v[0].asFloat());
			vertCoords.push_back(v[1].asFloat());
			vertCoords.push_back(v[2].asFloat());
			n_verts++;
		}

		hkStridedVertices verts;
		verts.m_vertices = &vertCoords[0];
		verts.m_numVertices = n_verts;
		verts.m_striding = sizeof(float) * 3; //3 floats per vertex

		int n_planes = 0;
		for (auto p : data["Planes"])
		{
			++n_planes;
		}
		hkArray<hkVector4> planes;
		planes.setSize(n_planes);

		for (auto p : data["Planes"])
		{
			planes.pushBack(hkVector4(p[0].asFloat(), p[1].asFloat(), p[2].asFloat(), p[3].asFloat()));

		}

		builder.AddShape(new hkpConvexVerticesShape(verts, planes, extentVector.length3()));
	}


	/*Part 2 : Compile the MOPP*/

	std::vector<char> output;

	hkpMoppCode* mopp = builder.Build();
	
	hkArray<char> moppcodeBuffer;
	hkOArchive outputArchive(moppcodeBuffer);
	hkpMoppCodeStreamer::writeMoppCodeToArchive(mopp, outputArchive);

	/*Part 3: write to output container*/
	for (int i = 0, len = moppcodeBuffer.getSize(); i < len; ++i)
	{
		output.push_back(moppcodeBuffer[i]);
	}
	
	return output;
}

HavokShapeListMoppCompiler::HavokShapeListMoppCompiler()
	: m_stackBuffer(nullptr)
	, m_hasInitialized(false)
	, m_hkThreadMemory(nullptr)
	, havokStackSize(1 << 22 /*4 MB*/)
{}

void HavokShapeListMoppCompiler::Init()
{
	assert(!m_hasInitialized);

	hkPoolMemory* memoryManager = new hkPoolMemory();
	m_hkThreadMemory = new hkThreadMemory(memoryManager);
	hkBaseSystem::init(memoryManager, m_hkThreadMemory, errorReport);

	m_stackBuffer = hkAllocate<char>(havokStackSize, HK_MEMORY_CLASS_BASE);
	m_hkThreadMemory->setStackArea(m_stackBuffer, havokStackSize);

	memoryManager->removeReference();

	m_hasInitialized = true;
}

void HavokShapeListMoppCompiler::Quit()
{
	assert(m_hasInitialized);

	m_hkThreadMemory->setStackArea(0, 0);
	hkDeallocate(m_stackBuffer);
	m_hkThreadMemory->removeReference();

	hkBaseSystem::quit();
	m_hasInitialized = false;
}

int main()
{
	/*
	HavokShapeListMoppCompiler m;
	m.Init();
	//m.MoppFromJson("");
	
	
	getchar();
	MeshFactory mf;
	hkpShape* s =  mf.Build();


	hkpTriangleShape tris;

	std::cout << "tri size: " << sizeof(hkpTriangleShape) << std::endl;

	std::cout << "hkpExtendedMeshShape: " << sizeof(hkpExtendedMeshShape) <<  " at: " << s  << std::endl;
	getchar();




	hkpWorldCinfo worldInfo;

	// Set the simulation type of the world to multi-threaded.
	worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_FIX_ENTITY;
	worldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_DISCRETE;
	worldInfo.m_gravity = hkVector4(0.0f, -9.8f, 0.0f);
	worldInfo.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_2ITERS_MEDIUM);
	worldInfo.m_collisionTolerance = 0.01f;

	hkpWorld* world = new hkpWorld(worldInfo);
	std::cout << "created a world of size: " << sizeof(hkpWorld) <<  " at: " << world << std::endl;
	getchar();

	std::cout << "before quit " << std::endl;
	m.Quit();

	getchar();
	std::cout << "after quit " << std::endl;
	*/
}
