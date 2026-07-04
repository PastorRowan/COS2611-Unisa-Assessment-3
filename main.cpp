
/*
Name: Rowan
Surname: Van Zyl
Student Number: 25939831
Module: COS2611
Assignment: Assessment 3
*/

#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <cstdlib>
#include <string>

// Declarations
void clearScreen();
struct Edge;
struct Node;
class ConservationNetworkGraph;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
};


struct Edge {
    const unsigned int to;
    const unsigned int weight;
};

typedef std::vector<Edge> Edges;

struct Node {
    const std::string name;
    const std::string shortName;
    const Edges edges;
};

typedef std::vector<Node> AdjList;

class ConservationNetworkGraph {

    private:

        const AdjList adjList;

    public:

        ConservationNetworkGraph(
            const AdjList adjListP
        ): adjList(adjListP) {};

        std::string toString() {

            std::string output = "Conservation Network:\n\n";

            // Loop over all nodes in the adjacency list
            for (unsigned int i = 0; i < adjList.size(); ++i) {

                // Get current node data
                const Node& currentNode = adjList.at(i);
                const std::string& currentNodeName = currentNode.name;
                const Edges& currentNodeEdges = currentNode.edges;

                // Loop over the current node's edges
                for (unsigned int j = 0; j < currentNodeEdges.size(); ++j) {

                    // Get current edge data
                    const Edge& currentEdge = currentNodeEdges[j];
                    const unsigned int& currentNeighborIndex = currentEdge.to;
                    const unsigned int& currentEdgeWeight = currentEdge.weight;

                    // Get current neighbor data
                    const Node& currentNeighbor = adjList.at(currentNeighborIndex);
                    const std::string& currentNeighborName = currentNeighbor.name;

                    // append edge data to console
                    output += currentNodeName + " -> " + currentNeighborName + " (" + std::to_string(currentEdgeWeight) + " km)\n";

                };

                output += '\n';

            };

            return output;

        };

        std::string adjListToString() {

            std::string output = "";

            for (unsigned int i = 0; i < adjList.size(); ++i) {
                const Node& currentNode = adjList.at(i);
                const std::string& currentNodeShortName = currentNode.shortName;
                output += '\t' + currentNodeShortName;
            };

            output += '\n';

            for (unsigned int i = 0; i < adjList.size(); ++i) {

                const unsigned int& currentNodeIndex = i;
                const Node& currentNode = adjList.at(currentNodeIndex);
                const std::string& currentNodeShortName = currentNode.shortName;
                const Edges& currentNodeEdges = currentNode.edges;

                output += currentNodeShortName + '\t';

                for (unsigned int j = 0; j < adjList.size(); ++j) {

                    const unsigned int& currentOtherNodeIndex = j;

                    const Edge* currentNodeEdge = nullptr;

                    for (unsigned int k = 0; k < currentNodeEdges.size(); ++k) {

                        const unsigned int& currentNodeNeighborIndex = k;
                        const Edge& currentNodeNeighbor = currentNodeEdges.at(currentNodeNeighborIndex);
                        const unsigned int& currentNodeNeighborIndex = currentNodeNeighbor.to;

                        if (currentOtherNodeIndex == currentNodeNeighborIndex) {
                            currentNodeEdge = &currentNodeNeighbor;
                            break;
                        };
                    };

                    if (currentNodeEdge != nullptr) {
                        const unsigned int& currentNodeEdgeWeight = currentNodeEdge->weight;
                        output += std::to_string(currentNodeEdgeWeight) + '\t';
                    } else {
                        output += '0' + '\t';
                    };

                };
            };

        };

        /*

        void BFS(int start) {

            std::queue<int> visiting;

            std::vector<bool> visited(adjList.size(), false);

            visiting.push(start);
            visited.at(start) = true;

            while (!visiting.empty()) {

                int vIndex = visiting.front();
                visiting.pop();
                std::cout << vIndex << ' ';

                const std::vector<int>& vertexNeighbors = adjList.at(vIndex);
                for (unsigned int i = 0; i < vertexNeighbors.size(); i++) {
                    const int neighbor = vertexNeighbors.at(i);
                    if (!visited.at(neighbor)) {
                        visiting.push(neighbor);
                        visited.at(neighbor) = true;
                    };
                };
            };

        };

        void DFS(const std::vector<std::vector<int>>& adjList, int start) {

            std::stack<int> visiting;

            std::vector<bool> visited(adjList.size(), false);

            visiting.push(start);
            visited.at(start) = true;

            while (!visiting.empty()) {

                int vIndex = visiting.top();
                visiting.pop();
                std::cout << vIndex << ' ';

                const std::vector<int>& vertexNeighbors = adjList.at(vIndex);
                for (int i = vertexNeighbors.size() - 1; i >= 0; i--) {
                    const int neighbor = vertexNeighbors.at(i);
                    if (!visited.at(neighbor)) {
                        visiting.push(neighbor);
                        visited.at(neighbor) = true;
                    };
                };
            };

        };
        */
};

