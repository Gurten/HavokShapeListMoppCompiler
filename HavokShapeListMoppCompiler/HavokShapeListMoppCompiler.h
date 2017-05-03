#ifndef HAVOKSHAPELISTMOPPCOMPILER_H

#define HAVOKSHAPELISTMOPPCOMPILER_H


class hkThreadMemory;


#include <vector>

class __declspec(dllexport)  HavokShapeListMoppCompiler
{
public:
	HavokShapeListMoppCompiler();
	void Init();
	void Quit();
	std::vector<char> MoppFromJson(std::string jsonString);

private:
	//The stack buffer used by Havok
	char * m_stackBuffer;
	bool m_hasInitialized;
	hkThreadMemory * m_hkThreadMemory;
	const size_t havokStackSize;
};













#endif
