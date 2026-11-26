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
    // TODO:
    // 1. abrir o arquivo e conferir
    // 2. para cada linha não vazia:
    //    - quebrar em campos e conferir que são 8
    //    - converter cada campo
    //    - montar o DeviceRequest (atenção à ORDEM das colunas no PDF)
    //    - construir o Process com o PID = quantidade de processos já lidos
    // 3. retornar o vetor
}

void readReferenceStrings(const std::string& path, std::vector<Process>& processes) {
    // TODO:
    // 1. abrir o arquivo e conferir
    // 2. para cada linha não vazia, converter todos os campos e
    //    atribuir ao referenceString do processo correspondente
    // 3. decidir: e se houver menos linhas que processos? E mais?
}

FileSystemInput readFileSystemInput(const std::string& path) {
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