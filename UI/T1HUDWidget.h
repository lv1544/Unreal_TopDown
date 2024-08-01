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
    // �������� ���ε��� ��ҵ�
    UPROPERTY(meta = (BindWidget))
    TObjectPtr <class UProgressBar> HpBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr <class UTextBlock> CurHp;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr <class UTextBlock> CurrentBullet;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr <class UTextBlock> RemainBullet;

     //�߰��� ������ �� �̹��� ����
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> AxeImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> GunImage;

    // ���ο� �г� ���� ���ε�
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UPanelWidget> AmmoPanel;

    // �Լ��� ���� ������Ʈ
    void SetHealth(float HealthPercentage,float MaxHp);
    void SetCurrentAmmo(int32 AmmoInMagazine, int32 ReserveAmmo);
    void SetWeaponChange();

};
