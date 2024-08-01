// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/T1AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/T1Character.h"

UT1AnimInstance::UT1AnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{



}

void UT1AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<AT1Character>(TryGetPawnOwner());

	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();

		bIsCrouch = Character->bIsCrouch;

		bIsRange = Character->bIsRange;
	}



}

void UT1AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character == nullptr)
		return;

	if (MovementComponent == nullptr)
		return;

	Velocity = MovementComponent->Velocity;
	GroundSpeed = Velocity.Size2D();

	bShouldMove = GroundSpeed > 3.f;

	bIsCrouch = Character->bIsCrouch;

	bIsRange = Character->bIsRange;

}
