// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/T1Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Collision/T1Collision.h"
#include "Engine/DamageEvents.h"
#include "Character/T1Character.h"

// Sets default values
AT1Bullet::AT1Bullet()
{
    // Use a sphere as a simple collision representation
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &AT1Bullet::OnHit);  // set up a notification for when this component hits something blocking


    CollisionComp->SetCollisionProfileName(CPROFILE_ABCAPSULE);

    // Players can't walk on it
    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComp->CanCharacterStepUpOn = ECB_No;

    // Set as root component
    RootComponent = CollisionComp;

    // Create and set up the static mesh component
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
    MeshComp->SetupAttachment(RootComponent);

    // Use a ProjectileMovementComponent to govern this projectile's movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;

    // Disable gravity effect
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    // Die after 3 seconds by default
    InitialLifeSpan = 10.0f;

}

void AT1Bullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    //// Only add impulse and destroy projectile if we hit a physics
    //if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
    //{
    //    

    //    OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

    //    AttackHitCheck();

    //    Destroy();
    //}

    UE_LOG(LogTemp, Log, TEXT("Bullet hit something"));

    if (OtherActor)
    {
        UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *OtherActor->GetName());
        
        AT1Character* otherAct = Cast<AT1Character>(OtherActor);

        if (otherAct)
        {
            otherAct->TakeRangeDamage(BulletDamage);
        }
        
        Destroy();
    }

    //if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
    //{
    //    OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
    //    UE_LOG(LogTemp, Log, TEXT("Applied impulse at location: %s"), *GetActorLocation().ToString());

    //    AttackHitCheck();

    //    Destroy();
    //    UE_LOG(LogTemp, Log, TEXT("Bullet destroyed after hitting a physics object"));
    //}


}

