#include "ThrashTheCache.h"
#include <imgui.h>
#include <imgui_plot.h>

void eng::ThrashTheCache::RenderImgui() {

	ImGui::Begin("Exercise 1");
	int sampleCount = 10;
	ImGui::InputInt("# Samples", &sampleCount, 1, 100);
	if (ImGui::Button("Thrash the cache")) MeasureInts(sampleCount);

	if (m_IntMeasurements.size() > 0) {
		ImGui::PlotConfig conf{};

		conf.values.ys = m_IntMeasurements.data();
		conf.values.count = static_cast<int>(m_IntMeasurements.size());
		conf.scale.min = 0;
		conf.scale.max = 200;
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.2f, y=%.2f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 2.f;
		ImGui::Plot("Int measurements", conf);
	}

	ImGui::End();

	ImGui::Begin("Exercise 2");
	int sampleCount2 = 10;
	ImGui::InputInt("# Samples", &sampleCount2, 1, 100);
	if (ImGui::Button("Thrash the cache with GameObject3D")) m_GameObject3dMeasurements = TakeMeasurement<gameObject3d>(sampleCount);

	if (m_GameObject3dMeasurements.size() > 0) {
		ImGui::PlotConfig conf{};

		conf.values.ys = m_GameObject3dMeasurements.data();
		conf.values.count = static_cast<int>(m_GameObject3dMeasurements.size());
		conf.scale.min = 0;
		conf.scale.max = 300;
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.2f, y=%.2f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 2.f;
		ImGui::Plot("GameObject3d measurements", conf);
	}

	if (ImGui::Button("Thrash the cache with GameObject3DAlt")) m_GameObject3dAltMeasurements = TakeMeasurement<gameObject3dAlt>(sampleCount);

	if (m_GameObject3dAltMeasurements.size() > 0) {
		ImGui::PlotConfig conf{};

		conf.values.ys = m_GameObject3dAltMeasurements.data();
		conf.values.count = static_cast<int>(m_GameObject3dAltMeasurements.size());
		conf.scale.min = 0;
		conf.scale.max = 300;
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.2f, y=%.2f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 2.f;
		ImGui::Plot("GameObject3dAlt measurements", conf);
	}

	ImGui::End();

}

void eng::ThrashTheCache::MeasureInts(int sampleSize) {
	std::vector<int> arr(67'000'000);

	std::vector<float> measurements;
	for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2) {
		std::vector<float> subMeasurements{};
		for (int sampleId{}; sampleId < sampleSize; ++sampleId) {
			auto start = std::chrono::high_resolution_clock::now();
			for (int i{ 0 }; i < arr.size(); i += stepSize) {
				arr[i] *= 2;
			}
			subMeasurements.emplace_back(static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count()));
		};
		std::ranges::sort(subMeasurements);
		std::rotate(subMeasurements.begin(), subMeasurements.begin() + 1, subMeasurements.end());
		subMeasurements.pop_back();
		subMeasurements.pop_back();

		measurements.emplace_back(std::accumulate(subMeasurements.begin(), subMeasurements.end(), 0.f) / static_cast<float>(subMeasurements.size()));

		m_IntMeasurements = measurements;
	};
}