int main() {

    ConservationNetworkGraph conservationNetworkGraph({
        /*
        Kruger National Park
        Index = 0
        */
        {
            .name = "Kruger",
            .shortName = "KR",
            .edges = {
                /*
                Kruger -> Limpopo
                0 -> 1
                */
                {
                    .to = 1,
                    .weight = 120
                },
                /*
                Kruger -> Hwange
                0 -> 2
                */
                {
                    .to = 2,
                    .weight = 800
                },
                /*
                Kruger -> Kgalagadi
                0 -> 5
                */
                {
                    .to = 5,
                    .weight = 1100
                },
            }
        },
        /*
        Complete
        Limpopo National Park
        Index = 1
        */
        {
            .name = "Limpopo",
            .shortName = "LI",
            .edges = {
                /*
                Limpopo -> Kruger
                1 -> 0
                */
                {
                    .to = 0,
                    .weight = 120
                },
                /*
                Limpopo -> Hwange
                1 -> 2
                */
                {
                    .to = 2,
                    .weight = 850
                }
            }
        },
        /*
        Hwange National Park
        Index = 2
        */
        {
            .name = "Hwange",
            .shortName = "HW",
            .edges = {
                /*
                Hwange -> Kruger
                2 -> 0
                */
                {
                    .to = 0,
                    .weight = 800
                },
                /*
                Hwange -> Limpopo
                2 -> 1
                */
                {
                    .to = 1,
                    .weight = 850
                },
                /*
                Hwange  -> Chobe
                2 -> 3
                */
                {
                    .to = 3,
                    .weight = 90
                },
                /*
                Hwange -> Kgalagadi
                2 -> 5
                */
                {
                    .to = 5,
                    .weight = 900
                }
            }
        },
        /*
        complete
        Chobe National Park
        Index = 3
        */
        {
            .name = "Chobe",
            .shortName = "CH",
            .edges = {
                /*
                Chobe -> Hwange
                3 -> 2
                */
                {
                    .to = 2,
                    .weight = 90
                },
                /*
                Chobe -> Etosha
                3 -> 4
                */
                {
                    .to = 4,
                    .weight = 950
                },
                /*
                Chobe -> Kgalagadi
                3 -> 5
                */
                {
                    .to = 5,
                    .weight = 900
                }
            }
        },
        /*
        complete
        Etosha National Park
        Index = 4
        */
        {
            .name = "Etosha",
            .shortName = "ET",
            .edges = {
                /*
                Etosha -> Kgalagadi
                4 -> 5
                */
                {
                    .to = 5,
                    .weight = 850
                },
                /*
                Etosha -> Chobe
                4 -> 3
                */
                {
                    .to = 3,
                    .weight = 950
                }
            }
        },
        /*
        complete
        Kgalagadi Transfrontier Park
        Index = 5
        */
        {
            .name = "Kgalagadi",
            .shortName = "KG",
            .edges = {
                /*
                Kgalagadi -> Kruger
                5 -> 0
                */
                {
                    .to = 0,
                    .weight = 1100
                },
                /*
                Kgalagadi -> Etosha
                5 -> 4
                */
                {
                    .to = 4,
                    .weight = 850
                },
                /*
                Kgalagadi -> Chobe
                5 -> 3
                */
                {
                    .to = 3,
                    .weight = 900
                },
                /*
                Kgalagadi -> Hwange
                5 -> 3
                */
                {
                    .to = 2,
                    .weight = 900
                }
            }
        }
    });

    const std::string HEADER = R"(
Programmer: Rowan Van Zyl, 25939831
Wildlife Corridor Network System
----------------------------------

)";

    std::string input_str = "";
    char input_char = ' ';
    std::string status = "Ok";
    bool running  = true;

    do {

        clearScreen();

        std::cout
            << HEADER
            << "1. Display conservation network" << std::endl
            << "2. Display adjacency matrix" << std::endl
            << "3. Perform BFS traversal" << std::endl
            << "4. Find shortest path between parks" << std::endl
            << "5. Exit" << std::endl
            << std::endl
            << "Status: " << status << std::endl
            << std::endl
        ;

        std::getline(
            std::cin,
            input_str,
            '\n'
        );

        if (
            (
                input_str.size() >= 2
            ) ||
            !(
                input_str[0] == '1' ||
                input_str[0] == '2' ||
                input_str[0] == '3' ||
                input_str[0] == '4' ||
                input_str[0] == '5'
            )
        ) {
            status = "Error: input must be a character 1, 2, 3, 4 or 5";
            continue;
        };

        input_char = input_str[0];

        switch (input_char) {
            case '1': {
                clearScreen();
                std::cout
                    << HEADER
                    << conservationNetworkGraph.toString()
                    << "Press enter to continue..."
                    << std::endl
                ;
                std::string dummyStr;
                std::getline(
                    std::cin,
                    dummyStr,
                    '\n'
                );
                break;
            };
            case '2': {
                /*
Adjacency Matrix (km)

        KR   LI   CH   HW   ET   KG
KR       0   120    0    0    0   500
LI     120     0  290    0    0     0
CH       0   290    0   90    0     0
HW       0     0   90    0  600     0
ET       0     0    0  600    0   850
KG     500     0    0    0  850     0
                */
                return 0;
                break;
            };
            case '3': {
/*
BFS Traversal starting from Kruger:

Kruger → Limpopo → Kgalagadi → Chobe → Etosha → Hwange
*/
                return 0;
                break;
            };
            case '4': {
/*
Shortest path from Kruger to Hwange:

Kruger → Limpopo → Chobe → Hwange

Total distance: 500 km
Shortest path from Kruger to Etosha:

Kruger → Kgalagadi → Etosha

Total distance: 1350 km
*/
                return 0;
                break;
            };
            case '5': {
                std::cout << "Exiting program..." << std::endl;
                running = false;
                break;
            };
            default: {
                std::cout << "Error invalid input character: '" << input_char << '\'' << std::endl;
                return 1;
                break;
            };
        };

    } while(running);

    return 0;

};

