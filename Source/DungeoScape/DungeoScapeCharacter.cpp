// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeoScapeCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DungeoScape.h"
#include "CollectableItem.h"
#include "Lock.h"

ADungeoScapeCharacter::ADungeoScapeCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void ADungeoScapeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADungeoScapeCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADungeoScapeCharacter::DoJumpEnd);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADungeoScapeCharacter::MoveInput);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADungeoScapeCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ADungeoScapeCharacter::LookInput);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ADungeoScapeCharacter::Interact);
	}
	else
	{
		UE_LOG(LogDungeoScape, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADungeoScapeCharacter::Interact()
{
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector End = Start + (FirstPersonCameraComponent->GetForwardVector() * MaxInteractionDistance);

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f);

	FCollisionShape InteractionSphere = FCollisionShape::MakeSphere(InteractionSphereRadius);

	DrawDebugSphere(GetWorld(), End, InteractionSphere.GetSphereRadius(), 20, FColor::Blue, false, 5.0f);

	FHitResult outHitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(
		outHitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		InteractionSphere
	);


	if (HasHit)
	{
		AActor* HitActor = outHitResult.GetActor();

		if (HitActor->ActorHasTag("CollectableItem"))
		{
			CollectItemToBag(HitActor);
		}
		else if (HitActor->ActorHasTag("Lock"))
		{
			RemoveItemFromBag(HitActor);
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No actor hit!"));
	}
}

void ADungeoScapeCharacter::CollectItemToBag(AActor* HitActor)
{
	if (ACollectableItem* CollectableItem = Cast<ACollectableItem>(HitActor))
	{
		if (!ItemsBag.Contains(CollectableItem->ItemName))
		{
			ItemsBag.Add(CollectableItem->ItemName);

			CollectableItem->Destroy();
		}
	}
}

void ADungeoScapeCharacter::RemoveItemFromBag(AActor* HitActor)
{
	if (ALock* LockActor = Cast<ALock>(HitActor))
	{
		if (!LockActor->GetIsKeyPlaced())
		{
			PlaceKey(LockActor);
		}
		else
		{
			ReturnKey(LockActor);
		}
	}
}

void ADungeoScapeCharacter::PlaceKey(ALock* LockActor)
{
	if (ItemsBag.Contains(LockActor->KeyItemName))
	{
		if (ItemsBag.RemoveSingle(LockActor->KeyItemName))
		{
			LockActor->SetIsKeyPlaced(true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Key item not in the inventory"));
	}
}

void ADungeoScapeCharacter::ReturnKey(ALock* LockActor)
{
	if (!ItemsBag.Contains(LockActor->KeyItemName))
	{
		ItemsBag.Add(LockActor->KeyItemName);
		LockActor->SetIsKeyPlaced(false);
	}
}


void ADungeoScapeCharacter::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	DoMove(MovementVector.X, MovementVector.Y);
}

void ADungeoScapeCharacter::LookInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	DoAim(LookAxisVector.X, LookAxisVector.Y);
}

void ADungeoScapeCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ADungeoScapeCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ADungeoScapeCharacter::DoJumpStart()
{
	Jump();
}

void ADungeoScapeCharacter::DoJumpEnd()
{
	StopJumping();
}
