// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/T1Character.h"
#include "Collision/T1Collision.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/T1AIController.h"

// Sets default values
AT1Character::AT1Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	LastDamageTime = 0.0f;
	DamageCooldown = 0.5f; // 2 seconds cooldown

	IsMontagePlay = false;
}

// Called when the game starts or when spawned
void AT1Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AT1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AT1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AT1Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// Get the current game time in seconds
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// Check if the cooldown period has passed
	if (CurrentTime - LastDamageTime < DamageCooldown) return 0;

	if (CurHP <= 0.0f) return 0;

	// Set the last damage time to current time
	LastDamageTime = CurrentTime;

	GetController()->StopMovement();

	AT1AIController* ai = Cast<AT1AIController>(GetController());

	if (ai)
	{
		ai->StopAI();
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(OnHitMontage))
	{
		AnimInstance->Montage_Play(OnHitMontage);
	}

	if (DamageAmount <= 0.0f) return 0.f;

	// 애니메이션 끝났을 때 이벤트 바인딩
	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindUObject(this, &AT1Character::OnHitMontageEnded);
	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, OnHitMontage);

	// 현재 체력에서 데미지를 뺀다
	CurHP -= DamageAmount;

	// 체력이 0 이하로 떨어지면 죽음 처리
	if (CurHP <= 0.0f)
	{
		CurHP = 0.0f; // 체력은 0 이하로 떨어지지 않도록 설정
		SetDead();
	}


	return DamageAmount;
}

void AT1Character::TakeRangeDamage(float DamageAmount)
{
	if (DamageAmount <= 0.0f) return;

	// Get the current game time in seconds
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// Check if the cooldown period has passed
	if (CurrentTime - LastDamageTime < DamageCooldown) return ;

	if (CurHP <= 0.0f) return ;

	// Set the last damage time to current time
	LastDamageTime = CurrentTime;

	GetController()->StopMovement();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(OnHitMontage))
	{
		AnimInstance->Montage_Play(OnHitMontage);
	}

	AT1AIController* ai = Cast<AT1AIController>(GetController());

	if (ai)
	{
		ai->StopAI();
	}

	// 애니메이션 끝났을 때 이벤트 바인딩
	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindUObject(this, &AT1Character::OnHitMontageEnded);
	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, OnHitMontage);


	// 현재 체력에서 데미지를 뺀다
	CurHP -= DamageAmount;

	// 체력이 0 이하로 떨어지면 죽음 처리
	if (CurHP <= 0.0f)
	{
		CurHP = 0.0f; // 체력은 0 이하로 떨어지지 않도록 설정
		SetDead();
	}

}



void AT1Character::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 50.0f;
	const float AttackRadius = 60.0f;

	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(MeleeDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	//FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	//float CapsuleHalfHeight = AttackRange * 0.5f;
	//FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	//DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

void AT1Character::MeleAttacking()
{
	if (!bIsRange && MeleeAttackMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && !AnimInstance->Montage_IsPlaying(MeleeAttackMontage))
		{
			AnimInstance->Montage_Play(MeleeAttackMontage);
		}
	}

}

void AT1Character::OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == OnHitMontage)
	{

		AT1AIController* ai = Cast<AT1AIController>(GetController());

		if (ai && CurHP >0)
		{
			ai->RunAI();
		}
	}
}

void AT1Character::SetDead()
{

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);


}

void AT1Character::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);

}

