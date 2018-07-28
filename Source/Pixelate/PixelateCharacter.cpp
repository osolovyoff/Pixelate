// Fill out your copyright notice in the Description page of Project Settings.

#include "PixelateCharacter.h"
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

const FName APixelateCharacter::MoveForwardBinding("MoveForward");
const FName APixelateCharacter::MoveRightBinding("MoveRight");

// Sets default values
APixelateCharacter::APixelateCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

    //Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

    // Cache our sound effect
    static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
    FireSound = FireAudio.Object;

    // Weapon
    GunOffset = FVector(90.f, 0.f, 0.f);
    FireRate = 0.1f;
    bCanFire = true;
}

// Called to bind functionality to input
void APixelateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("ActionFire", IE_Pressed, this, &APixelateCharacter::FireShot);
    PlayerInputComponent->BindAxis(MoveForwardBinding);
    PlayerInputComponent->BindAxis(MoveRightBinding);
}

// Called when the game starts or when spawned
void APixelateCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APixelateCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    Slide(DeltaTime);
}

void APixelateCharacter::FireShot()
{
    const FVector  GufOffset(1);
    const FRotator FireRotation = GetActorRotation();
    const FVector  SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

    UWorld* const World = GetWorld();
    if (World != NULL)
        World->SpawnActor<APixelateProjectile>(SpawnLocation, FireRotation);
}

void APixelateCharacter::Syncronization_Implementation()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Called on client and executed on server !"));
}

bool APixelateCharacter::Syncronization_Validate()
{
    return true;
}

void APixelateCharacter::Slide(float DeltaSeconds)
{
    //const float Forward = GetInputAxisValue(MoveForwardBinding);
    //const float RotationAngle = GetInputAxisValue(MoveRightBinding);
    //if (RotationAngle)
    //{
    //    const FRotator rotation(0.f, 0.f, 60 * DeltaSeconds*25.f);
    //    Controller->SetControlRotation(rotation);
    //    //AddMovementInput()
    //    //RootComponent->SetWorldRotation(RootComponent->RelativeRotation.Quaternion() * FQuat::MakeFromEuler(FVector(0.f, 0.0f, 60 * DeltaSeconds*25.f) * RotationAngle));
    //}

    //const float MaxVelocityMagnitude = 100.0f;
    //if (Forward != 0.0f)
    //{
    //    const float EnginePower = 1000.0f;
    //    const FVector ForwardVector = GetActorForwardVector();
    //    const FVector Direction = ForwardVector * EnginePower * Forward;
    //    AddMovementInput(Direction, EnginePower);
    //    //ShipMeshComponent->AddForce(ForwardVector * EnginePower * Forward, NAME_None, true);
    //}
}