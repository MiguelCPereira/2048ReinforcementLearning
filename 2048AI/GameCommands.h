#pragma once
#include "Command.h"
//#include "GameManager.h"

class SwipeLeftCommand final : public dae::Command
{
public:
	void Execute() override { /*GetActor()->GetComponent<GameManager>()->SwipeLeft();*/ }
};

class SwipeRightCommand final : public dae::Command
{
public:
	void Execute() override { /*GetActor()->GetComponent<GameManager>()->SwipeRight();*/ }
};

class SwipeUpCommand final : public dae::Command
{
public:
	void Execute() override { /*GetActor()->GetComponent<GameManager>()->SwipeUp();*/ }
};

class SwipeDownCommand final : public dae::Command
{
public:
	void Execute() override { /*GetActor()->GetComponent<GameManager>()->SwipeDown();*/ }
};