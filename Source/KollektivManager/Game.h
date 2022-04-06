// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class KOLLEKTIVMANAGER_API Avatar {
public:

};
class Kollektiv;
class KOLLEKTIVMANAGER_API Member {
public:
	Member(FString name) : mName{ name } {};
	Member(FString name, int points) : mName{name}, mPoints{points} {};
	FString mName = "";
	int mPoints = 0;
	TArray<Kollektiv*> mKollektiver;
};

class KOLLEKTIVMANAGER_API Task {
public:
	//Task(FString name, FString mDesc);
	FString mTaskName;
	FString mTaskDescription;
	bool bDone;
	Member* mMember = nullptr;
	void DoTask() { bDone = true; };

};
class KOLLEKTIVMANAGER_API Room {
public:
	FString mRoomName;
	TArray<Task*> mTasks;
	bool AddTask(Task* t) { mTasks.Push(t); return true; };
};

class KOLLEKTIVMANAGER_API Kollektiv {
public:
	Kollektiv(FString kollektivName) : mName{ kollektivName } {};
	FString mName = "Kollektiv";
	TArray<Member*> mMembers;
	TArray<Room*> mRooms;
	

};

class KOLLEKTIVMANAGER_API Game
{
public:
	Game();
	~Game();

	Member* signedInMember = nullptr;
	Kollektiv* mActiveKollektiv;

	TArray<Member*> allMembers;
	TArray<Kollektiv*> allKollektiver;
	UFUNCTION(BlueprintCallable)
	void SignIn(FString memberName);

	Kollektiv* LoadKollektiv(FString kollektivName);
	void SaveKollektiv(Kollektiv* k);
	
	Member* LoadMember(FString memberName);
	void SaveMember(Member* m);

	void AddMember(Member* m);
	void AddKollektiv(Kollektiv* k);
};
