#pragma once

#include <ActorJsonFactory.h>

namespace tron {

nlohmann::ordered_json BuildLevel(const std::string& levelBackgroundPath, const std::string& levelCollisionPath);

}