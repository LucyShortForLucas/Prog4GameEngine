#include "FpsTracker.h"
#include "Actor.h"
#include "TextRenderer.h"
#include <string>

#include "Services.h"

namespace eng {

void FpsTracker::OnEnable() {
	assert(Owner().GetComponent<TextRenderer>() && "An FpsTracker Component requires a Text Renderer");
	m_TextRendererPtr = Owner().GetComponent<TextRenderer>();
}

void FpsTracker::OnDisable() {
	m_TextRendererPtr = nullptr;
}

void eng::FpsTracker::Update() {
	float deltaTime{ service::gameTime.Get().DeltaTime() };

	//Only update this component every ~0.5 seconds so our fps is actually readable
	static double f_timePassed{};
	if (f_timePassed += deltaTime; f_timePassed < 0.5) return;
	f_timePassed = 0;

	int fps = static_cast<int>(1 / deltaTime);

	m_TextRendererPtr->SetText("fps: " + std::to_string(fps));
}

} // !eng