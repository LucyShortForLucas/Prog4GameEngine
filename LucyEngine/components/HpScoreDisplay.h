#pragma once														
																	
#include "AbstractComponent.h"										
#include "TextRenderer.h"											
#include "EventSource.h"											
#include "HpScore.h"												
																	
namespace eng {														
																	
class HpScoreDisplay final : public AbstractComponent, 				
	public AbstractEventListener<event::LivesChanged>,				
	public AbstractEventListener<event::ScoreChanged> {				
public: 
	//---- Constructor/Destructor/copy/move ----													
	HpScoreDisplay(Actor& owner) : AbstractComponent(owner) {};		
	~HpScoreDisplay() = default;									
																	
	HpScoreDisplay(const HpScoreDisplay&) = delete;					
	HpScoreDisplay& operator=	(const HpScoreDisplay&) = delete;	
																	
	HpScoreDisplay(HpScoreDisplay&&) = delete;						
	HpScoreDisplay& operator=	(HpScoreDisplay&&) = delete;		
																	
	//---- Event methods ----
																	
	void OnEvent(const event::LivesChanged& event) override;		
	void OnEvent(const event::ScoreChanged& event) override;		
																	
private:
	//----- Text data -----														
	std::string m_LiveDisplayText{ "# lives: 3" };					
	std::string m_ScoreDisplayText{"Score: 0"};						
																	
	//----- Helper method -----
																	
	void UpdateDisplay();											
																	
}; // !HpScoreDisplay												
																			   
} // !eng																	   