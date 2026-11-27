#include "file_system.hpp"

#include <stdexcept>

FileSystem::FileSystem(int totalBlocks, const std::vector<DiskSegment>& initialSegments) {

    if (totalBlocks <= 0) throw std::runtime_error("Quantidade inválida de blocos totais!");

    blocks.assign(totalBlocks, "");

    for (const DiskSegment& segment : initialSegments) { 
        const int endBlock = segment.firstBlock + segment.blockCount;

        if (segment.firstBlock < 0 || segment.blockCount <= 0 || endBlock > totalBlocks) {
            throw std::runtime_error("segmento " + segment.fileName + " fora dos limites do disco");
        }

        if (files.find(segment.fileName) != files.end()) { 
            throw std::runtime_error("arquivo " + segment.fileName + " aparece mais de uma vez");
        }

        for (int i = segment.firstBlock; i < endBlock; ++i) {
            if (!blocks[i].empty()) {
                throw std::runtime_error(
                    "Segmento " + segment.fileName + "sobrescreve " + blocks[i]
                );
            }

            // Finalmente depois de todas as checagens, coloca o bloco pela porta de entrada
            placeFile(segment.fileName, segment.firstBlock, segment.blockCount, NO_ONWER);
        }
    }
}

OperationResult FileSystem::execute(const FileOperation& operation,
                                    const std::vector<Process>& processes) {
    
    if (operation.pid < 0 || static_cast<size_t>(operation.pid) >= processes.size()) {
        throw std::runtime_error("Pid inválido: " + std::to_string(operation.pid));
    }

    if (operation.type == FileOperationType::Create) {
        FileSystem::createFile(operation);
    }

    if (operation.type == FileOperationType::Delete) {
        FileSystem::deleteFile(operation, processes[operation.pid]);
    }
}

OperationResult FileSystem::createFile(const FileOperation& operation) {
    // TODO, nesta ordem:
    // 1. já existe arquivo com esse nome? -> falha (decisão sua, documente)
    // 2. quantidade de blocos <= 0? -> falha
    // 3. início = findFirstFit(...)
    //    NOT_FOUND -> "O processo X não pode criar o arquivo Y (falta de espaço)."
    // 4. placeFile(..., operation.pid)
    //    -> "O processo X criou o arquivo Y (blocos ...)."
}

OperationResult FileSystem::deleteFile(const FileOperation& operation, const Process& process) {
    // TODO:
    // 1. procurar no map; não achou ->
    //    "O processo X não pode deletar o arquivo Y porque ele não existe."
    // 2. não é tempo real E o dono não é ele -> falha de permissão
    // 3. limpar os blocos do arquivo (voltam a "") e apagar do map
    //    -> "O processo X deletou o arquivo Y."
}

int FileSystem::findFirstFit(int blockCount) const {
    // TODO: percorrer os blocos do 0 em diante contando livres CONSECUTIVOS.
    //   bloco livre  -> se for o primeiro da sequência, anota o início; conta++
    //                   chegou em blockCount? retorna o início
    //   bloco ocupado -> zera a contagem
    // fim do disco sem achar -> NOT_FOUND
}

void FileSystem::placeFile(const std::string& name, int firstBlock, int blockCount, int ownerPid) {
    for (int i = firstBlock; i < firstBlock + blockCount; ++i) {
        blocks[i] = name;
    }    

    FileEntry entry;
    entry.firstBlock = firstBlock;
    entry.blockCount = blockCount;
    entry.ownerPid   = ownerPid;
    files[name]      = entry;
}

std::string FileSystem::formatBlockList(int firstBlock, int blockCount) {
    // TODO: 1 bloco -> "bloco 4"
    //       2       -> "blocos 8 e 9"
    //       3+      -> "blocos 0, 1 e 2"
}

std::string FileSystem::diskMap() const {
    // TODO: juntar os blocos separados por espaço; bloco vazio vira "0"
}