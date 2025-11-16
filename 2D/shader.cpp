// Include standard headers
#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <sstream>

using namespace std;


#include "dependente\glew\glew.h"

namespace ShaderUtils
{
	void readShaderFile(const char* shaderFilePath, std::string& output)
	{
		std::ifstream file(shaderFilePath, std::ios::binary);

		if (!file)
		{
			throw std::runtime_error("Cannot open shader file : " + std::string(shaderFilePath) + ". Are you in the right directory?\n\n");
		}

		output.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	}

	GLuint compileShader(const char* shaderFilePath, GLenum shaderType)
	{
	
		GLuint shaderId = glCreateShader(shaderType);

	
		std::string shaderCode;
		readShaderFile(shaderFilePath, shaderCode);

		
		printf("Compiling shader : %s\n", shaderFilePath);
		char const* sourcePointer = shaderCode.c_str();
		glShaderSource(shaderId, 1, &sourcePointer, NULL);
		glCompileShader(shaderId);

		GLint compileSuccessful = GL_FALSE;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileSuccessful);

		int infoLogLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

		
		if (infoLogLength > 0)
		{
			std::vector<char> shaderErrorMessage(infoLogLength + 1);
			glGetShaderInfoLog(shaderId, infoLogLength, NULL, &shaderErrorMessage[0]);
			std::string log(&shaderErrorMessage[0]);

			std::string emptySpaces = "                   ";
			std::string glslErrorPrefix = "ERROR: ";


		
			size_t pos = 0;
			while ((pos = log.find(glslErrorPrefix, pos)) != std::string::npos)
			{
				log.erase(pos, glslErrorPrefix.length());
			}

		
			std::vector<std::string> lines;
			size_t start = 0;
			size_t end = log.find('\n');
			while (end != std::string::npos)
			{
				lines.push_back(log.substr(start, end - start));
				start = end + 1;
				end = log.find('\n', start);
			}

			if (start < log.size())
			{
				lines.push_back(log.substr(start));
			}

			
			std::string formattedLog = emptySpaces + "^^^\n" + emptySpaces + "Compilation errors: \n";
			for (size_t i = 0; i < lines.size(); ++i)
			{
				std::string& line = lines[i];

				
				size_t colon1 = line.find(':');
				size_t colon2 = line.find(':', colon1 + 1);
				if (colon1 != std::string::npos && colon2 != std::string::npos) {
					std::string linenum = line.substr(colon1 + 1, colon2 - colon1 - 1);
					std::string msg = line.substr(colon2 + 1);
					formattedLog += emptySpaces + "- line " + linenum + ":" + msg + "\n";
				}
				
				else if (line.find("compilation errors") != std::string::npos)
				{
					formattedLog += emptySpaces + "Total of " + line + "\n\n";
				}
			}

			printf("%s", formattedLog.c_str());
		}

		if (compileSuccessful != GL_TRUE)
		{
			glDeleteShader(shaderId);
			shaderId = 0;
		}

		return shaderId;
	}

	void cleanup(GLuint programId, GLuint vertexShaderId, GLuint fragmentShaderId)
	{
		if (programId != 0)
		{
			glDetachShader(programId, vertexShaderId);
			glDetachShader(programId, fragmentShaderId);
		}

		if (vertexShaderId != 0)
		{
			glDeleteShader(vertexShaderId);
		}

		if (fragmentShaderId != 0)
		{
			glDeleteShader(fragmentShaderId);
		}
	}

	GLuint linkProgram(GLuint vertexShaderId, GLuint fragmentShaderId)
	{
	
		if (!vertexShaderId || !fragmentShaderId)
		{
			return 0;
		}

		printf("Linking program... ");
		GLuint programId = glCreateProgram();
		glAttachShader(programId, vertexShaderId);
		glAttachShader(programId, fragmentShaderId);
		glLinkProgram(programId);

		GLint linkSuccessful = GL_FALSE;
		glGetProgramiv(programId, GL_LINK_STATUS, &linkSuccessful);

		int infoLogLength = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			std::vector<char> ProgramErrorMessage(infoLogLength + 1);
			glGetProgramInfoLog(programId, infoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}

		if (linkSuccessful != GL_TRUE)
		{
			glDeleteProgram(programId);
			printf("program could not be linked.\n\n");
			return 0;
		}

		printf("program linked successfully.\n\n");

		
		cleanup(programId, vertexShaderId, fragmentShaderId);

		return programId;
	}
}

[[nodiscard]]
GLuint LoadShaders(const char* vertexFilePath, const char* fragmentFilePath)
{
	
	GLuint vertexShaderId = ShaderUtils::compileShader(vertexFilePath, GL_VERTEX_SHADER);
	GLuint fragmentShaderId = ShaderUtils::compileShader(fragmentFilePath, GL_FRAGMENT_SHADER);


	GLuint programId = ShaderUtils::linkProgram(vertexShaderId, fragmentShaderId);

	return programId;
}
