#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <cmath>
#include <stack>

class Pipe {
public:
    int id;
    std::string name;
    double length;
    int diameter;
    bool underRepair;
    bool inUse;
    
    Pipe(int id = 0, std::string name = "", double length = 0, 
         int diameter = 0, bool repair = false)
        : id(id), name(name), length(length), 
          diameter(diameter), underRepair(repair), inUse(false) {}
    
    bool isAvailable() const {
        return !underRepair && !inUse;
    }
    
    double getCapacity() const {
        if (underRepair) return 0.0;
        
        static const std::map<int, double> capacityTable = {
            {500, 100.0},
            {700, 300.0},
            {1000, 700.0},
            {1400, 1200.0}
        };
        
        auto it = capacityTable.find(diameter);
        if (it != capacityTable.end()) {
            return it->second;
        }
        return 0.0;
    }
    
    double getWeight() const {
        if (underRepair) return std::numeric_limits<double>::infinity();
        return length;
    }
};

class CompressorStation {
public:
    int id;
    std::string name;
    int totalWorkshops;
    int workingWorkshops;
    int stationClass;
    
    CompressorStation(int id = 0, std::string name = "", 
                     int total = 0, int working = 0, int cls = 0)
        : id(id), name(name), totalWorkshops(total), 
          workingWorkshops(working), stationClass(cls) {}
};

class NetworkConnection {
public:
    int pipeId;
    int fromStationId;
    int toStationId;
    
    NetworkConnection(int pipeId = 0, int from = 0, int to = 0)
        : pipeId(pipeId), fromStationId(from), toStationId(to) {}
};

int nextPipeId = 1;
int nextStationId = 1;
std::vector<Pipe> pipes;
std::vector<CompressorStation> stations;
std::vector<NetworkConnection> connections;

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void logAction(const std::string& action) {
    std::ofstream logFile("log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << action << std::endl;
    }
    logFile.close();
}

Pipe* findAvailablePipe(int diameter) {
    for (auto& pipe : pipes) {
        if (pipe.diameter == diameter && pipe.isAvailable()) {
            return &pipe;
        }
    }
    return nullptr;
}

void addPipe() {
    Pipe newPipe;
    newPipe.id = nextPipeId++;
    
    std::cout << "Vvedite nazvanie truby: ";
    std::getline(std::cin, newPipe.name);
    
    std::cout << "Vvedite dlinu truby (km): ";
    while (!(std::cin >> newPipe.length) || newPipe.length <= 0) {
        std::cout << "Nevernyj vvod. Vvedite polozhitelnoe chislo: ";
        clearInputBuffer();
    }
    
    std::cout << "Vvedite diametr truby (mm): ";
    std::cout << "Dostupnye diametry: 500, 700, 1000, 1400" << std::endl;
    while (!(std::cin >> newPipe.diameter) || 
           (newPipe.diameter != 500 && newPipe.diameter != 700 && 
            newPipe.diameter != 1000 && newPipe.diameter != 1400)) {
        std::cout << "Nevernyj vvod. Vvedite odin iz dostupnyh diametrov (500, 700, 1000, 1400): ";
        clearInputBuffer();
    }
    
    newPipe.underRepair = false;
    newPipe.inUse = false;
    clearInputBuffer();
    
    pipes.push_back(newPipe);
    logAction("Dobavlena truba ID: " + std::to_string(newPipe.id));
    std::cout << "Truba uspeshno dobavlena! ID: " << newPipe.id << std::endl;
}

void addCompressorStation() {
    CompressorStation newStation;
    newStation.id = nextStationId++;
    
    std::cout << "Vvedite nazvanie kompressornoj stancii: ";
    std::getline(std::cin, newStation.name);
    
    std::cout << "Vvedite obshhee kolichestvo cehov: ";
    while (!(std::cin >> newStation.totalWorkshops) || newStation.totalWorkshops <= 0) {
        std::cout << "Nevernyj vvod. Vvedite polozhitelnoe celoe chislo: ";
        clearInputBuffer();
    }
    
    std::cout << "Vvedite kolichestvo rabotayushhih cehov: ";
    while (!(std::cin >> newStation.workingWorkshops) || newStation.workingWorkshops < 0 || newStation.workingWorkshops > newStation.totalWorkshops) {
        std::cout << "Nevernyj vvod. Vvedite chislo ot 0 do " << newStation.totalWorkshops << ": ";
        clearInputBuffer();
    }
    
    std::cout << "Vvedite klass stancii: ";
    while (!(std::cin >> newStation.stationClass) || newStation.stationClass <= 0) {
        std::cout << "Nevernyj vvod. Vvedite polozhitelnoe celoe chislo: ";
        clearInputBuffer();
    }
    clearInputBuffer();
    
    stations.push_back(newStation);
    logAction("Dobavlena KS ID: " + std::to_string(newStation.id));
    std::cout << "Kompressornaja stancija uspeshno dobavlena! ID: " << newStation.id << std::endl;
}

