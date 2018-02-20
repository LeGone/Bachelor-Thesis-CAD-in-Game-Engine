#include "stdafx.h"

#define WRAPPER_EXPORTS
#include "EarClipperWrapper.hpp"

#include <msclr/marshal.h>
using namespace msclr::interop;

#include <strsafe.h>
#include <vcclr.h>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace EarClipperW
{
	System::Reflection::Assembly^ currentDomain_AssemblyResolve(Object^ sender, ResolveEventArgs^ args)
	{
		sender;

		// If this is an mscorlib, do a bare load
		if (args->Name->Length >= 8 && args->Name->Substring(0, 8) == L"mscorlib")
		{
			return System::Reflection::Assembly::Load(args->Name->Substring(0, args->Name->IndexOf(L",")) + L".dll");
		}

		// Load the assembly from the specified path
		String^ finalPath = nullptr;
		try
		{
			finalPath = gcnew String("C:/Users/LeGone/Documents/Unreal Projects/VUEProject/Plugins/VUEImporter/Source/ThirdParty/EarClipper/Binaries/") + args->Name->Substring(0, args->Name->IndexOf(",")) + ".dll";
			System::Reflection::Assembly^ retval = System::Reflection::Assembly::LoadFrom(finalPath);
			return retval;
		}
		catch (...)
		{
		}

		return nullptr;
	}

	EarClipper::EarClipper()
	{
		AppDomain::CurrentDomain->AssemblyResolve += gcnew ResolveEventHandler(currentDomain_AssemblyResolve);

		// Managed
		// Instantiate the C# class CSSimpleObject.
		EarClipperLib::Helper^ EarClipperInstance = gcnew EarClipperLib::Helper();

		// Pin the CSSimpleObject .NET object, and record the address of the 
		// pinned object in m_impl. 
		EarClipperPointer = GCHandle::ToIntPtr(GCHandle::Alloc(EarClipperInstance)).ToPointer();

		// Unmanaged
		ResultIndices = NULL;
	}

	EarClipper::~EarClipper()
	{
		// Managed
		GCHandle AGCHandle = GCHandle::FromIntPtr(IntPtr(EarClipperPointer));
		AGCHandle.Free();

		// Unmanaged
		if (ResultIndices)
		{
			delete[] ResultIndices;
		}
	}

	void EarClipper::AddVertex(double X, double Y, double Z)
	{
		GCHandle AGCHandle = GCHandle::FromIntPtr(IntPtr(EarClipperPointer));
		EarClipperLib::Helper^ EarClipperInstance = safe_cast<EarClipperLib::Helper^>(AGCHandle.Target);

		EarClipperInstance->AddVertex(X, Y, Z);
	}

	void EarClipper::SetNormal(double X, double Y, double Z)
	{
		GCHandle AGCHandle = GCHandle::FromIntPtr(IntPtr(EarClipperPointer));
		EarClipperLib::Helper^ EarClipperInstance = safe_cast<EarClipperLib::Helper^>(AGCHandle.Target);

		EarClipperInstance->SetNormal(X, Y, Z);
	}

	void EarClipper::Triangulate()
	{
		GCHandle AGCHandle = GCHandle::FromIntPtr(IntPtr(EarClipperPointer));
		EarClipperLib::Helper^ EarClipperInstance = safe_cast<EarClipperLib::Helper^>(AGCHandle.Target);

		System::Collections::Generic::List<short>^ ManagedIndices = EarClipperInstance->TriangulateAndReturnIndices();

		ResultIndicesCount = ManagedIndices->Count;
		if (ResultIndicesCount)
		{
			ResultIndices = new short[ResultIndicesCount];
			for (short Index = 0; Index < ResultIndicesCount; Index++)
			{
				ResultIndices[Index] = ManagedIndices[Index];
			}
		}
	}
}