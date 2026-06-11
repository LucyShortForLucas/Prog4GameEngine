#pragma once

#include <SDL3//SDL.h>
#include <vector>
#include <bitset>
#include <glm/glm.hpp>

namespace eng {

class ColorMap {
public:
	//--- Ctor
	template <std::ranges::input_range R>
		requires std::convertible_to<std::ranges::range_value_t<R>, SDL_Color>
	explicit ColorMap(const R& range, std::size_t width)
		: m_Colors(std::ranges::begin(range), std::ranges::end(range)),
		m_Width{ width },
		m_Height{ m_Colors.size() / width } {}

	template <std::ranges::input_range R>
		requires std::convertible_to<std::ranges::range_value_t<R>, SDL_Color>
	explicit ColorMap(R&& range, std::size_t width)
		: m_Colors(std::make_move_iterator(std::ranges::begin(range)),
			std::make_move_iterator(std::ranges::end(range))),
			m_Width{width},
			m_Height{m_Colors.size()/width} {}

	//---- Api
	const std::size_t RowSize() const { return m_Width; } ;
	const std::size_t ColumnSize() const { return m_Height; };

	SDL_Color At(std::size_t x, std::size_t y) {
		return m_Colors.at(x + y * m_Width);
	}

private:
	const std::vector<SDL_Color> m_Colors;
	const std::size_t m_Width;
	const std::size_t m_Height;
};

template <std::size_t W, std::size_t H>
class BlackWhiteMap {
public:
	BlackWhiteMap(ColorMap& from) :
		m_Width{ from.RowSize()},
		m_Height{from.ColumnSize()},
		m_Bits{ [&from] {
			std::bitset<W* H> bits{};
			for (std::size_t y{}; y < from.ColumnSize(); ++y) {
				for (std::size_t x{}; x < from.RowSize(); ++x) {
					SDL_Color color{ from.At(x, y) };
					if (color.r != 0 || color.g != 0 || color.b != 0)
						bits.set(x+y*W);
				}
			}
			return bits;
		}()
	} {}

	std::size_t RowSize() { return m_Width; };
	std::size_t ColumnSize() { return m_Height; };

	bool At(std::size_t x, std::size_t y) {
		return m_Bits.test(x + y * m_Width);
	}

private:
	const std::size_t m_Width;
	const std::size_t m_Height;
	const std::bitset<W * H> m_Bits;
};

} // !eng