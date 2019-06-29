#include "log.h"
#include "subsystem.h"

int main(int argc, char ** argv)
{
	if (argc != 2) throw std::runtime_error{ "Invalid number of arguments." };

	std::ofstream log_file{ "nes-emulator.log" };

	platform::default_log.set_level(platform::log::Info);
	platform::default_log.set_stream(log_file);

	platform::subsystem{ argv[1] };

	return 0;
}
