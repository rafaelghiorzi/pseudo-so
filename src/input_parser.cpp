#include "input_parser.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace {

    // Remove espaços, tabs e \r
    std::string trim(const std::string& text) {
        const std::string whitespace = " \t\r\n";
        size_t first = text.find_first_not_of(whitespace);

        if (first == std::string::npos) {
            return ""; // a string só tem espaços
        }

        size_t last = text.find_last_not_of(whitespace);
        return text.substr(first, last - first + 1);
    }

    std::vector<std::string> splitByComma(const std::string& line) {
        std::vector<std::string> fields;
        std::stringstream stream(line);
        std::string field;

        while (std::getline(stream, field, ',')) {
            fields.push_back(trim(field));
        }
        return fields;
    }

    // Converte para int, retorna mensagem de erro com a linha
    int toInt(const std::string& text, int lineNumber) {
        try {
            return std::stoi(text);
        } catch (const std::exception&) {
            throw std::runtime_error("linha " + std::to_string(lineNumber) +
                                     ": valor inteiro inválido '" + text + "'");
        }
    }

} // namespace

std::vector<Process> readProcesses(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Não foi possível abrir " + path);
    }

    std::vector<Process> processes;
    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;

        if (trim(line).empty()) {
            continue; // ignorar linhas em branco
        }

        std::vector<std::string> fields = splitByComma(line);
        if (fields.size() != 8) {
            throw std::runtime_error("Linha " + std::to_string(lineNumber) + " esperava 8 campos");
        }

        int pid        = static_cast<int>(processes.size());
        int arrival    = toInt(fields[0], lineNumber);
        int priority   = toInt(fields[1], lineNumber);
        int cpuTime    = toInt(fields[2], lineNumber);
        int workingSet = toInt(fields[3], lineNumber);

        DeviceRequest devices;
        devices.printer = toInt(fields[4], lineNumber);
        devices.scanner = toInt(fields[5], lineNumber);
        devices.modem   = toInt(fields[6], lineNumber);
        devices.sata    = toInt(fields[7], lineNumber);

        processes.push_back(Process(
            pid,
            arrival,
            priority,
            cpuTime,
            workingSet,
            devices
        ));
    }
            
    return processes;
}

void readReferenceStrings(const std::string& path, std::vector<Process>& processes) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Não foi possível abrir " + path);
    }

    std::string line;
    int lineNumber = 0;
    size_t processIndex = 0; // Quantas strings já foram atribuídas

    while (std::getline(file, line)) {
        ++lineNumber;

        if (trim(line).empty()) {
            continue; // ignora linhas em branco
        }

        if (processIndex >= processes.size()) {
            throw std::runtime_error("string.txt tem mais linhas que processos");
        }

        for (const std::string& page : splitByComma(line)) {
            processes[processIndex].referenceString.push_back(toInt(page, lineNumber));
        }
        ++processIndex;
    }

    if (processIndex != processes.size()) {
        throw std::runtime_error("string.txt tem menos linhas que processos");
    }    

}

FileSystemInput readFileSystemInput(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Não foi possível abrir " + path);
    }

    // TODO — dica: primeiro leia TODAS as linhas não vazias para um
    // std::vector<std::string>, e só depois interprete por índice:
    //   linhas[0]            -> total de blocos
    //   linhas[1]            -> n
    //   linhas[2 .. n+1]     -> segmentos
    //   linhas[n+2 .. fim]   -> operações
    // Isso evita uma máquina de estados dentro do while.
    //
    // Nas operações: o 4º campo só existe quando o código é 0.
    // Conferir a quantidade de campos de acordo com o código.
}