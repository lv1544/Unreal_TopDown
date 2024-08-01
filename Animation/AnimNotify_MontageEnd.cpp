// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MontageEnd.h"
#include "Interface/T1MontageEndInterface.h"

void UAnimNotify_MontageEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
		IT1MontageEndInterface* AttackPawn = Cast<IT1MontageEndInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->OnMontageEnd();
		}
	}


}
