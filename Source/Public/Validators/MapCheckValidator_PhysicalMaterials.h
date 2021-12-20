#pragma once

#include "MapCheckValidatorBase.h"

#include <CoreMinimal.h>

#include "MapCheckValidator_PhysicalMaterials.generated.h"

UCLASS()
class MAPCHECKVALIDATION_API AMapCheckValidator_PhysicalMaterials final : public AMapCheckValidatorBase
{
    GENERATED_BODY()

public:
#if WITH_EDITOR
    void CheckForErrors() override;
#endif
};
