#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "process.hpp"

#include <string>
#include <vector>
#include <map>

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


// Resultado de uma operação, pronto para o despachante
struct OperationResult {
    bool success = false;
    std::string message;
};


// Disco simulado. Utiliza alocação contígua first-fit
// Processos de tempo real podem criar e deletar qualquer arquivo
// Processos de usuário podem criar, mas deletar apenas os próprios arquivos
class FileSystem {
public:
    // Monta o disco com os arquivos que já existem no início
    // Lança std::runtime_error se algum segmento sair do disco ou sobrepor outro
    FileSystem(int totalBlocks, const std::vector<DiskSegment>& initialSegments);

    // Executa uma operação de criar ou deletar e devolve o resultado
    OperationResult execute(const FileOperation& operation, const std::vector<Process>& processes);

    // Mapa de ocupação do disco: nome do arquivo em cada bloco, "0" nos blocos livres
    std::string diskMap() const;


private:
    // Onde um arquivo está e quem o criou
    struct FileEntry {
        int firstBlock = 0;
        int blockCount = 0;
        int ownerPid   = 0;
    };

    static constexpr int NO_ONWER  = -1; // arquivos iniciais no disco
    static constexpr int NOT_FOUND = -1; // first-fit sem espaço

    std::vector<std::string> blocks;         // nome do arquivo em cada bloco ("" = livre)
    std::map<std::string, FileEntry> files;  // none -> localização e dono

    // Únicas funções que alteram blocks e files
    void placeFile(const std::string& name, int firstBlock, int blockCount, int ownerPid);
    
    OperationResult deleteFile(const FileOperation& operation, const Process& process);
    OperationResult createFile(const FileOperation& operation);
    
    int findFirstFit(int blockCount) const;
    static std::string formatBlockList(int firstBlock, int blockCount);
};

#endif // FILESYSTEM_HPP