// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "T1HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class T1_API UT1HUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UT1HUDWidget(const FObjectInitializer& ObjectInitializer);


protected:
	virtual void NativeConstruct() override;

public:
    // 위젯에서 바인딩할 요소들
    UPROPERTY(meta = (BindWidget))
    TObjectPtr <class UProgressBar> HpBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr <class UTextBlock> CurHp;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr <class UTextBlock> CurrentBullet;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr <class UTextBlock> RemainBullet;

     //추가된 도끼와 총 이미지 위젯
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> AxeImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> GunImage;

    // 새로운 패널 위젯 바인딩
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UPanelWidget> AmmoPanel;

    // 함수로 위젯 업데이트
    void SetHealth(float HealthPercentage,float MaxHp);
    void SetCurrentAmmo(int32 AmmoInMagazine, int32 ReserveAmmo);
    void SetWeaponChange();

};
