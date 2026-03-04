#include "ThrashTheCache.h"
#include <imgui.h>

void eng::ThrashTheCache::RenderImgui() {

	ImGui::Begin("Exercise 1");
	int sampleCount = 10;
	ImGui::InputInt("# Samples", &sampleCount, 1, 100);
	if (ImGui::Button("Thrash the cache")) MeasureInts(sampleCount);
	ImGui::End();

	ImGui::Begin("Exercise 2");
	int sampleCount2 = 10;
	ImGui::InputInt("# Samples", &sampleCount2, 1, 100);
	if (ImGui::Button("Thrash the cache with GameObject3D")) m_GameObject3dMeasurements = TakeMeasurement<gameObject3d>(sampleCount);
	if (ImGui::Button("Thrash the cache with GameObject3DAlt")) m_GameObject3dAltMeasurements = TakeMeasurement<gameObject3dAlt>(sampleCount);

	ImGui::End();

}

void eng::ThrashTheCache::MeasureInts(int sampleSize) {
	std::vector<int> arr(67'000'000);

	std::vector<int> measurements;
	for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2) {
		std::vector<int> subMeasurements{};
		for (int sampleId{}; sampleId < sampleSize; ++sampleId) {
			auto start = std::chrono::high_resolution_clock::now();
			for (int i{ 0 }; i < arr.size(); i += stepSize) {
				arr[i] *= 2;
			}
			subMeasurements.emplace_back(static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count()));
		};
		std::ranges::sort(subMeasurements);
		std::rotate(subMeasurements.begin(), subMeasurements.begin() + 1, subMeasurements.end());
		subMeasurements.pop_back();
		subMeasurements.pop_back();

		measurements.emplace_back(std::accumulate(subMeasurements.begin(), subMeasurements.end(), 0) / static_cast<int>(subMeasurements.size()));

		m_IntMeasurements = measurements;
	};
}