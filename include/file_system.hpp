#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include <vector>

struct DiskSegment {
    // TODO: nome do arquivo, primeiro bloco, quantidade de blocos
};

enum class FileOperationType {
    // TODO: dois valores, dá pra fixar o número de cada um: Create = 0, Delete = 1
};

struct FileOperation {
    // TODO: PID, tipo, nome do arquivo, quantidade de blocos (só para criar)
};

struct FileSystemInput {
    // TODO: total de blocos, segmentos iniciais, operações
};

// (Definir a classe do filesystem aqui depois)

#endif // FILESYSTEM_HPP