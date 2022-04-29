// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MissionFrameworkPrivatePCH.h"

class FMissionFrameworkPlugin : public IMissionFrameworkPlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FMissionFrameworkPlugin, MissionFramework )



void FMissionFrameworkPlugin::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FMissionFrameworkPlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



