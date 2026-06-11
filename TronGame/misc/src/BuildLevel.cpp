#include "BuildLevel.h"

#include <ColorMap.h>
#include <Services.h>

namespace tron {

constexpr float TILESIZE{ 32 };

nlohmann::ordered_json BuildLevel(const std::string& levelBackgroundPath, const std::string& levelCollisionPath) {
	eng::ActorJsonFactory factory{};

	// Set up main level object, which has background, pathfind grid, and is tagsetter with tag "LevelHead"
	factory
		.AddComponent("TextureRenderer")
		.AddProperty("TexturePath", levelBackgroundPath)
		.AddProperty("Size", glm::vec2{ 968, 1032 })
		.Owner()
		.AddComponent("TagSetter")
		.AddProperty("Tags", std::vector<std::string>{"LevelHead"})
		.Owner()
		.AddChildActor()
		.AddComponent("FpsTracker")
		.Owner()
		.AddComponent("TextureRenderer")
		.Owner()
		.AddComponent("TextRenderer");

// Create level colliders

	eng::BlackWhiteMap<30, 28> colliderGrid{ *eng::service::resources.Get().LoadColorMap(levelCollisionPath) };

	glm::vec2 arenaStartPos{ 0, 136 };

	// Horizontal pass
	for (int y{}; y < colliderGrid.ColumnSize(); ++y) {
		int startXTile{ -1 };
		int tileCount{};

		for (int x{}; x < colliderGrid.RowSize(); ++x) {
			if (colliderGrid.At(x, y)) {
				if (startXTile == -1) {
					if (!(x > 0 && colliderGrid.At(x - 1, y) || x < colliderGrid.RowSize() - 1 && colliderGrid.At(x + 1, y)) // ignore if not attached horizontally but attached vetically
						&& (y > 0 && colliderGrid.At(x, y - 1) || y < colliderGrid.ColumnSize() - 1 && colliderGrid.At(x, y + 1))
						)
						continue;

					startXTile = x;
				}

				++tileCount;
			}
			if (startXTile != -1 && (!colliderGrid.At(x, y) || x == colliderGrid.RowSize() - 1)) {
				glm::vec2 startPos{ arenaStartPos.x + startXTile * TILESIZE, arenaStartPos.y + y * TILESIZE };
				float length{ TILESIZE * tileCount };

				factory.AddChildActor()
					.AddComponent("AabbCollider")
						.AddProperty("Bounds", SDL_FRect{ startPos.x, startPos.y, length, TILESIZE })
						.Owner()
					.AddComponent("PhysicsBody")
						.AddProperty("Static", true);

				startXTile = -1;
				tileCount = 0;
			}
		}
	}


		// Vertical pass
		for (int x{}; x < colliderGrid.RowSize(); ++x) {
			int startYTile{ -1 };
			int tileCount{};

			for (int y{}; y < colliderGrid.ColumnSize(); ++y) {
				if (colliderGrid.At(x, y)) {
					if (startYTile == -1) {
						if (!(y > 0 && colliderGrid.At(x, y - 1) || y < colliderGrid.ColumnSize() - 1 && colliderGrid.At(x, y + 1))) // ignore if not attached  vetically
							continue;

						startYTile = y;
					}

					++tileCount;
				}
				if (startYTile != -1 && (!colliderGrid.At(x, y) || y == colliderGrid.ColumnSize() - 1)) {
					glm::vec2 startPos{ arenaStartPos.x + x * TILESIZE, arenaStartPos.y + startYTile * TILESIZE };
					float length{ TILESIZE * tileCount };

					factory.AddChildActor()
						.AddComponent("AabbCollider")
							.AddProperty("Bounds", SDL_FRect{ startPos.x, startPos.y, TILESIZE, length })
							.Owner()
						.AddComponent("PhysicsBody")
							.AddProperty("Static", true);


					startYTile = -1;
					tileCount = 0;
				}
			}
		}

		return factory.Build();
	}

}
