#pragma once

#include <MapCheckValidatorBase.h>

#include <CoreMinimal.h>

#include "MapCheckValidator_StaticSubLevels.generated.h"

UCLASS()
class MAPCHECKVALIDATION_API AMapCheckValidator_StaticSubLevels final : public AMapCheckValidatorBase
{
    GENERATED_BODY()

public:

    AMapCheckValidator_StaticSubLevels();

#if WITH_EDITOR
    void CheckForErrors() override;
#endif

private:

    /*
     * This will only check the sublevels which end with that value
     */
    UPROPERTY( EditAnywhere )
    FString MapSuffix;
};
