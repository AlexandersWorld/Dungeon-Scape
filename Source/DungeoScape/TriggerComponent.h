// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DUNGEOSCAPE_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	UTriggerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnyWhere)
	AActor* MoverActor;
	UPROPERTY(VisibleAnyWhere)
	UMover* Mover;
	UPROPERTY(EditAnywhere)
	bool IsPressurePlate = false;

	UPROPERTY(VisibleAnyWhere)
	bool IsTriggered = false;


	UPROPERTY(VisibleAnyWhere)
	int32 ActivatorCount = 0;

	UFUNCTION()
	void Trigger(bool NewTriggerValue);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
