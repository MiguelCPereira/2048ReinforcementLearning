#include "ScoreDisplay.h"
#include "GameLogic.h"
#include <string>
#include "ResourceManager.h"
#include "TextComponent.h"



ScoreDisplay::ScoreDisplay(GameLogic* gameLogic, dae::TextComponent* textComp)
	: m_GameLogicComp(gameLogic)
	, m_TextComp(textComp)
{
	Initialize();
}

ScoreDisplay::~ScoreDisplay()
{
	if (m_GameLogicComp != nullptr)
		m_GameLogicComp->GetSubject()->RemoveObserver(this);

	m_TextComp = nullptr;
}


void ScoreDisplay::Initialize()
{
	if (m_GameLogicComp != nullptr)
	{
		m_GameLogicComp->GetSubject()->AddObserver(this);
		UpdateScoreText();
	}
}

void ScoreDisplay::OnNotify(const dae::Event& event)
{
	switch (event)
	{
	case dae::Event::ScoreIncreased:
		UpdateScoreText();
		break;
	}
}

void ScoreDisplay::Update(const float)
{
}

void ScoreDisplay::UpdateScoreText() const
{
	std::string string = "SCORE: ";
	string.append(std::to_string(m_GameLogicComp->GetScore()));
	m_TextComp->SetText(string);
}
