#pragma once

#include "MapCheckValidatorBase.h"

#include <CoreMinimal.h>

#include "MapCheckValidator_NavLinkProxies.generated.h"

UCLASS()
class MAPCHECKVALIDATION_API AMapCheckValidator_NavLinkProxies final : public AMapCheckValidatorBase
{
    GENERATED_BODY()

public:
#if WITH_EDITOR
    void CheckForErrors() override;
#endif

private:
    UPROPERTY( EditAnywhere )
    uint8 bForbidSimpleNavLinks : 1;

    UPROPERTY( EditAnywhere )
    uint8 bForbidSmartNavLinks : 1;

    UPROPERTY( EditAnywhere )
    FNavAgentSelector SupportedAgents;
};
