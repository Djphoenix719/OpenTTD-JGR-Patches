/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file test_main.cpp Entry point for all the unit tests. */

#include "../stdafx.h"

#include <stdarg.h>
#include <stdio.h>

#define CATCH_CONFIG_MAIN
#define DO_NOT_USE_WMAIN
#include "../3rdparty/catch2/catch.hpp"

void CDECL error(const char *s, ...)
{
	va_list va;
	char buffer[1024];

	va_start(va, s);
	vsnprintf(buffer, 1024, s, va);
	va_end(va);

	CATCH_RUNTIME_ERROR(buffer);
}