void displayAllObjects() {
    std::cout << "\n=== TRUBY ===" << std::endl;
    if (pipes.empty()) {
        std::cout << "Net trub." << std::endl;
    } else {
        for (const auto& pipe : pipes) {
            std::cout << "ID: " << pipe.id << ", Nazvanie: " << pipe.name 
                      << ", Dlina: " << pipe.length << " km"
                      << ", Diametr: " << pipe.diameter << " mm"
                      << ", V remonte: " << (pipe.underRepair ? "Da" : "Net")
                      << ", V ispolzovanii: " << (pipe.inUse ? "Da" : "Net")
                      << ", Proizvoditelnost: " << pipe.getCapacity() << " ed." << std::endl;
        }
    }
    
    std::cout << "\n=== KOMPRESSORNYE STANCII ===" << std::endl;
    if (stations.empty()) {
        std::cout << "Net kompressornyh stancij." << std::endl;
    } else {
        for (const auto& station : stations) {
            double percent = (station.totalWorkshops - station.workingWorkshops) * 100.0 / station.totalWorkshops;
            std::cout << "ID: " << station.id << ", Nazvanie: " << station.name
                      << ", Ceha: " << station.workingWorkshops << "/" << station.totalWorkshops
                      << ", Neispolzovano: " << percent << "%"
                      << ", Klass: " << station.stationClass << std::endl;
        }
    }
}

void displayNetwork() {
    std::cout << "\n=== GASOTRANSPORTNAYA SET ===" << std::endl;
    if (connections.empty()) {
        std::cout << "Set pusta." << std::endl;
    } else {
        for (const auto& conn : connections) {
            std::cout << "KS " << conn.fromStationId << " -> KS " << conn.toStationId 
                      << " (Truba ID: " << conn.pipeId << ")" << std::endl;
        }
    }
}

bool stationExists(int id) {
    for (const auto& station : stations) {
        if (station.id == id) {
            return true;
        }
    }
    return false;
}

void connectStations() {
    if (stations.size() < 2) {
        std::cout << "Dolzhno byt minimum 2 KS dlja soedinenija." << std::endl;
        return;
    }
    
    std::cout << "Dostupnye diametry: 500, 700, 1000, 1400" << std::endl;
    std::cout << "Vvedite diametr truby: ";
    int diameter;
    while (!(std::cin >> diameter) || 
           (diameter != 500 && diameter != 700 && 
            diameter != 1000 && diameter != 1400)) {
        std::cout << "Nevernyj vvod. Vvedite odin iz dostupnyh diametrov (500, 700, 1000, 1400): ";
        clearInputBuffer();
    }
    
    Pipe* availablePipe = findAvailablePipe(diameter);
    
    if (!availablePipe) {
        std::cout << "Net dostupnyh trub s diametrom " << diameter << " mm." << std::endl;
        std::cout << "Sozdat novuju trubu? (1 - Da, 0 - Net): ";
        int choice;
        std::cin >> choice;
        clearInputBuffer();
        
        if (choice == 1) {
            addPipe();
            if (!pipes.empty()) {
                availablePipe = &pipes.back();
                if (availablePipe->diameter != diameter) {
                    std::cout << "Diametr novoj truby ne sovpadaet s trebuemym!" << std::endl;
                    return;
                }
            } else {
                std::cout << "Oshibka pri sozdanii truby!" << std::endl;
                return;
            }
        } else {
            return;
        }
    }
    
    std::cout << "Vvedite ID KS vhoda: ";
    int fromId;
    std::cin >> fromId;
    
    std::cout << "Vvedite ID KS vyhoda: ";
    int toId;
    std::cin >> toId;
    clearInputBuffer();
    
    if (!stationExists(fromId)) {
        std::cout << "KS s ID " << fromId << " ne sushhestvuet." << std::endl;
        return;
    }
    
    if (!stationExists(toId)) {
        std::cout << "KS s ID " << toId << " ne sushhestvuet." << std::endl;
        return;
    }
    
    if (fromId == toId) {
        std::cout << "Nelzja soedinit KS s samoj soboj." << std::endl;
        return;
    }
    
    for (const auto& conn : connections) {
        if (conn.fromStationId == fromId && conn.toStationId == toId) {
            std::cout << "Soedinenie uzhe sushhestvuet." << std::endl;
            return;
        }
    }
    
    availablePipe->inUse = true;
    
    NetworkConnection newConn(availablePipe->id, fromId, toId);
    connections.push_back(newConn);
    
    logAction("Soedinenie: KS " + std::to_string(fromId) + " -> KS " + 
              std::to_string(toId) + " (Truba ID: " + std::to_string(availablePipe->id) + ")");
    std::cout << "KS uspeshno soedineny!" << std::endl;
}

