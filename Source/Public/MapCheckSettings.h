#pragma once

#include <CoreMinimal.h>
#include <Engine/DeveloperSettings.h>

#include "MapCheckSettings.generated.h"

class AMapCheckValidatorBase;
UCLASS( config = Editor )
class MAPCHECKVALIDATION_API UMapCheckSettings final : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UMapCheckSettings();

    UPROPERTY( config, EditAnywhere )
    TArray< TSubclassOf< AMapCheckValidatorBase > > Validators;
};