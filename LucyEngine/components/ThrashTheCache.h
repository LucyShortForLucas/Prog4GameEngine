#pragma once

#include "AbstractComponent.h"
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>


namespace eng {

class ThrashTheCache final : public AbstractComponent  {
public: //--------------- Constructor/Destructor/copy/move --------------

	ThrashTheCache(eng::Actor& owner) : AbstractComponent(owner) {};
	~ThrashTheCache() = default;

	ThrashTheCache(const ThrashTheCache&) = delete;
	ThrashTheCache& operator=	(const ThrashTheCache&) = delete;

	ThrashTheCache(const ThrashTheCache&&) = delete;
	ThrashTheCache& operator=	(const ThrashTheCache&&) = delete;


public: //--------------- Gameloop Methods --------------

	void RenderImgui() override;

private: //------------------- private Cache exercise helper methods --------

	template <typename T>
	static std::vector<int> TakeMeasurement(int sampleSize) {
		std::vector<T> arr(67'000'000);

		std::vector<int> measurements;

		for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2) {
			std::vector<int> subMeasurements{};
			for (int sampleId{}; sampleId < sampleSize; ++sampleId) {
				auto start = std::chrono::high_resolution_clock::now();
				for (int i{ 0 }; i < arr.size(); i += stepSize) {
					arr[i].id *= 2;
				}
				subMeasurements.emplace_back(static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count()));
			};
			std::ranges::sort(subMeasurements);
			std::rotate(subMeasurements.begin(), subMeasurements.begin() + 1, subMeasurements.end());
			subMeasurements.pop_back();
			subMeasurements.pop_back();

			measurements.emplace_back(std::accumulate(subMeasurements.begin(), subMeasurements.end(), 0) / static_cast<int>(subMeasurements.size()));
		};

		return measurements;
	}

	void MeasureInts(int sampleSize);

private: //-------------- Helper classes ---------------
	struct transform {
		float matrix[16] {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
		};
	};

	class gameObject3d {
	public:
		transform local{};
		int id{};
	};

	class gameObject3dAlt {
	public:
		transform* local{};
		int id{};
	};

private: //--------------------- MeasurementData ---------------

	std::vector<int> m_GameObject3dMeasurements{};
	std::vector<int> m_GameObject3dAltMeasurements{};
	std::vector<int> m_IntMeasurements{};
};

} // !eng


