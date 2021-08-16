#pragma once
#include "Command.h"
//#include "GameManager.h"

class SwipeLeftCommand final : public dae::Command
{
public:
	void Execute() override { /*GetActor()->GetComponent<GameLogic>()->SwipeLeft();*/ }
};

class SwipeRightCommand final : public dae::Command
{
public:
	void Execute() override { /*GetActor()->GetComponent<GameLogic>()->SwipeRight();*/ }
};

class SwipeUpCommand final : public dae::Command
{
public:
	void Execute() override { /*GetActor()->GetComponent<GameLogic>()->SwipeUp();*/ }
};

class SwipeDownCommand final : public dae::Command
{
public:
	void Execute() override { /*GetActor()->GetComponent<GameLogic>()->SwipeDown();*/ }
};