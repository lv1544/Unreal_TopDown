// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/T1Character.h"
#include "Interface/T1PlayerHUDInterface.h"
#include "Interface/T1MontageEndInterface.h"
#include "T1Player.generated.h"



DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnWeaponChanged);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHpChangedDelegate, float /*CurrentHp*/, float /*Max*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAmmoChangedDelegate, int /*CurrentBullet*/,int /*remainBullet*/);

/**
 * 
 */
UCLASS()
class T1_API AT1Player : public AT1Character , public IT1PlayerHUDInterface , public IT1MontageEndInterface
{
	GENERATED_BODY()
	
public:
	AT1Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetDead() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void Crouching();

	void RangeAttacking(FVector hitpoint);

	void ItemChange();

	void Reload();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


public:
	bool bIsAttacking = false;
	bool bIsReloading = false;

	//AnimMoatage
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> RangeAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ReloadMontage;

	// Item Section
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> EquipWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> EquipWeapon1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TObjectPtr<class USkeletalMesh> AxeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TObjectPtr<class USkeletalMesh> GunMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TSubclassOf<class AT1Bullet> Bullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	int32 AmmoInMagazine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	int32 TotalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	int32 RemainingAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	int32 MaxAmmoInMagazine;


	FOnHpZeroDelegate OnHpZero;
	FOnWeaponChanged OnWeaponChanged;
	FOnHpChangedDelegate OnHpChanged;
	FOnAmmoChangedDelegate OnAmmoChanged;


public:
	float CrouchMaxWalkSpeed = 200.0f;
	float NormalMaxWalkSpeed = 600.0f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> Camera;


	// IT1PlayerHUDInterface을(를) 통해 상속됨
	void SetupHUDWidget(UT1HUDWidget* InHUDWidget) override;


	// IT1MontageEndInterface을(를) 통해 상속됨
	void OnMontageEnd() override;

};
