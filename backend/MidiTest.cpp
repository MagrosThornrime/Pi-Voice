#include <fmt/core.h>
#include <midi/Ports.hpp>

int main() {
	getchar();
	fmt::println("dupa1");
	midi::Ports::refresh();
	fmt::println("dupa2");
	midi::Ports::refresh();
	fmt::println("dupa3");
	midi::Ports::refresh();
	fmt::println("dupa4");
	midi::Ports::refresh();
	fmt::println("dupa5");
}