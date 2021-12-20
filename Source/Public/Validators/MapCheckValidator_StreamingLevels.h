#pragma once

#include <MapCheckValidatorBase.h>

#include <CoreMinimal.h>

#include "MapCheckValidator_StreamingLevels.generated.h"

UCLASS()
class MAPCHECKVALIDATION_API AMapCheckValidator_StreamingLevels final : public AMapCheckValidatorBase
{
    GENERATED_BODY()

public:

    AMapCheckValidator_StreamingLevels();

#if WITH_EDITOR
    void CheckForErrors() override;
#endif

private:

    /*
     * Set here the prefix of all your levels. For example L_
     */
    UPROPERTY( EditAnywhere )
    FString LevelPrefix;

    /*
     * This will validate that the streaming level assets are located in the same folder as the persistent level
     */
    UPROPERTY( EditAnywhere )
    uint8 bLevelStreamingLevelsMustBeInRootFolder : 1;

    UPROPERTY( EditAnywhere, meta = ( editCondition = "bLevelStreamingLevelsMustBeInRootFolder" ) )
    FString LevelsRootFolder;

    /*
     * This will validate that the streaming level assets are named with the persistent level name as a prefix.
     * Ex: the persistent level is named L_Foo, the sublevels will need to be named L_Foo_Something
     */
    UPROPERTY( EditAnywhere )
    uint8 bLevelStreamingLevelNamesMustStartWithPersistentLevelName : 1;
};