void disconnectStations() {
    if (connections.empty()) {
        std::cout << "Net soedinenij dlja razryva." << std::endl;
        return;
    }
    
    std::cout << "Vvedite ID KS vhoda: ";
    int fromId;
    std::cin >> fromId;
    
    std::cout << "Vvedite ID KS vyhoda: ";
    int toId;
    std::cin >> toId;
    clearInputBuffer();
    
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        if (it->fromStationId == fromId && it->toStationId == toId) {
            for (auto& pipe : pipes) {
                if (pipe.id == it->pipeId) {
                    pipe.inUse = false;
                    break;
                }
            }
            
            connections.erase(it);
            logAction("Razryv soedinenija: KS " + std::to_string(fromId) + " -> KS " + std::to_string(toId));
            std::cout << "Soedinenie razorvano!" << std::endl;
            return;
        }
    }
    
    std::cout << "Soedinenie ne najdeno." << std::endl;
}

void topologicalSort() {
    if (connections.empty()) {
        std::cout << "Set pusta. Net chto sortirovat." << std::endl;
        return;
    }
    
    std::map<int, std::vector<int>> graph;
    std::map<int, int> inDegree;
    
    for (const auto& station : stations) {
        graph[station.id] = std::vector<int>();
        inDegree[station.id] = 0;
    }
    
    for (const auto& conn : connections) {
        graph[conn.fromStationId].push_back(conn.toStationId);
        inDegree[conn.toStationId]++;
    }
    
    std::queue<int> zeroInDegree;
    for (const auto& [node, degree] : inDegree) {
        if (degree == 0) {
            zeroInDegree.push(node);
        }
    }
    
    std::vector<int> sortedOrder;
    
    while (!zeroInDegree.empty()) {
        int current = zeroInDegree.front();
        zeroInDegree.pop();
        sortedOrder.push_back(current);
        
        for (int neighbor : graph[current]) {
            inDegree[neighbor]--;
            if (inDegree[neighbor] == 0) {
                zeroInDegree.push(neighbor);
            }
        }
    }
    
    if (sortedOrder.size() != stations.size()) {
        std::cout << "V grafe obnaruzhen cikl! Topologicheskaja sortirovka nevozmozhna." << std::endl;
        return;
    }
    
    std::cout << "\n=== TOPOLOGICHESKAYA SORTIROVKA KS ===" << std::endl;
    for (size_t i = 0; i < sortedOrder.size(); ++i) {
        std::string stationName = "N/A";
        for (const auto& station : stations) {
            if (station.id == sortedOrder[i]) {
                stationName = station.name;
                break;
            }
        }
        std::cout << i + 1 << ". KS " << sortedOrder[i] << " (" << stationName << ")" << std::endl;
    }
}

