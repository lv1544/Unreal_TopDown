// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/T1HUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Interface/T1PlayerHUDInterface.h"

UT1HUDWidget::UT1HUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UT1HUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

    HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
    ensure(HpBar);
    CurHp = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurHp")));
    ensure(CurHp);

    CurrentBullet = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentBullet")));

    AxeImage = Cast<UImage>(GetWidgetFromName(TEXT("AxeImage")));

    GunImage = Cast<UImage>(GetWidgetFromName(TEXT("GunImage")));

    AmmoPanel = Cast<UPanelWidget>(GetWidgetFromName(TEXT("AmmoPanel")));

    IT1PlayerHUDInterface* HUDPawn = Cast<IT1PlayerHUDInterface>(GetOwningPlayerPawn());
    if (HUDPawn)
    {
        HUDPawn->SetupHUDWidget(this);
    }



}

void UT1HUDWidget::SetHealth(float NewCurrentHp,float MaxHp)
{
    HpBar->SetPercent(NewCurrentHp / MaxHp);
    CurHp->SetText(FText::AsNumber(NewCurrentHp));
}


void UT1HUDWidget::SetCurrentAmmo(int32 AmmoInMagazine, int32 ReserveAmmo)
{
    CurrentBullet->SetText(FText::AsNumber(AmmoInMagazine));
    RemainBullet->SetText(FText::AsNumber(ReserveAmmo));
}

void UT1HUDWidget::SetWeaponChange()
{
    if (AxeImage && GunImage)
    {
        if (AxeImage->GetVisibility() == ESlateVisibility::Visible)
        {
            AxeImage->SetVisibility(ESlateVisibility::Hidden);
            GunImage->SetVisibility(ESlateVisibility::Visible);
            AmmoPanel->SetVisibility(ESlateVisibility::Visible);
        }
        else if (GunImage->GetVisibility() == ESlateVisibility::Visible)
        {
            GunImage->SetVisibility(ESlateVisibility::Hidden);
            AmmoPanel->SetVisibility(ESlateVisibility::Hidden);
            AxeImage->SetVisibility(ESlateVisibility::Visible);
        }
    }


}

