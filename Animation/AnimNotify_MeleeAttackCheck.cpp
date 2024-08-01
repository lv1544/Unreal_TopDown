// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_MeleeAttackCheck.h"
#include "Interface/T1DamageInterface.h"

void UAnimNotify_MeleeAttackCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
		IT1DamageInterface* AttackPawn = Cast<IT1DamageInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}

}
