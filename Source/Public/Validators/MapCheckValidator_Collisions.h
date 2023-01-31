#pragma once

#include "MapCheckValidatorBase.h"

#include <CoreMinimal.h>

#include "MapCheckValidator_Collisions.generated.h"

UCLASS()
class MAPCHECKVALIDATION_API AMapCheckValidator_Collisions final : public AMapCheckValidatorBase
{
    GENERATED_BODY()

public:
#if WITH_EDITOR
    void CheckForErrors() override;
#endif

private:
    UPROPERTY( EditAnywhere )
    TArray< FName > ForbiddenPresetNames;

    UPROPERTY( EditAnywhere )
    TArray< TEnumAsByte< ECollisionChannel > > ForbiddenBlockChannels;
};