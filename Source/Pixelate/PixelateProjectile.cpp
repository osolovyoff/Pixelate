// Copyright 1998-2018 Epic Games, Inc. All Rights Reserve

#include "PixelateProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine.h"

APixelateProjectile::APixelateProjectile() 
{
    // Static reference to the mesh to use for the projectile
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));
    // static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Game/TwinStick/Meshes/RedMaterial.RedMaterial"));
    // static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PhysicsMaterial(TEXT("/Game/ProjectilePhysicsMaterial"));
    // if (PhysicsMaterial.Object != NULL)
    // {
    //     GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("Physics"/*FString::SanitizeFloat(Forward*/));
    // }

    // Create mesh component for the projectile sphere
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
    ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
    ProjectileMesh->SetupAttachment(RootComponent);
    ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
    ProjectileMesh->OnComponentHit.AddDynamic(this, &APixelateProjectile::OnHit);		// set up a notification for when this component hits something
    ProjectileMesh->SetSimulatePhysics(true);
    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

    //if (Material.Object != NULL)
    //{
    //    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("Material"/*FString::SanitizeFloat(Forward*/));
    //    TheMaterial = (UMaterial*)Material.Object;
    //    UMaterialInstanceDynamic* TheMaterial_Dynamic = UMaterialInstanceDynamic::Create(TheMaterial, this);
    //    ProjectileMesh->SetMaterial(0, TheMaterial_Dynamic);
    //}

    RootComponent = ProjectileMesh;

    // Use a ProjectileMovementComponent to govern this projectile's movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
    ProjectileMovement->UpdatedComponent = ProjectileMesh;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->Bounciness = 1.0f;
    ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity

    // Die after 3 seconds by default
    //InitialLifeSpan = 3.0f;
    ProjectileMesh->GetBodyInstance()->bLockZTranslation = true;
    ProjectileMesh->GetBodyInstance()->bLockXRotation = true;
    ProjectileMesh->GetBodyInstance()->bLockYRotation = true;
}

void APixelateProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 20.0f, GetActorLocation());
	}

	Destroy();
}