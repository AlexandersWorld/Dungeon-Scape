#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEOSCAPE_API UMover : public UActorComponent
{
	GENERATED_BODY()

private:	
	bool ShouldMove = false;

protected:
	virtual void BeginPlay() override;

public:	
	UMover();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere)
	FVector StartLocation;


	UPROPERTY(VisibleAnywhere)
	FVector TargetLocation;

	UPROPERTY(EditAnywhere)
	FVector MoveOffset;


	UPROPERTY(EditAnywhere)
	float MoveTime = 4.0f;

	UPROPERTY(VisibleAnywhere)
	bool ReachedTarget = false;

	UFUNCTION()
	void SetShouldMove(bool NewShouldMove);

	UFUNCTION()
	bool GetShouldMove();
};
