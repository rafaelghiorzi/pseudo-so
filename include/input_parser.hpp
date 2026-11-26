#ifndef INPUT_PARSER_HPP
#define INPUT_PARSER_HPP

#include <string>
#include <vector>

#include "file_system.hpp"
#include "process.hpp"

// Lê o arquivo processes.txt e cada linha não vazia é um processo e o PID é a ordem das linhas
// Lança um runtime error se o arquivo não abrir ou tiver linha malformada
std::vector<Process> readProcess(const std::string& path);

// Lê string.txt e preenche referenceString de cada processo (linha i -> PID i)
void readReferenceStrings(const std::string& path, std::vector<Process>& processes);

// Lê files.txt e preenche tamanho do disco, segmentos iniciais e operações
FileSystemInput readFileSystemInput(const std::string& path);

#endif // INPUT_PARSER_HPP