// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

USTRUCT(BlueprintType)
struct FMember {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Member")
	FString mName = "";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
	int mPoints = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
		TArray<FString> mKollektiver;
	FMember() {
		mName = "";
		mPoints = 0;
	}
};

USTRUCT(BlueprintType)
struct FRoom {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		FString mRoomName = "Task";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		FString mRoomDesc = "Description";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		FString mKollektiv = "";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		TArray<FString> mTasks;
	FRoom() {

	}

};

USTRUCT(BlueprintType)
struct FTask {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		FString mTaskName = "Task";							  
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		FString mTaskDesc = "Description";					  
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		FString mKollektiv = "";				   
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		FString mMemberAssigned = "Adam";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		bool mIsFinished = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		FString mRoom = "";
	FTask() {

	}

};


USTRUCT(BlueprintType)
struct FKollektiv {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kollektiv")
	FString mName = "Kollektiv";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kollektiv")
	TArray<FString> mMembers;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kollektiv")
		TArray<FString> mRooms;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kollektiv")
		TArray<FString> mTasks;
	FKollektiv() {
		mName = "";

	}

};



UCLASS()
class KOLLEKTIVMANAGER_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<FMember> allMembers;
	TArray<FKollektiv> allKollektiver;
	TArray<FMember> allRooms;
	TArray<FKollektiv> allTasks;


	//Signing into an member account
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
	FMember signedInMember;
	UFUNCTION(BlueprintCallable)
		FMember GetMember(FString name);
//Kollektiver
	UFUNCTION(BlueprintCallable)
		FKollektiv GetKollektiv(FString name);
	UFUNCTION(BlueprintCallable)
		FKollektiv NewKollektiv(FString name, TArray<FString> members);
	UFUNCTION(BlueprintCallable)
		bool SignIn(FString memberName);

	void SaveKollektiv(FKollektiv k);
	
	void SaveMember(FMember m);

	void AddMember(FMember m);

	void AddKollektiv(FKollektiv k);
	
	UFUNCTION(BlueprintCallable)
		TArray<FKollektiv> GetSignedInMembersKollektiver();
};
