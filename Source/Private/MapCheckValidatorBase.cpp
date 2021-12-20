#include "MapCheckValidatorBase.h"

AMapCheckValidatorBase::AMapCheckValidatorBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

#if WITH_EDITOR
void AMapCheckValidatorBase::CheckForErrors()
{
    Super::CheckForErrors();
}
#endif