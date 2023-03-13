#include <fstream>
#include <iostream>
#include <unordered_map>
#include <cctype>

struct buffer_t {
	unsigned char* data = nullptr;
	size_t length = 0;
};

buffer_t read();
void write(const char* buffer);
void convert(buffer_t buffer);

int main(int argc, char** argv)
{
	auto [buffer, length] = read();
	if (buffer)
	{
		convert({ buffer, length });
		write(reinterpret_cast<char*>(buffer));
		if (buffer)
			free(reinterpret_cast<void*>(buffer));
	}
	else {
		if (!length)
		{
			std::cout << "The file is empty\n";
		}
	}

	return 0;
}

buffer_t read()
{
	std::ifstream fin;
	fin.exceptions(std::ios::failbit | std::ios::badbit);
	unsigned char* buffer = nullptr;
	try {
		fin.open("input.txt");
		fin.seekg(0, std::ios::end);
		size_t length = fin.tellg();
		if (length)
		{
			fin.seekg(0, std::ios::beg);
			buffer = reinterpret_cast<unsigned char*>(malloc(length + 1));
			if (buffer)
			{
				fin.read(reinterpret_cast<char*>(buffer), length);
				buffer[length] = 0;
				return { buffer, length }; // normal string
			}
			else {
				std::cerr << "Bad allocation error\n";
				return { nullptr, ~size_t(0) }; // error
			}
		}
		return {}; // empty file
	}
	catch (const std::ios::failure& e)
	{
		std::cerr << "Exception opening/reading/closing file\n";
	}
	if (buffer)
		free(reinterpret_cast<void*>(buffer));
	return { nullptr, ~size_t(0) }; // error
}
void write(const char* buffer)
{
	std::ofstream fout;
	fout.exceptions(std::ios::failbit | std::ios::badbit);
	try {
		fout.open("output.txt");
		fout << buffer;
	}
	catch (const std::ios::failure& e)
	{
		std::cerr << "Exception opening/writing/closing file\n";
	}
}
void convert(buffer_t buffer)
{
	std::unordered_map<int, unsigned char*> firstMetPositions;
	for (size_t index = 0; index < buffer.length; index++)
	{
		auto& character = buffer.data[index];
		auto ins = firstMetPositions.emplace(tolower(character), &character);
		if (ins.second)
		{
			const_cast<unsigned char&>(character) = '(';
		}
		else
		{
			*ins.first->second = ')';
			character = ')';
		}
	}
}