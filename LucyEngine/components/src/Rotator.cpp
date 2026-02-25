#include "Rotator.h"
#include "Actor.h"
#include "TextRenderer.h"
#include <string>

#include "Services.h"

namespace eng {

void eng::Rotator::Update() {
	float rad{service::gameTime.Get().DeltaTime()};
	float cosrad{ std::cos(rad) };
	float sinrad{ std::sin(rad) };
	float x{ Owner().GetTransform().GetLocal().position.x };
	float y{ Owner().GetTransform().GetLocal().position.y };
	float newX{ x * cosrad - y * sinrad };
	float newY{x*sinrad + y * cosrad};
	Owner().GetTransform().SetLocalPosition(newX, newY);
}

} // !eng