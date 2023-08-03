#include <iostream>
#include <fstream>
#include <array>
#include <vector>

struct Frame {
	float a{ 1 };
	float b{ 2 };
	float c{ 3 };
	int a1{ 123 };
	int a2{ 1234 };
	int a3{ 12345 };
};

struct Header {
	int32_t version{ 1 };
	int32_t dataSize{ 0 };

	std::array<int, 7> padding;

	Header() {
		padding.fill(0);
	}
};

int main(int argc, char* argv[])
{
	{
		// Populate data with some values

		Header header;

		std::vector<Frame> frames;

		for (size_t i = 0; i < 25; i++)
		{
			frames.push_back({ .a = i + 0.12f });
		}

		header.dataSize = static_cast<int32_t>(frames.size());

		// Clear
		{
			std::fstream file;
			file.open("test.bin", std::ios::out | std::ios::trunc);
			file.close();
		}

		// Write
		{
			std::fstream file;
			file.open("test.bin", std::ios::app | std::ios::binary);

			file.write(reinterpret_cast<char*>(&header), sizeof(header));
			file.write(reinterpret_cast<char*>(frames.data()), sizeof(Frame) * frames.size());
		}
	}

	{
		// Read

		std::ifstream file("test.bin", std::ios::out | std::ios::binary);

		Header header;
		std::vector<Frame> frames;

		file.read(reinterpret_cast<char*>(&header), sizeof(header));

		size_t dataSize = static_cast<size_t>(header.dataSize);

		std::cout << "size: " << dataSize << "\n";

		for (size_t i = 0; i < dataSize; i++)
		{
			frames.push_back({});
		}

		file.read(reinterpret_cast<char*>(frames.data()), sizeof(Frame) * dataSize);

		for (size_t i = 0; i < frames.size(); i++)
		{
			std::cout << i << ": " << frames[i].a << "\n";
		}
	}

}