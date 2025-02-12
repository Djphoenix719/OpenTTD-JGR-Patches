/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file common.hpp Common functionality for all blitter implementations. */

#ifndef BLITTER_COMMON_HPP
#define BLITTER_COMMON_HPP

#include "base.hpp"
#include "../core/math_func.hpp"

#include <utility>
#include <wchar.h>

#ifdef __APPLE__
#include <string.h>
#endif

/** Cached black value. */
static const constexpr Colour _black_colour(0, 0, 0);

template <typename SetPixelT>
void Blitter::DrawLineGeneric(int x1, int y1, int x2, int y2, int screen_width, int screen_height, int width, int dash, SetPixelT set_pixel)
{
	int dy;
	int dx;
	int stepx;
	int stepy;

	dy = (y2 - y1) * 2;
	if (dy < 0) {
		dy = -dy;
		stepy = -1;
	} else {
		stepy = 1;
	}

	dx = (x2 - x1) * 2;
	if (dx < 0) {
		dx = -dx;
		stepx = -1;
	} else {
		stepx = 1;
	}

	if (dx == 0 && dy == 0) {
		/* The algorithm below cannot handle this special case; make it work at least for line width 1 */
		if (x1 >= 0 && x1 < screen_width && y1 >= 0 && y1 < screen_height) set_pixel(x1, y1);
		return;
	}

	int frac_diff = width * std::max(dx, dy);
	if (width > 1) {
		/* compute frac_diff = width * sqrt(dx*dx + dy*dy)
		 * Start interval:
		 *    max(dx, dy) <= sqrt(dx*dx + dy*dy) <= sqrt(2) * max(dx, dy) <= 3/2 * max(dx, dy) */
		int64 frac_sq = ((int64) width) * ((int64) width) * (((int64) dx) * ((int64) dx) + ((int64) dy) * ((int64) dy));
		int frac_max = 3 * frac_diff / 2;
		while (frac_diff < frac_max) {
			int frac_test = (frac_diff + frac_max) / 2;
			if (((int64) frac_test) * ((int64) frac_test) < frac_sq) {
				frac_diff = frac_test + 1;
			} else {
				frac_max = frac_test - 1;
			}
		}
	}

	int gap = dash;
	if (dash == 0) dash = 1;
	int dash_count = 0;
	if (dx > dy) {
		if (stepx < 0) {
			std::swap(x1, x2);
			std::swap(y1, y2);
			stepy = -stepy;
		}
		if (x2 < 0 || x1 >= screen_width) return;

		int y_low     = y1;
		int y_high    = y1;
		int frac_low  = dy - frac_diff / 2;
		int frac_high = dy + frac_diff / 2;

		while (frac_low < -(dx / 2)) {
			frac_low += dx;
			y_low -= stepy;
		}
		while (frac_high >= dx / 2) {
			frac_high -= dx;
			y_high += stepy;
		}

		if (x1 < 0) {
			dash_count = (-x1) % (dash + gap);
			auto adjust_frac = [&](int64 frac, int &y_bound) -> int {
				frac -= ((int64) dy) * ((int64) x1);
				if (frac >= 0) {
					int quotient = frac / dx;
					int remainder = frac % dx;
					y_bound += (1 + quotient) * stepy;
					frac = remainder - dx;
				}
				return frac;
			};
			frac_low = adjust_frac(frac_low, y_low);
			frac_high = adjust_frac(frac_high, y_high);
			x1 = 0;
		}
		x2++;
		if (x2 > screen_width) {
			x2 = screen_width;
		}

		while (x1 != x2) {
			if (dash_count < dash) {
				for (int y = y_low; y != y_high; y += stepy) {
					if (y >= 0 && y < screen_height) set_pixel(x1, y);
				}
			}
			if (frac_low >= 0) {
				y_low += stepy;
				frac_low -= dx;
			}
			if (frac_high >= 0) {
				y_high += stepy;
				frac_high -= dx;
			}
			x1++;
			frac_low += dy;
			frac_high += dy;
			if (++dash_count >= dash + gap) dash_count = 0;
		}
	} else {
		if (stepy < 0) {
			std::swap(x1, x2);
			std::swap(y1, y2);
			stepx = -stepx;
		}
		if (y2 < 0 || y1 >= screen_height) return;

		int x_low     = x1;
		int x_high    = x1;
		int frac_low  = dx - frac_diff / 2;
		int frac_high = dx + frac_diff / 2;

		while (frac_low < -(dy / 2)) {
			frac_low += dy;
			x_low -= stepx;
		}
		while (frac_high >= dy / 2) {
			frac_high -= dy;
			x_high += stepx;
		}

		if (y1 < 0) {
			dash_count = (-y1) % (dash + gap);
			auto adjust_frac = [&](int64 frac, int &x_bound) -> int {
				frac -= ((int64) dx) * ((int64) y1);
				if (frac >= 0) {
					int quotient = frac / dy;
					int remainder = frac % dy;
					x_bound += (1 + quotient) * stepx;
					frac = remainder - dy;
				}
				return frac;
			};
			frac_low = adjust_frac(frac_low, x_low);
			frac_high = adjust_frac(frac_high, x_high);
			y1 = 0;
		}
		y2++;
		if (y2 > screen_height) {
			y2 = screen_height;
		}

		while (y1 != y2) {
			if (dash_count < dash) {
				for (int x = x_low; x != x_high; x += stepx) {
					if (x >= 0 && x < screen_width) set_pixel(x, y1);
				}
			}
			if (frac_low >= 0) {
				x_low += stepx;
				frac_low -= dy;
			}
			if (frac_high >= 0) {
				x_high += stepx;
				frac_high -= dy;
			}
			y1++;
			frac_low += dx;
			frac_high += dx;
			if (++dash_count >= dash + gap) dash_count = 0;
		}
	}
}

inline void memset_uint32(uint32 *s, uint32 c, size_t n)
{
#ifdef __APPLE__
	memset_pattern4(static_cast<void *>(s), static_cast<const void *>(&c), n * sizeof(uint32));
#else
	if constexpr (sizeof(wchar_t) == sizeof(uint32)) {
		wmemset((wchar_t *)s, (wchar_t)c, n);
	} else {
		for (; n > 0; n--) {
			*s = c;
			s++;
		}
	}
#endif
}

inline void memset_colour(Colour *s, Colour c, size_t n)
{
	memset_uint32((uint32 *)s, c.data, n);
}

#endif /* BLITTER_COMMON_HPP */
