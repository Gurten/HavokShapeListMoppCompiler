// JsonToMoppCLR.h
#pragma once

//The #include must come before the 'using namespace' 
#include <msclr\marshal_cppstd.h>

#include "..\HavokShapeListMoppCompiler\HavokShapeListMoppCompiler.h"

#include <vector>

using namespace System;
using namespace System::IO;

using namespace System;


using namespace System;

namespace JsonMoppNet {

	public ref class JsonToMopp
	{
	public:

		MemoryStream^ CreateMopp(String^ json);

	};
}
