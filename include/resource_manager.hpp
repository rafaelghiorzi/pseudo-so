#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "process.hpp"

#include <array>

class ResourceManager {
public:
    ResourceManager() = default;

    // Verifica se todos os dispositivos pedidos pelo processo estão livres
    // Lança std::runtime_error se a requisição for inválida
    bool canAllocate(const Process& process) const;

    // Tenta alocar todos os dispositivos pedidos pelo processo
    // Retorna true se conseguiu alocar tudo (ou se nada foi pedido)
    // retorna false sem alterar o estado caso algum recurso esteja ocupado
    bool allocate(const Process& process);

    // Libera todos os dispositivos atualmente alocados para o processo.
    void release(const Process& process);

private:
    static constexpr int FREE          = -1; // dispositivo livre
    static constexpr int PRINTER_COUNT = 2;
    static constexpr int SATA_COUNT    = 2;

    // PID do processo dono de cada dispositivo
    int scannerOwner = FREE;
    int modemOwner   = FREE;
    std::array<int, PRINTER_COUNT> printerOwners = {FREE, FREE};
    std::array<int, SATA_COUNT>    sataOwners    = {FREE, FREE};

    // Valida limites dos campos de DeviceRequest e regra de tempo real
    static void validateRequest(const Process& process);
    static bool hasRequestedDevices(const DeviceRequest& devices);
};

#endif // RESOURCE_MANAGER_HPP
