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

    // Lê o arquivo inteiro e devolve apenas as linhas não vazias, já sem espaços nas pontas
    std::vector<std::string> readNonEmptyLines(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Não foi possível abrir " + path);
        }

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            std::string cleanLine = trim(line);
            if (!cleanLine.empty()) {
                lines.push_back(cleanLine);
            }
        }
        return lines;
    }

    // "X, 0, 2" -> arquivo X, começando no bloco 0, ocupando 2 blocos
    DiskSegment parseSegment(const std::string& line, int lineNumber) {
        std::vector<std::string> fields = splitByComma(line);
        if (fields.size() != 3) {
            throw std::runtime_error("linha " + std::to_string(lineNumber) +
                                    ": segmento deve ter 3 campos");
        }

        DiskSegment segment;
        segment.fileName   = fields[0];
        segment.firstBlock = toInt(fields[1], lineNumber);
        segment.blockCount = toInt(fields[2], lineNumber);
        return segment;
    }

    // "0, 0, A, 5" (criar) ou "0, 1, X" (deletar)
    FileOperation parseOperation(const std::string& line, int lineNumber) {
        std::vector<std::string> fields = splitByComma(line);
        if (fields.size() < 3) {
            throw std::runtime_error("linha " + std::to_string(lineNumber) +
                                    ": operação deve ter ao menos 3 campos");
        }

        FileOperation operation;
        operation.pid      = toInt(fields[0], lineNumber);
        operation.fileName = fields[2];

        int code = toInt(fields[1], lineNumber);
        if (code == static_cast<int>(FileOperationType::Create)) {
            if (fields.size() != 4) {
                throw std::runtime_error("linha " + std::to_string(lineNumber) +
                                        ": operação de criar deve ter 4 campos");
            }
            operation.type       = FileOperationType::Create;
            operation.blockCount = toInt(fields[3], lineNumber);
        } else if (code == static_cast<int>(FileOperationType::Delete)) {
            if (fields.size() != 3) {
                throw std::runtime_error("linha " + std::to_string(lineNumber) +
                                        ": operação de deletar deve ter 3 campos");
            }
            operation.type = FileOperationType::Delete;
        } else {
            throw std::runtime_error("linha " + std::to_string(lineNumber) +
                                    ": código de operação inválido " + std::to_string(code));
        }
        return operation;
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
    std::vector<std::string> lines = readNonEmptyLines(path);
    if (lines.size() < 2) {
        throw std::runtime_error(path + ": faltam as linhas de total de blocos e de segmentos");
    }

    FileSystemInput input;
    input.totalBlocks = toInt(lines[0], 1);

    int segmentCount = toInt(lines[1], 2);
    if (segmentCount < 0) {
        throw std::runtime_error(path + ": quantidade de segmentos negativa");
    }

    size_t firstOperationLine = static_cast<size_t>(segmentCount) + 2;
    if (lines.size() < firstOperationLine) {
        throw std::runtime_error(path + ": faltam linhas de segmentos");
    }

    for (size_t i = 2; i < firstOperationLine; ++i) {
        input.initialSegments.push_back(parseSegment(lines[i], static_cast<int>(i) + 1));
    }
    for (size_t i = firstOperationLine; i < lines.size(); ++i) {
        input.operations.push_back(parseOperation(lines[i], static_cast<int>(i) + 1));
    }
    return input;
}