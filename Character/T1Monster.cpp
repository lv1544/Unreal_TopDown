// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/T1Monster.h"
#include "AI/T1AIController.h"

AT1Monster::AT1Monster()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	AIControllerClass = AT1AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AT1Monster::BeginPlay()
{
	Super::BeginPlay();

	MaxHP = 100;

	CurHP = MaxHP;
	MeleeDamage = 10;

	//AT1AIController* ai = Cast<AT1AIController>(GetController());

	//if (ai)
	//{
	//	ai->StopAI();
	//}

}

void AT1Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AT1Monster::SetDead()
{
	Super::SetDead();

	AT1AIController* ai = Cast<AT1AIController>(GetController());

	if (ai)
	{
		ai->StopAI();
	}

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);

}


float AT1Monster::GetAIPatrolRadius()
{
	return 1000.0f;
}

float AT1Monster::GetAIDetectRange()
{
	return 800.0f;
}

float AT1Monster::GetAIAttackRange()
{
	return 100.0f;
}

float AT1Monster::GetAITurnSpeed()
{
	return 2.0f;
}

void AT1Monster::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AT1Monster::AttackByAI()
{
	MeleAttacking();
}

void AT1Monster::AttackEnd()
{
	OnAttackFinished.ExecuteIfBound();
}
