#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <vector>

constexpr int REAL_TIME_PRIORITY = 0;
constexpr int LOWEST_USER_PRIORITY = 3;

// Estados que um processo pode admitir
enum class ProcessState {
    WaitingAdmission,
    Ready,
    Finished
};

// Dispositivos de E/S solicitados por um processo (4 últimas colunas de processes.txt).
// Impressora e SATA: 0 = nenhum, 1 ou 2 = qual unidade.
// Scanner e modem:   0 = não usa, 1 = usa (só existe uma unidade de cada).
struct DeviceRequest {
    int scanner = 0;
    int printer = 0;
    int modem   = 0;
    int sata    = 0;
};

struct Process {

    // Dados lidos na entrada
    int pid             = 0;
    int arrivalTime     = 0;
    int initialPriority = 0;
    int totalCpuTime    = 0;
    int workingSetSize  = 0;
    DeviceRequest devices;
    std::vector<int> referenceString; // prenchida depois com string.txt

    // Estados da simulação, que mudam durante a execução
    ProcessState state       = ProcessState::WaitingAdmission;
    int priority             = 0; // prioridade atual (feedback e aging mudam esse)
    int remainingCpuTime     = 0;
    int executedInstructions = 0;
    int queueEntryTick       = 0; // tick que entrou na fila atual (usado no aging)
    
    // Resultados
    int pageFaultCount;


    Process(int processId, int arrival, int basePriority, int cpuTime,
            int workingSet, const DeviceRequest& requestedDevices)
        : pid(processId),
          arrivalTime(arrival),
          initialPriority(basePriority),
          totalCpuTime(cpuTime),
          workingSetSize(workingSet),
          devices(requestedDevices),
          priority(basePriority),
          remainingCpuTime(cpuTime) {}

    bool isRealTime() const {
        return initialPriority == REAL_TIME_PRIORITY;
    }

    bool isFinished() const {
        return remainingCpuTime == 0;
    }
};


#endif  // PROCESS_HPP