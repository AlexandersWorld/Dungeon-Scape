// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectableItem.generated.h"

UCLASS()
class DUNGEOSCAPE_API ACollectableItem : public AActor
{
	GENERATED_BODY()
	
private:	
	

protected:
	virtual void BeginPlay() override;

public:	
	ACollectableItem();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FString ItemName;
};
