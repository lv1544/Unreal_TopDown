// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackEnd.h"
#include "Interface/T1CharacterAIInterface.h"

void UAnimNotify_AttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
		IT1CharacterAIInterface* AttackPawn = Cast<IT1CharacterAIInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackEnd();
		}
	}

}
