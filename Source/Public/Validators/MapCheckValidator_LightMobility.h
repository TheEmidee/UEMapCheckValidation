#pragma once

#include "MapCheckValidatorBase.h"

#include <CoreMinimal.h>

#include "MapCheckValidator_LightMobility.generated.h"

UCLASS()
class MAPCHECKVALIDATION_API AMapCheckValidator_LightMobility final : public AMapCheckValidatorBase
{
    GENERATED_BODY()

public:
#if WITH_EDITOR
    void CheckForErrors() override;
#endif
};
