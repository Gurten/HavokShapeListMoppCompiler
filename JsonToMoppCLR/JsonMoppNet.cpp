#include "stdafx.h"

#include "JsonMoppNet.h"

inline MemoryStream ^ JsonMoppNet::JsonToMopp::CreateMopp(String ^ json)
{
MemoryStream^ output = gcnew MemoryStream();

msclr::interop::marshal_context context;
std::string standardString = context.marshal_as<std::string>(json);

HavokShapeListMoppCompiler compiler;
compiler.Init();
std::vector<char> mopp = compiler.MoppFromJson(standardString);
compiler.Quit();

array<unsigned char>^ _buffer = gcnew array<unsigned char>(mopp.size());
for (int i = 0, len = mopp.size(); i < len; ++i)
{
_buffer[i] = mopp[i];
}

output->Write(_buffer, 0, (int)mopp.size());
output->Position = 0;
return output;
}
