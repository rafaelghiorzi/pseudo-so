#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include <vector>

// Arquivo já gravado no disco antes da simulação
struct DiskSegment {
    std::string fileName;
    int firstBlock  = 0;
    int blockCount = 0;
};

// Tipo de operação
enum class FileOperationType {
    Create = 0,
    Delete = 1,
};

// Operação a ser executada pelo sistema de arquivos
struct FileOperation {
    int pid = 0;
    FileOperationType type = FileOperationType::Create;
    std::string fileName;
    int blockCount = 0; // usado só em create
};


// Conteúdo completo de files.txt, devidamente interpretado
struct FileSystemInput {
    int totalBlocks = 0;
    std::vector<DiskSegment> initialSegments;
    std::vector<FileOperation> operations;
};

// (Definir a classe do filesystem aqui depois)

#endif // FILESYSTEM_HPP