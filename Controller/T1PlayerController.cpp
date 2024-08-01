// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/T1PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/T1Player.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "UI/T1HUDWidget.h"


AT1PlayerController::AT1PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;


	static ConstructorHelpers::FClassFinder<UT1HUDWidget> T1HUDWidgetRef(TEXT("/Game/MyGame/UI/Widget/WBP_HUD.WBP_HUD_C"));
	if (T1HUDWidgetRef.Class)
	{
		T1HUDWidgetClass = T1HUDWidgetRef.Class;
	}


}

void AT1PlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	T1HUDWidget = CreateWidget<UT1HUDWidget>(this, T1HUDWidgetClass);
	if (T1HUDWidget)
	{
		T1HUDWidget->AddToViewport();
	}

}

void AT1PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(MRClickAction, ETriggerEvent::Started, this,   &AT1PlayerController::OnRClickStarted);
		EnhancedInputComponent->BindAction(MRClickAction, ETriggerEvent::Triggered, this, &AT1PlayerController::OnRClickTriggered);
		EnhancedInputComponent->BindAction(MRClickAction, ETriggerEvent::Completed, this, &AT1PlayerController::OnRClickReleased);

		EnhancedInputComponent->BindAction(MLClickAction, ETriggerEvent::Started, this, &AT1PlayerController::OnLClickStarted);
		EnhancedInputComponent->BindAction(MLClickAction, ETriggerEvent::Triggered, this, &AT1PlayerController::OnLClickTriggered);
		EnhancedInputComponent->BindAction(MLClickAction, ETriggerEvent::Completed, this, &AT1PlayerController::OnLClickReleased);

		EnhancedInputComponent->BindAction(ZAction, ETriggerEvent::Started, this, &AT1PlayerController::OnZActionStarted);
		EnhancedInputComponent->BindAction(EAction, ETriggerEvent::Started, this, &AT1PlayerController::OnEActionStarted);
		EnhancedInputComponent->BindAction(RAction, ETriggerEvent::Started, this, &AT1PlayerController::OnRActionStarted);


	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void AT1PlayerController::OnRClickStarted()
{
	if (bIsLeftClicking)
		return;

	bIsRightClicking = true;

	StopMovement();
}

void AT1PlayerController::OnRClickTriggered()
{
	if (bIsLeftClicking)
		return;

	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	//APawn* ControlledPawn = GetPawn();

	AT1Player* playerCharac = Cast<AT1Player>(GetPawn());

	if (playerCharac->bIsReloading)
		return;

	if (playerCharac != nullptr)
	{
		if (playerCharac->GetMesh()->GetAnimInstance()->Montage_IsPlaying(playerCharac->MeleeAttackMontage) == false && playerCharac->GetMesh()->GetAnimInstance()->Montage_IsPlaying(playerCharac->OnHitMontage) == false)
		{
			FVector WorldDirection = (CachedDestination - playerCharac->GetActorLocation()).GetSafeNormal();
			playerCharac->AddMovementInput(WorldDirection, 1.0, false);
		}
	}

}

void AT1PlayerController::OnRClickReleased()
{
	if (bIsLeftClicking)
		return;

	AT1Player* playerCharac = Cast<AT1Player>(GetPawn());

	if (playerCharac->bIsReloading)
		return;

	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;

	bIsRightClicking = false;
}

void AT1PlayerController::OnLClickStarted()
{
	if (bIsRightClicking)
		return;

	bIsLeftClicking = true;

	StopMovement();

}

void AT1PlayerController::OnLClickTriggered()
{
	if (bIsRightClicking)
		return;

	StopMovement();

	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector LookAtDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();

		// Set the control rotation to look at the cached destination
		CachedLookRotate = LookAtDirection.Rotation();
		ControlledPawn->SetActorRotation(CachedLookRotate);
	}



}

void AT1PlayerController::OnLClickReleased()
{
	if (bIsRightClicking)
		return;

	bIsLeftClicking = false;

	StopMovement();

	FollowTime = 0.f;


	AT1Player* playerCharac = Cast<AT1Player>(GetPawn());
	if (playerCharac != nullptr)
	{
		StopMovement();

		if (playerCharac->bIsRange)
		{
			playerCharac->RangeAttacking(CachedDestination);

		}
		else
		{
			playerCharac->MeleAttacking();
		}
	}
}

void AT1PlayerController::OnZActionStarted()
{
	StopMovement();

	AT1Player* playerCharac = Cast<AT1Player>(GetPawn());
	if (playerCharac != nullptr)
	{
		playerCharac->Crouching();
	}

}

void AT1PlayerController::OnEActionStarted()
{

	StopMovement();

	AT1Player* playerCharac = Cast<AT1Player>(GetPawn());
	if (playerCharac != nullptr)
	{
		playerCharac->ItemChange();
	}


}

void AT1PlayerController::OnRActionStarted()
{
	StopMovement();

	AT1Player* playerCharac = Cast<AT1Player>(GetPawn());
	if (playerCharac != nullptr)
	{
		playerCharac->Reload();
	}

}
