#pragma once

#include <CoreMinimal.h>
#include <Modules/ModuleInterface.h>
#include <Modules/ModuleManager.h>
#include <Stats/Stats.h>

class MAPCHECKVALIDATION_API IMapCheckValidationModule : public IModuleInterface
{

public:
    static IMapCheckValidationModule & Get()
    {
        QUICK_SCOPE_CYCLE_COUNTER( STAT_IMapCheckValidationModule_Get );
        static auto & singleton = FModuleManager::LoadModuleChecked< IMapCheckValidationModule >( "MapCheckValidation" );
        return singleton;
    }

    static bool IsAvailable()
    {
        QUICK_SCOPE_CYCLE_COUNTER( STAT_IMapCheckValidationModule_IsAvailable );
        return FModuleManager::Get().IsModuleLoaded( "MapCheckValidation" );
    }
};
