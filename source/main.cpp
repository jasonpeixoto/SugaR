/*
  SugaR, a UCI chess playing engine derived from Stockfish
  Copyright (C) 2004-2008 Tord Romstad (Glaurung author)
  Copyright (C) 2008-2015 Marco Costalba, Joona Kiiski, Tord Romstad
  Copyright (C) 2015-2017 Marco Costalba, Joona Kiiski, Gary Linscott, Tord Romstad

  SugaR is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  SugaR is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>


#include "bitboard.h"
#include "evaluate.h"
#include "position.h"
#include "search.h"
#include "thread.h"
#include "tt.h"
#include "uci.h"
#include "syzygy/tbprobe.h"
#include "tzbook.h"


#include <time.h>

#ifdef _WIN32
#include <windows.h>
#undef max
#undef min

#include "VersionHelpers.h"
#endif

namespace PSQT {
	void init();
}

int main(int argc, char* argv[]) {

	{
		const size_t time_length_const = 100;
		char time_local[time_length_const];
		memset(time_local, char(0), time_length_const);
		time_t result = time(NULL);
		tm tm_local;
		errno_t errno_local = localtime_s(&tm_local, &result);
		if (errno_local == 0)
		{
			errno_local = asctime_s(time_local, time_length_const, &tm_local);
			if (errno_local == 0)
			{
				std::cout << time_local;
			}
			else
			{
				assert(errno_local != 0);
			}
		}
		else
		{
			assert(errno_local != 0);
		}
	}

	std::cout << engine_info() << std::endl;

#ifdef _WIN32
	{
		SYSTEM_INFO siSysInfo;

		// Copy the hardware information to the SYSTEM_INFO structure. 

		GetSystemInfo(&siSysInfo);

		// Display the contents of the SYSTEM_INFO structure. 

		std::cout << "Hardware information: " << std::endl;
		std::cout << "  OEM ID: " << siSysInfo.dwOemId << std::endl;
		std::cout << "  Number of processors: " << siSysInfo.dwNumberOfProcessors << std::endl;
		std::cout << "  Page size: " << siSysInfo.dwPageSize << std::endl;
		std::cout << "  Processor type: " << siSysInfo.dwProcessorType << std::endl;
		std::cout << "  Minimum application address: " << siSysInfo.lpMinimumApplicationAddress << std::endl;
		std::cout << "  Maximum application address: " << siSysInfo.lpMaximumApplicationAddress << std::endl;
		std::cout << "  Active processor mask: " << siSysInfo.dwActiveProcessorMask << std::endl;


		{
			InitVersion();

			if (IsWindowsXPOrGreater())
			{
				if (IsWindowsXPSP1OrGreater())
				{
					if (IsWindowsXPSP2OrGreater())
					{
						if (IsWindowsXPSP3OrGreater())
						{
							if (IsWindowsVistaOrGreater())
							{
								if (IsWindowsVistaSP1OrGreater())
								{
									if (IsWindowsVistaSP2OrGreater())
									{
										if (IsWindows7OrGreater())
										{
											if (IsWindows7SP1OrGreater())
											{
												if (IsWindows8OrGreater())
												{
													if (IsWindows8Point1OrGreater())
													{
														if (IsWindows10OrGreater())
														{
															std::cout << "Windows 10 Or Greater" << std::endl;
														}
														else
														{
															std::cout << "Windows 8 Point 1 Or Greater" << std::endl;
														}
													}
													else
													{
														std::cout << "Windows 8 Or Greater" << std::endl;
													}
												}
												else
												{
													std::cout << "Windows 7 SP1 Or Greater" << std::endl;
												}
											}
											else
											{
												std::cout << "Windows7 Or Greater" << std::endl;
											}
										}
										else
										{
											std::cout << "Vista SP2 Or Greater" << std::endl;
										}
									}
									else
									{
										std::cout << "Vista SP1O rGreater" << std::endl;
									}
								}
								else
								{
									std::cout << "Vista Or Greater" << std::endl;
								}
							}
							else
							{
								std::cout << "XP SP3 Or Greater" << std::endl;
							}
						}
						else
						{
							std::cout << "XP SP2 Or Greater" << std::endl;
						}
					}
					else
					{
						std::cout << "XP SP1 Or Greater" << std::endl;
					}
				}
				else
				{
					std::cout << "XP Or Greater" << std::endl;
				}
			}

			if (IsWindowsServer())
			{
				std::cout << "Server" << std::endl;
			}
			else
			{
				std::cout << "Client" << std::endl;
			}
		}
	}
#endif

	UCI::init(Options);
	PSQT::init();
	Bitboards::init();
	Position::init();
	Bitbases::init();
	Eval::init();
	Search::init(Options["Clean Search"]);
	Pawns::init();
	Tablebases::init(Options["SyzygyPath"]);
	TT.resize(Options["Hash"]);
	Threads.init(Options["Threads"]);
	Search::clear(); // After threads are up
	tzbook.init(Options["BookPath"]);
	UCI::loop(argc, argv);

	Threads.exit();
	return 0;
}
