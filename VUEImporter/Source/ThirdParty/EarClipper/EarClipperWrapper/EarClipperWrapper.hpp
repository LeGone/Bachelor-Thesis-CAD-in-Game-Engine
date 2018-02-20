#pragma once

#ifdef WRAPPER_EXPORTS
#define SYMBOL_DECLSPEC __declspec(dllexport)
#else
#define SYMBOL_DECLSPEC	__declspec(dllimport)
#endif

namespace EarClipperW
{
	/****************************************************
	* Classes
	****************************************************/
	class SYMBOL_DECLSPEC EarClipper
	{
	public:

		short* ResultIndices;
		short ResultIndicesCount;

		EarClipper();
		~EarClipper();

		void AddVertex(double X, double Y, double Z);
		void SetNormal(double X, double Y, double Z);
		void Triangulate();

	private:

		void* EarClipperPointer;
	};
}