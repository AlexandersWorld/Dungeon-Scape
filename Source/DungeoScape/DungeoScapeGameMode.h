#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DungeoScapeGameMode.generated.h"

/**
 *  Simple GameMode for a first person game
 */
UCLASS(abstract)
class ADungeoScapeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADungeoScapeGameMode();
};
