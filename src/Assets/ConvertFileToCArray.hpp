#pragma once
#include <fstream>
#include <vector>
#include <iomanip>
#include <cassert>

// This function is only for development purposes. It converts a file to a C array.
void ConvertFileToCArray(const char* inputPath, const char* outputPath, const char* arrayName)
{
	std::ifstream file(inputPath, std::ios::binary);
	assert(file.is_open() && "File not found.");
	std::vector<char> input((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	std::ofstream output(outputPath);
	output << "// This file was generated by ConvertFileToCArray.hpp\n\n";
	output << "static const int " << arrayName << "_size = " << input.size() << ";\n";
	output << "static const unsigned char " << arrayName << "[] = {\n";

	int byteIdx = 0;
	while (byteIdx < input.size())
	{
		// Begin line
		output << "\t\"";

		const int lineLength = 16;
		for (int i = 0; i < lineLength; i++)
		{
			if (byteIdx >= input.size())
				break;

			// Add byte
			output << "\\x" << std::setfill('0') << std::setw(2) << std::hex << (int)(unsigned char)input[byteIdx];

			// Next byte
			byteIdx++;
		}

		// End line
		output << "\"\n";
	}

	output << "};\n";

}