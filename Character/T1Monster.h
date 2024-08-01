// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/T1Character.h"
#include "Interface/T1CharacterAIInterface.h"
#include "T1Monster.generated.h"

/**
 * 
 */
UCLASS()
class T1_API AT1Monster : public AT1Character, public IT1CharacterAIInterface
{
	GENERATED_BODY()
public:
	AT1Monster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDead() override;

	// AI Section
protected:

	// IT1CharacterAIInterface을(를) 통해 상속됨
	float GetAIPatrolRadius() override;

	float GetAIDetectRange() override;

	float GetAIAttackRange() override;

	float GetAITurnSpeed() override;

	void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;

	void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;

	// IT1CharacterAIInterface을(를) 통해 상속됨
	void AttackEnd() override;
};
