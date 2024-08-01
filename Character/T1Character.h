// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/T1DamageInterface.h"
#include "T1Character.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

UCLASS()
class T1_API AT1Character : public ACharacter ,public IT1DamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AT1Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void TakeRangeDamage(float DamageAmount);

	// IT1DamageInterface을(를) 통해 상속됨
	void AttackHitCheck() override;

	void MeleAttacking();
	//AnimMoatage
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> MeleeAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> OnHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

	float LastDamageTime;
	float DamageCooldown;

	UFUNCTION()
	void OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted);


public:
	FOnHpZeroDelegate OnHpZero;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", Meta = (AllowPrivateAccess = "true"))
	float CurHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", Meta = (AllowPrivateAccess = "true"))
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", Meta = (AllowPrivateAccess = "true"))
	float MeleeDamage;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsCrouch = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsRange = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsMontagePlay;

	// Dead Section
protected:


	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;
};
