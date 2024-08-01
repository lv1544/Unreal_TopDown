// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "T1PlayerController.generated.h"


class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class T1_API AT1PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AT1PlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void OnRClickStarted();
	void OnRClickTriggered();
	void OnRClickReleased();

	void OnLClickStarted();
	void OnLClickTriggered();
	void OnLClickReleased();

	void OnZActionStarted();
	void OnEActionStarted();
	void OnRActionStarted();

	bool bIsLeftClicking = false;
	bool bIsRightClicking = false;


public:
	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MLClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MRClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ZAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RAction;


public:
	UPROPERTY(EditDefaultsOnly, Category = "CircularWidget")
	TSubclassOf<UUserWidget> DestWidget;

	// HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UT1HUDWidget> T1HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UT1HUDWidget> T1HUDWidget;


private:
	FVector CachedDestination;
	FRotator CachedLookRotate;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed




};
