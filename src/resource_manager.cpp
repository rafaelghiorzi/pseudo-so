#include "resource_manager.hpp"

#include <stdexcept>
#include <string>

bool ResourceManager::hasRequestedDevices(const DeviceRequest& devices) {
    // TODO:
    // Retornar true se qualquer campo de devices (scanner, printer, modem, sata) for != 0
}

void ResourceManager::validateRequest(const Process& process) {
    // TODO, nesta ordem:
    // 1. scanner fora de [0, 1] -> lançar std::runtime_error com PID e valor inválido
    // 2. modem fora de [0, 1]   -> lançar std::runtime_error com PID e valor inválido
    // 3. printer fora de [0, PRINTER_COUNT] -> lançar std::runtime_error
    // 4. sata fora de [0, SATA_COUNT]       -> lançar std::runtime_error
    // 5. process.isRealTime() && hasRequestedDevices(process.devices)
    //    -> definir e documentar decisão (ex.: lançar std::runtime_error, pois
    //       processos de tempo real não podem requisitar recursos de E/S)
}

bool ResourceManager::canAllocate(const Process& process) const {
    // TODO, nesta ordem:
    // 1. chamar validateRequest(process)
    // 2. se process.isRealTime(), retornar true (não precisa de E/S)
    // 3. checar cada recurso pedido:
    //    - scanner == 1 && scannerOwner != FREE -> false
    //    - modem == 1   && modemOwner != FREE   -> false
    //    - printer > 0  && printerOwners[printer - 1] != FREE -> false
    //    - sata > 0     && sataOwners[sata - 1] != FREE       -> false
    // 4. se todos os recursos pedidos estiverem livres, retornar true
}

bool ResourceManager::allocate(const Process& process) {
    // TODO, nesta ordem:
    // 1. se !canAllocate(process), retornar false (não altera nada)
    // 2. se process.isRealTime(), retornar true
    // 3. marcar o dono (process.pid) em cada recurso pedido:
    //    - scanner == 1 -> scannerOwner = process.pid
    //    - modem == 1   -> modemOwner = process.pid
    //    - printer > 0  -> printerOwners[printer - 1] = process.pid
    //    - sata > 0     -> sataOwners[sata - 1] = process.pid
    // 4. retornar true
}

void ResourceManager::release(const Process& process) {
    // TODO:
    // Percorrer todos os recursos e voltar para FREE os que pertencem a process.pid:
    // - se scannerOwner == process.pid -> scannerOwner = FREE
    // - se modemOwner   == process.pid -> modemOwner = FREE
    // - para cada slot em printerOwners: se slot == process.pid -> slot = FREE
    // - para cada slot em sataOwners:    se slot == process.pid -> slot = FREE
}
