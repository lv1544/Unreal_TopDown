// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/T1Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapon/T1Bullet.h"
#include "Components/ArrowComponent.h"
#include "UI/T1HUDWidget.h"

AT1Player::AT1Player()
{
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	SpringArm->TargetArmLength = 800.f;
	SpringArm->SetRelativeRotation(FRotator(-60, 0, 0));

	// Weapon Component
	EquipWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipWeapon"));
	EquipWeapon->SetupAttachment(RootComponent);; // ArrowComponent에 무기 부착
	EquipWeapon->SetSkeletalMesh(GunMesh); // AxeMesh를 실제 사용 중인 스켈레탈 메쉬 변수로 교체하세요
	EquipWeapon->SetRelativeLocation(FVector(-20.0f, 9.0f, 10.0f));
	EquipWeapon->SetRelativeRotation(FRotator(0.0f, -90.f, 0.f));


	EquipWeapon1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipWeapon1"));
	EquipWeapon1->SetupAttachment(GetMesh(), "hand_rSocket"); 
	EquipWeapon1->SetSkeletalMesh(AxeMesh); // AxeMesh를 실제 사용 중인 스켈레탈 메쉬 변수로 교체하세요
	EquipWeapon1->SetRelativeLocation(FVector(0.f, 0.f, 0.0f));
	EquipWeapon1->SetRelativeRotation(FRotator(0.0f, 0.f, 0.f));

}

void AT1Player::BeginPlay()
{
	Super::BeginPlay();

	MaxHP = 200;
	
	CurHP = MaxHP;
	
	MeleeDamage = 100;

	ItemChange();

	TotalAmmo = 100;

	MaxAmmoInMagazine = 30;

	RemainingAmmo = TotalAmmo - MaxAmmoInMagazine;

	AmmoInMagazine = MaxAmmoInMagazine;

	OnHpChanged.Broadcast(CurHP, MaxHP);
}

void AT1Player::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);

		//IABGameInterface* ABGameMode = Cast<IABGameInterface>(GetWorld()->GetAuthGameMode());
		//if (ABGameMode)
		//{
		//	ABGameMode->OnPlayerDead();
		//}
	}
}

void AT1Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AT1Player::Crouching()
{
	bIsCrouch = !bIsCrouch;

	if (bIsCrouch)
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchMaxWalkSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalMaxWalkSpeed;
	}
}


void AT1Player::RangeAttacking(FVector hitpoint)
{
	if (AmmoInMagazine <= 0)
		return;

	if (AmmoInMagazine > 0)
		AmmoInMagazine--;


	if (bIsRange && RangeAttackMontage)
	{
		//EquipWeapon->SetupAttachment(GetArrowComponent()); // ArrowComponent에 무기 부착
		//EquipWeapon->SetSkeletalMesh(GunMesh); // AxeMesh를 실제 사용 중인 스켈레탈 메쉬 변수로 교체하세요
		//EquipWeapon->SetRelativeLocation(FVector(-20.0f, 19.0f, 0.0f));
		//EquipWeapon->SetRelativeRotation(FRotator(0.0f, -90.f, 0.f));



		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && !AnimInstance->Montage_IsPlaying(RangeAttackMontage))
		{
			AnimInstance->Montage_Play(RangeAttackMontage);
		}

		// 총구 위치에서 목표 지점까지의 방향 벡터를 계산합니다.
		FVector MuzzleLocation = EquipWeapon->GetSocketLocation(TEXT("Muzzle")); // 총구 위치

		// 총신의 포워드 벡터 가져오기
		FVector ForwardVector = EquipWeapon->GetRightVector();

		// 목표 지점의 Z 좌표를 총구의 Z 좌표로 고정
		FVector AdjustedHitPoint = FVector(hitpoint.X, hitpoint.Y, MuzzleLocation.Z);
		FVector Direction = ForwardVector.GetSafeNormal();

		// 디버그 광선을 총구에서 히트 포인트로 그립니다.
		//FVector EndLocation = MuzzleLocation + (Direction * 10000.0f); // 10000.0f는 광선 길이
		//DrawDebugLine(GetWorld(), MuzzleLocation, EndLocation, FColor::Red, false, 2.0f, 0, 2.0f);

		
		// 발사체 스폰
		if (Bullet != nullptr) // Check if the Bullet class is set
		{
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				// Spawn the bullet at the muzzle location
				AActor* SpawnedBullet = World->SpawnActor<AT1Bullet>(Bullet,MuzzleLocation, Direction.Rotation(), SpawnParams);
				if (SpawnedBullet)
				{
					// Additional setup for SpawnedBullet if needed
				}
			}
		}

	}

	OnAmmoChanged.Broadcast(AmmoInMagazine, RemainingAmmo);



}

