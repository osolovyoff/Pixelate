// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PixelateCharacter.generated.h"

UCLASS(config = Game)
class PIXELATE_API APixelateCharacter : public ACharacter
{
	GENERATED_BODY()

    /* The mesh component */
    UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* ShipMeshComponent;

    /** The camera */
    UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* CameraComponent;

    /** Camera boom positioning the camera above the character */
    UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

public:
	// Sets default values for this character's properties
	APixelateCharacter();

    /** Offset from the ships location to spawn projectiles */
    UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    FVector GunOffset;

    /* How fast the weapon will fire */
    UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    float FireRate;

    /* The speed our ship moves around the level */
    UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
    float MoveSpeed;

    /** Sound to play each time we fire */
    UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
    class USoundBase* FireSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
    // Static names for axis bindings
    static const FName MoveForwardBinding;
    static const FName MoveRightBinding;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /* Fire a shot in the specified direction */
    void FireShot();

    UFUNCTION(Reliable, Server, WithValidation)
    void Syncronization();
    void Syncronization_Implementation();
    bool Syncronization_Validate();

private:
    /* Flag to control firing  */
    uint32 bCanFire : 1;
	
private:
    void Slide(float DeltaSeconds);

public:
    /** Returns ShipMeshComponent subobject **/
    FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
    /** Returns CameraComponent subobject **/
    FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
