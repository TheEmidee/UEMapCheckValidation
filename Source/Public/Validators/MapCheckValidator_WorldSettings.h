#pragma once

#include "MapCheckValidatorBase.h"

#include <CoreMinimal.h>

#include "MapCheckValidator_WorldSettings.generated.h"

UCLASS()
class MAPCHECKVALIDATION_API AMapCheckValidator_WorldSettings final : public AMapCheckValidatorBase
{
    GENERATED_BODY()

public:
#if WITH_EDITOR
    void CheckForErrors() override;
#endif

private:
    UPROPERTY( EditAnywhere, meta = ( AllowAbstract = "true" ) )
    TSoftClassPtr< AWorldSettings > RequiredWorldSettingsClass;
};