double calculateMaxFlow(int sourceId, int sinkId) {
    if (connections.empty()) {
        std::cout << "Set pusta." << std::endl;
        return 0.0;
    }
    
    if (!stationExists(sourceId)) {
        std::cout << "Istochnik s ID " << sourceId << " ne sushhestvuet." << std::endl;
        return 0.0;
    }
    
    if (!stationExists(sinkId)) {
        std::cout << "Stok s ID " << sinkId << " ne sushhestvuet." << std::endl;
        return 0.0;
    }
    
    if (sourceId == sinkId) {
        std::cout << "Istochnik i stok ne mogut byt odnoj i toj zhe KS." << std::endl;
        return 0.0;
    }
    
    std::map<int, std::vector<std::pair<int, double>>> graph;
    
    for (const auto& conn : connections) {
        double capacity = 0.0;
        for (const auto& pipe : pipes) {
            if (pipe.id == conn.pipeId) {
                capacity = pipe.getCapacity();
                break;
            }
        }
        
        graph[conn.fromStationId].push_back({conn.toStationId, capacity});
        graph[conn.toStationId].push_back({conn.fromStationId, 0.0});
    }
    
    std::map<int, std::map<int, double>> flow;
    for (const auto& [from, neighbors] : graph) {
        for (const auto& [to, cap] : neighbors) {
            flow[from][to] = 0.0;
        }
    }
    
    double maxFlow = 0.0;
    
    while (true) {
        std::map<int, int> parent;
        std::queue<int> q;
        q.push(sourceId);
        parent[sourceId] = -1;
        
        while (!q.empty() && parent.find(sinkId) == parent.end()) {
            int current = q.front();
            q.pop();
            
            for (const auto& [neighbor, capacity] : graph[current]) {
                if (parent.find(neighbor) == parent.end() && 
                    capacity > flow[current][neighbor]) {
                    parent[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }
        
        if (parent.find(sinkId) == parent.end()) {
            break;
        }
        
        double pathFlow = std::numeric_limits<double>::infinity();
        for (int v = sinkId; v != sourceId; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, graph[u].at(v) - flow[u][v]);
        }
        
        for (int v = sinkId; v != sourceId; v = parent[v]) {
            int u = parent[v];
            flow[u][v] += pathFlow;
            flow[v][u] -= pathFlow;
        }
        
        maxFlow += pathFlow;
    }
    
    return maxFlow;
}

void calculateShortestPath(int startId, int endId) {
    if (connections.empty()) {
        std::cout << "Set pusta." << std::endl;
        return;
    }
    
    if (!stationExists(startId)) {
        std::cout << "Nachalnaja KS s ID " << startId << " ne sushhestvuet." << std::endl;
        return;
    }
    
    if (!stationExists(endId)) {
        std::cout << "Konechnaja KS s ID " << endId << " ne sushhestvuet." << std::endl;
        return;
    }
    
    if (startId == endId) {
        std::cout << "Nachalnaja i konechnaja KS ne mogut byt odnoj i toj zhe." << std::endl;
        return;
    }
    
    std::map<int, std::vector<std::pair<int, double>>> graph;
    
    for (const auto& conn : connections) {
        double weight = 0.0;
        for (const auto& pipe : pipes) {
            if (pipe.id == conn.pipeId) {
                weight = pipe.getWeight();
                break;
            }
        }
        
        graph[conn.fromStationId].push_back({conn.toStationId, weight});
    }
    
    std::map<int, double> dist;
    std::map<int, int> parent;
    std::set<std::pair<double, int>> pq;
    
    for (const auto& station : stations) {
        dist[station.id] = std::numeric_limits<double>::infinity();
    }
    dist[startId] = 0.0;
    parent[startId] = -1;
    
    pq.insert({0.0, startId});
    
    while (!pq.empty()) {
        auto [currentDist, current] = *pq.begin();
        pq.erase(pq.begin());
        
        if (current == endId) {
            break;
        }
        
        for (const auto& [neighbor, weight] : graph[current]) {
            double newDist = currentDist + weight;
            
            if (newDist < dist[neighbor]) {
                pq.erase({dist[neighbor], neighbor});
                dist[neighbor] = newDist;
                parent[neighbor] = current;
                pq.insert({newDist, neighbor});
            }
        }
    }
    
    if (dist[endId] == std::numeric_limits<double>::infinity()) {
        std::cout << "Put mezhdu KS " << startId << " i KS " << endId << " ne najden." << std::endl;
        return;
    }
    
    std::vector<int> path;
    for (int v = endId; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    std::reverse(path.begin(), path.end());
    
    std::cout << "\n=== KRATCHAISHIJ PUT ===" << std::endl;
    std::cout << "Ot KS " << startId << " do KS " << endId << std::endl;
    std::cout << "Obshhaja dlina: " << dist[endId] << " km" << std::endl;
    std::cout << "Marshrut: ";
    
    for (size_t i = 0; i < path.size(); ++i) {
        std::string stationName = "N/A";
        for (const auto& station : stations) {
            if (station.id == path[i]) {
                stationName = station.name;
                break;
            }
        }
        std::cout << "KS " << path[i] << " (" << stationName << ")";
        if (i < path.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
}

void maxFlowMenu() {
    if (stations.empty()) {
        std::cout << "Net KS dlja rascheta." << std::endl;
        return;
    }
    
    std::cout << "Vvedite ID KS istochnika: ";
    int sourceId;
    std::cin >> sourceId;
    
    std::cout << "Vvedite ID KS stoka: ";
    int sinkId;
    std::cin >> sinkId;
    clearInputBuffer();
    
    double maxFlow = calculateMaxFlow(sourceId, sinkId);
    
    std::cout << "\n=== MAKSIMALNYJ POTOK ===" << std::endl;
    std::cout << "Ot KS " << sourceId << " do KS " << sinkId << std::endl;
    std::cout << "Maksimalnyj potok: " << maxFlow << " ed." << std::endl;
    
    logAction("Raschet maksimalnogo potoka: KS " + std::to_string(sourceId) + 
              " -> KS " + std::to_string(sinkId) + " = " + std::to_string(maxFlow) + " ed.");
}

void shortestPathMenu() {
    if (stations.empty()) {
        std::cout << "Net KS dlja rascheta." << std::endl;
        return;
    }
    
    std::cout << "Vvedite ID nachalnoj KS: ";
    int startId;
    std::cin >> startId;
    
    std::cout << "Vvedite ID konechnoj KS: ";
    int endId;
    std::cin >> endId;
    clearInputBuffer();
    
    calculateShortestPath(startId, endId);
}

void editCompressorStation() {
    if (stations.empty()) {
        std::cout << "Net kompressornyh stancij dlja redaktirovanija." << std::endl;
        return;
    }
    
    std::cout << "Vvedite ID kompressornoj stancii dlja redaktirovanija: ";
    int id;
    std::cin >> id;
    clearInputBuffer();
    
    for (auto& station : stations) {
        if (station.id == id) {
            std::cout << "Redaktirovanie KS ID: " << station.id << std::endl;
            
            std::cout << "Tekushee nazvanie: " << station.name << std::endl;
            std::cout << "Vvedite novoe nazvanie: ";
            std::getline(std::cin, station.name);
            
            std::cout << "Tekushee obshhee kolichestvo cehov: " << station.totalWorkshops << std::endl;
            std::cout << "Vvedite novoe obshhee kolichestvo cehov: ";
            while (!(std::cin >> station.totalWorkshops) || station.totalWorkshops <= 0) {
                std::cout << "Nevernyj vvod. Vvedite polozhitelnoe celoe chislo: ";
                clearInputBuffer();
            }
            
            std::cout << "Tekushee kolichestvo rabotayushhih cehov: " << station.workingWorkshops << std::endl;
            std::cout << "Vvedite novoe kolichestvo rabotayushhih cehov: ";
            while (!(std::cin >> station.workingWorkshops) || station.workingWorkshops < 0 || station.workingWorkshops > station.totalWorkshops) {
                std::cout << "Nevernyj vvod. Vvedite chislo ot 0 do " << station.totalWorkshops << ": ";
                clearInputBuffer();
            }
            
            std::cout << "Tekushij klass: " << station.stationClass << std::endl;
            std::cout << "Vvedite novyj klass: ";
            while (!(std::cin >> station.stationClass) || station.stationClass <= 0) {
                std::cout << "Nevernyj vvod. Vvedite polozhitelnoe celoe chislo: ";
                clearInputBuffer();
            }
            
            clearInputBuffer();
            logAction("Otredaktirovana KS ID: " + std::to_string(station.id));
            std::cout << "Kompressornaja stancija uspeshno otredaktirovana!" << std::endl;
            return;
        }
    }
    std::cout << "Kompressornaja stancija s ID " << id << " ne najdena." << std::endl;
}

void editPipe() {
    if (pipes.empty()) {
        std::cout << "Net trub dlja redaktirovanija." << std::endl;
        return;
    }
    
    std::cout << "Vvedite ID truby dlja redaktirovanija: ";
    int id;
    std::cin >> id;
    clearInputBuffer();
    
    for (auto& pipe : pipes) {
        if (pipe.id == id) {
            if (pipe.inUse) {
                std::cout << "Truba ispolzuetsja v seti. Redaktirovanie ogranicheno." << std::endl;
                return;
            }
            
            std::cout << "Redaktirovanie truby ID: " << pipe.id << std::endl;
            std::cout << "Tekushee nazvanie: " << pipe.name << std::endl;
            std::cout << "Vvedite novoe nazvanie: ";
            std::getline(std::cin, pipe.name);
            
            std::cout << "Tekushaya dlina: " << pipe.length << " km" << std::endl;
            std::cout << "Vvedite novuyu dlinu (km): ";
            while (!(std::cin >> pipe.length) || pipe.length <= 0) {
                std::cout << "Nevernyj vvod. Vvedite polozhitelnoe chislo: ";
                clearInputBuffer();
            }
            
            std::cout << "Tekushij diametr: " << pipe.diameter << " mm" << std::endl;
            std::cout << "Vvedite novyj diametr (mm): ";
            std::cout << "Dostupnye diametry: 500, 700, 1000, 1400" << std::endl;
            while (!(std::cin >> pipe.diameter) || 
                   (pipe.diameter != 500 && pipe.diameter != 700 && 
                    pipe.diameter != 1000 && pipe.diameter != 1400)) {
                std::cout << "Nevernyj vvod. Vvedite odin iz dostupnyh diametrov (500, 700, 1000, 1400): ";
                clearInputBuffer();
            }
            
            clearInputBuffer();
            logAction("Otredaktirovana truba ID: " + std::to_string(pipe.id));
            std::cout << "Truba uspeshno otredaktirovana!" << std::endl;
            return;
        }
    }
    std::cout << "Truba s ID " << id << " ne najdena." << std::endl;
}

void deletePipe() {
    if (pipes.empty()) {
        std::cout << "Net trub dlja udalenija." << std::endl;
        return;
    }
    
    std::cout << "Vvedite ID truby dlja udalenija: ";
    int id;
    std::cin >> id;
    clearInputBuffer();
    
    for (const auto& conn : connections) {
        if (conn.pipeId == id) {
            std::cout << "Truba ispolzuetsja v seti. Snachala razorvite soedinenija." << std::endl;
            return;
        }
    }
    
    for (auto it = pipes.begin(); it != pipes.end(); ++it) {
        if (it->id == id) {
            pipes.erase(it);
            logAction("Udalena truba ID: " + std::to_string(id));
            std::cout << "Truba uspeshno udalena!" << std::endl;
            return;
        }
    }
    std::cout << "Truba s ID " << id << " ne najdena." << std::endl;
}

void deleteCompressorStation() {
    if (stations.empty()) {
        std::cout << "Net kompressornyh stancij dlja udalenija." << std::endl;
        return;
    }
    
    std::cout << "Vvedite ID kompressornoj stancii dlja udalenija: ";
    int id;
    std::cin >> id;
    clearInputBuffer();
    
    for (const auto& conn : connections) {
        if (conn.fromStationId == id || conn.toStationId == id) {
            std::cout << "KS ispolzuetsja v seti. Snachala razorvite soedinenija." << std::endl;
            return;
        }
    }
    
    for (auto it = stations.begin(); it != stations.end(); ++it) {
        if (it->id == id) {
            stations.erase(it);
            logAction("Udalena KS ID: " + std::to_string(id));
            std::cout << "Kompressornaja stancija uspeshno udalena!" << std::endl;
            return;
        }
    }
    std::cout << "Kompressornaja stancija s ID " << id << " ne najdena." << std::endl;
}

void saveToFile() {
    std::string filename;
    std::cout << "Vvedite imja fajla dlja sohranenija: ";
    std::getline(std::cin, filename);
    
    std::ofstream file(filename);
    if (file.is_open()) {
        file << nextPipeId << std::endl;
        file << nextStationId << std::endl;
        
        file << pipes.size() << std::endl;
        for (const auto& pipe : pipes) {
            file << pipe.id << std::endl;
            file << pipe.name << std::endl;
            file << pipe.length << std::endl;
            file << pipe.diameter << std::endl;
            file << pipe.underRepair << std::endl;
            file << pipe.inUse << std::endl;
        }
        
        file << stations.size() << std::endl;
        for (const auto& station : stations) {
            file << station.id << std::endl;
            file << station.name << std::endl;
            file << station.totalWorkshops << std::endl;
            file << station.workingWorkshops << std::endl;
            file << station.stationClass << std::endl;
        }
        
        file << connections.size() << std::endl;
        for (const auto& conn : connections) {
            file << conn.pipeId << std::endl;
            file << conn.fromStationId << std::endl;
            file << conn.toStationId << std::endl;
        }
        
        logAction("Sohranenie dannyh v fajl: " + filename);
        std::cout << "Dannyye uspeshno sohraneny v fajl: " << filename << std::endl;
    } else {
        std::cout << "Oshibka sohranenija fajla!" << std::endl;
    }
}

void loadFromFile() {
    std::string filename;
    std::cout << "Vvedite imja fajla dlja zagruzki: ";
    std::getline(std::cin, filename);
    
    std::ifstream file(filename);
    if (file.is_open()) {
        pipes.clear();
        stations.clear();
        connections.clear();
        
        file >> nextPipeId;
        file >> nextStationId;
        clearInputBuffer();
        
        int pipeCount;
        file >> pipeCount;
        clearInputBuffer();
        for (int i = 0; i < pipeCount; i++) {
            Pipe pipe;
            file >> pipe.id;
            clearInputBuffer();
            std::getline(file, pipe.name);
            file >> pipe.length;
            file >> pipe.diameter;
            file >> pipe.underRepair;
            file >> pipe.inUse;
            clearInputBuffer();
            pipes.push_back(pipe);
        }
        
        int stationCount;
        file >> stationCount;
        clearInputBuffer();
        for (int i = 0; i < stationCount; i++) {
            CompressorStation station;
            file >> station.id;
            clearInputBuffer();
            std::getline(file, station.name);
            file >> station.totalWorkshops;
            file >> station.workingWorkshops;
            file >> station.stationClass;
            clearInputBuffer();
            stations.push_back(station);
        }
        
        int connCount;
        file >> connCount;
        clearInputBuffer();
        for (int i = 0; i < connCount; i++) {
            NetworkConnection conn;
            file >> conn.pipeId;
            file >> conn.fromStationId;
            file >> conn.toStationId;
            clearInputBuffer();
            connections.push_back(conn);
        }
        
        logAction("Zagruzka dannyh iz fajla: " + filename);
        std::cout << "Dannyye uspeshno zagruzheny iz fajla: " << filename << std::endl;
    } else {
        std::cout << "Oshibka zagruzki fajla!" << std::endl;
    }
}

int main() {
    int choice;
    
    do {
        std::cout << "\n=== GLAVNOE MENU ===" << std::endl;
        std::cout << "1. Dobavit trubu" << std::endl;
        std::cout << "2. Dobavit kompressornuju stanciju" << std::endl;
        std::cout << "3. Prosmotr vseh obektov" << std::endl;
        std::cout << "4. Prosmotr seti" << std::endl;
        std::cout << "5. Soedinit KS" << std::endl;
        std::cout << "6. Razorvat soedinenie" << std::endl;
        std::cout << "7. Topologicheskaja sortirovka" << std::endl;
        std::cout << "8. Maksimalnyj potok" << std::endl;
        std::cout << "9. Kratchajshij put" << std::endl;
        std::cout << "10. Redaktirovat trubu" << std::endl;
        std::cout << "11. Redaktirovat kompressornuju stanciju" << std::endl;
        std::cout << "12. Udalit trubu" << std::endl;
        std::cout << "13. Udalit kompressornuju stanciju" << std::endl;
        std::cout << "14. Sohranit v fajl" << std::endl;
        std::