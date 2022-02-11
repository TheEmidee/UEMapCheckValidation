#pragma once

#include <MapCheckValidatorBase.h>

#include <CoreMinimal.h>

#include "MapCheckValidator_StreamingLevels.generated.h"

USTRUCT()
struct MAPCHECKVALIDATION_API FMapCheckValidatorStreamingLevelFlags
{
    GENERATED_USTRUCT_BODY()

    FMapCheckValidatorStreamingLevelFlags();

    UPROPERTY( EditAnywhere )
    uint8 bMustBeSet : 1;

    UPROPERTY( EditAnywhere )
    TArray< FString > MapNameRequiredTokens;

    UPROPERTY( EditAnywhere )
    TArray< FString > MapNameExcludedTokens;
};

/*
 * This validator checks that all the sub-levels of a scene respect some naming convention:
 * - They must be in the same folder as the persistent level
 * - Their name must contain the persistent level name
 *
 * You can also define the following properties for each sub-level:
 * - Should be visible
 * - Should not always be loaded (aka blueprint loaded)
 * - Should not block on load
 * - Should not block on unload
 * - Should disable distance streaming
 *
 * For each of those flags, you can specify 
 */
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

    UPROPERTY( EditAnywhere )
    FMapCheckValidatorStreamingLevelFlags ShouldBeVisibleFlagCheck;

    UPROPERTY( EditAnywhere )
    FMapCheckValidatorStreamingLevelFlags ShouldAlwaysBeLoadedFlagCheck;

    UPROPERTY( EditAnywhere )
    FMapCheckValidatorStreamingLevelFlags ShouldBlockOnLoadFlagCheck;

    UPROPERTY( EditAnywhere )
    FMapCheckValidatorStreamingLevelFlags ShouldBlockOnUnloadFlagCheck;

    UPROPERTY( EditAnywhere )
    FMapCheckValidatorStreamingLevelFlags DisableDistanceStreamingFlagCheck;
};
