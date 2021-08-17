#pragma once

namespace dae
{
	class TextComponent;
	class GameObject;

	enum class Event
	{
		// Only the engine demo uses these
		ActorHPChange,
		ActorDeath,
		PointsIncrease,
		SlickSamCaught,
		TileChange,
		ColorChange,


		// Write any extra events here
		ScoreIncreased
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(const Event& event) = 0;
	};
}