void AT1Player::ItemChange()
{
	if (bIsReloading)
		return;
	
	bIsRange = !bIsRange;

		// 기존 무기 메쉬 제거
	if (bIsRange && GunMesh)
	{
		EquipWeapon->SetVisibility(true);
		EquipWeapon->SetHiddenInGame(false);
		EquipWeapon1->SetVisibility(false);
		EquipWeapon1->SetHiddenInGame(true);

		EquipWeapon->SetSkeletalMesh(GunMesh); // AxeMesh를 실제 사용 중인 스켈레탈 메쉬 변수로 교체하세요
		EquipWeapon->SetRelativeLocation(FVector(-20.0f, 19.0f, 10.0f));
		EquipWeapon->SetRelativeRotation(FRotator(0.0f, -90.f, 0.f));

	}
	else if (!bIsRange && AxeMesh)
	{

		EquipWeapon1->SetVisibility(true);
		EquipWeapon1->SetHiddenInGame(false);
		EquipWeapon->SetVisibility(false);
		EquipWeapon->SetHiddenInGame(true);


		//// 도끼의 메쉬와 트랜스폼 설정
		EquipWeapon1->SetRelativeLocation(FVector(0.f, 0.f, 0.0f));
		EquipWeapon1->SetRelativeRotation(FRotator(0.0f, 0.f, 0.f));
	}

	OnWeaponChanged.Broadcast();
}

void AT1Player::Reload()
{
	if (MaxAmmoInMagazine <= AmmoInMagazine)
		return;

	if (bIsRange && GunMesh)
	{
		bIsReloading = true;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && !AnimInstance->Montage_IsPlaying(ReloadMontage))
		{
			AnimInstance->Montage_Play(ReloadMontage);
		}

		int32 AmmoNeeded = MaxAmmoInMagazine - AmmoInMagazine;
		if (RemainingAmmo >= AmmoNeeded)
		{
			AmmoInMagazine += AmmoNeeded;
			RemainingAmmo -= AmmoNeeded;
		}
		else
		{
			AmmoInMagazine += RemainingAmmo;
			RemainingAmmo = 0;
		}
	


		OnAmmoChanged.Broadcast(AmmoInMagazine, RemainingAmmo);

	}


}

float AT1Player::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	OnHpChanged.Broadcast(CurHP, MaxHP);

	return DamageAmount;
}

void AT1Player::SetupHUDWidget(UT1HUDWidget* InHUDWidget)
{
	TotalAmmo = 100;

	MaxAmmoInMagazine = 30;

	RemainingAmmo = TotalAmmo - MaxAmmoInMagazine;

	AmmoInMagazine = MaxAmmoInMagazine;


	if (InHUDWidget)
	{
		InHUDWidget->SetHealth(CurHP,MaxHP);
		InHUDWidget->SetCurrentAmmo(AmmoInMagazine,RemainingAmmo);

		OnHpChanged.AddUObject(InHUDWidget, &UT1HUDWidget::SetHealth);
		OnAmmoChanged.AddUObject(InHUDWidget, &UT1HUDWidget::SetCurrentAmmo);
		OnWeaponChanged.AddUObject(InHUDWidget, &UT1HUDWidget::SetWeaponChange);
	}


}

void AT1Player::OnMontageEnd()
{

	if (bIsReloading)
		bIsReloading = false;


}
