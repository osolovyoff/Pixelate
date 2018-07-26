// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "PixelatePawn.h"
#include "PixelateProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Engine/Engine.h"

const FName APixelatePawn::MoveForwardBinding("MoveForward");
const FName APixelatePawn::MoveRightBinding("MoveRight");
const FName APixelatePawn::FireForwardBinding("FireForward");
const FName APixelatePawn::FireRightBinding("FireRight");

APixelatePawn::APixelatePawn()
{	
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
    // Create the mesh component
    ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
    RootComponent = ShipMeshComponent;
    ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
    ShipMeshComponent->SetSimulatePhysics(true);
    ShipMeshComponent->GetBodyInstance()->bLockZTranslation = true;
    ShipMeshComponent->GetBodyInstance()->bLockXRotation = true;
    ShipMeshComponent->GetBodyInstance()->bLockYRotation = true;
    ShipMeshComponent->GetBodyInstance()->LinearDamping = 1.f;
    ShipMeshComponent->GetBodyInstance()->AngularDamping = 20.0f;

    // Cache our sound effect
    static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
    FireSound = FireAudio.Object;

    // Weapon
    GunOffset = FVector(90.f, 0.f, 0.f);
    FireRate = 0.1f;
    bCanFire = true;
}

void APixelatePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    PlayerInputComponent->BindAction("ActionFire", IE_Pressed, this, &APixelatePawn::FireShot);

    // set up gameplay key bindings
    PlayerInputComponent->BindAxis(MoveForwardBinding);
    PlayerInputComponent->BindAxis(MoveRightBinding);
    PlayerInputComponent->BindAxis(FireForwardBinding);
    PlayerInputComponent->BindAxis(FireRightBinding);
}

void APixelatePawn::Tick(float DeltaSeconds)
{
    Slide(DeltaSeconds);
}

void APixelatePawn::FireShot()
{
    const FVector  GufOffset(1);
    const FRotator FireRotation = GetActorRotation();
    const FVector  SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

    UWorld* const World = GetWorld();
    if (World != NULL)
        World->SpawnActor<APixelateProjectile>(SpawnLocation, FireRotation);
}

void APixelatePawn::ShotTimerExpired()
{
	bCanFire = true;
}

void APixelatePawn::Syncronization_Implementation()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Called on client and executed on server !"));
}

bool APixelatePawn::Syncronization_Validate()
{
    return true;
}

void APixelatePawn::Slide(float DeltaSeconds)
{
    const float Forward = GetInputAxisValue(MoveForwardBinding);
    const float RotationAngle = GetInputAxisValue(MoveRightBinding);
    if (RotationAngle)
        RootComponent->SetWorldRotation(RootComponent->RelativeRotation.Quaternion() * FQuat::MakeFromEuler(FVector(0.f, 0.0f, 60 * DeltaSeconds*4.f) * RotationAngle));

    const float MaxVelocityMagnitude = 100.0f;
    if (Forward != 0.0f)
    {
        const float EnginePower = 1000.0f;
        const FVector forwardVector = GetActorForwardVector();
        ShipMeshComponent->AddForce(forwardVector * EnginePower * Forward, NAME_None, true);
    }
}
