#pragma once

#include "MapCheckValidatorBase.h"

#include <CoreMinimal.h>

#include "MapCheckValidator_StaticMeshesLODs.generated.h"

/*
 * This validator will check that all the static mesh components have a static mesh assigned and that static mesh has LODs
 */
UCLASS()
class MAPCHECKVALIDATION_API AMapCheckValidator_StaticMeshesLODs final : public AMapCheckValidatorBase
{
    GENERATED_BODY()

public:
#if WITH_EDITOR
    void CheckForErrors() override;
#endif
};
