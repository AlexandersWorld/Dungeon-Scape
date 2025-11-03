#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEOSCAPE_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMover();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere)
	FVector StartLocation;


	UPROPERTY(VisibleAnywhere)
	FVector TargetLocation;

	UPROPERTY(EditAnywhere)
	FVector MoveOffset;


	UPROPERTY(EditAnywhere)
	float MoveTime = 4.0f;

	UPROPERTY(EditAnywhere)
	bool ShouldMove = false;

	UPROPERTY(VisibleAnywhere)
	bool ReachedTarget = false;

};
