
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
#include <stdexcept>
#include <limits>
#include <algorithm>

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

                        const Edge& edge = currentNodeEdges.at(k);

                        if (currentOtherNodeIndex == edge.to) {
                            currentNodeEdge = &edge;
                            break;
                        };

                    };

                    if (currentNodeEdge != nullptr) {
                        const unsigned int& currentNodeEdgeWeight = currentNodeEdge->weight;
                        output += std::to_string(currentNodeEdgeWeight) + "\t";
                    } else {
                        output += "0\t";
                    };

                };

                output += '\n';

            };

            return output;

        };

        std::string BFSPathToString(
            const unsigned int start
        ) {

            std::string output = "";

            if (start >= adjList.size()) {
                output = "Start index '" + std::to_string(start) + "' is not from 0 to '" + std::to_string(adjList.size() - 1) + "' (inclusive)";
                return output;
            };

            std::queue<unsigned int> visiting;

            bool visited[adjList.size()] = {};

            for (unsigned int i = 0; i < adjList.size(); ++i) {
                visited[i] = false;
            };

            visiting.push(start);
            visited[start] = true;

            while (!visiting.empty()) {
                const unsigned int& currentNodeIndex = visiting.front();
                const Node& currentNode = adjList.at(currentNodeIndex);
                const std::string& currentName = currentNode.shortName;
                output += currentName + " -> ";
                const Edges& currentEdges = currentNode.edges;
                visiting.pop();
                for (unsigned int i = 0; i < currentEdges.size(); ++i) {
                    const Edge& currentEdge = currentEdges.at(i);
                    const unsigned int& currentNeighborIndex = currentEdge.to;
                    if (!visited[currentNeighborIndex]) {
                        visiting.push(currentNeighborIndex);
                        visited[currentNeighborIndex] = true;
                    };
                };
            };

            return output;

        };

        std::string shortestPathToString(
            const unsigned int& from,
            const unsigned int& to
        ) {

            if (from >= adjList.size()) {
                std::string errorMsg = "from index '" + std::to_string(from) + "' is not from 0 to '" + std::to_string(adjList.size() - 1) + "' (inclusive)";
                return errorMsg;
            };

            if (to >= adjList.size()) {
                std::string errorMsg = "to index '" + std::to_string(to) + "' is not from 0 to '" + std::to_string(adjList.size() - 1) + "' (inclusive)";
                return errorMsg;
            };

            const unsigned int& INF = UINT_MAX;
            const unsigned int& NULL_PREV = UINT_MAX;

            std::vector<unsigned int> queue;
            queue.reserve(adjList.size());

            unsigned int dist[adjList.size()] = {};
            unsigned int prev[adjList.size()] = {};

            for (unsigned int i = 0; i < adjList.size(); ++i) {
                const unsigned int& currentNodeIndex = i;
                dist[currentNodeIndex] = INF;
                prev[currentNodeIndex] = NULL_PREV;
                queue.push_back(currentNodeIndex);
            };

            dist[from] = 0;

            while (!queue.empty()) {
                unsigned int closestNodeIndex = queue.front();
                for (unsigned int i = 0; i < queue.size(); ++i) {
                    const unsigned int& currentNodeIndex = queue.at(i);
                    const unsigned int& currentDistance = dist[currentNodeIndex];
                    if (dist[currentNodeIndex] < dist[closestNodeIndex]) {
                        closestNodeIndex = currentNodeIndex;
                    };
                };

                auto it = std::find(queue.begin(), queue.end(), closestNodeIndex);

                if (it != queue.end()) {
                    queue.erase(it);
                };

                const Node& closestNode = adjList.at(closestNodeIndex);
                const Edges& edges = closestNode.edges;

                for (unsigned int i = 0; i < edges.size(); ++i) {
                    const unsigned int& currentEdgeIndex = i;
                    const Edge& currentEdge = edges.at(currentEdgeIndex);
                    const unsigned int& currentNeighborIndex = currentEdge.to;
                    const unsigned int& distanceFromCurrentNodeToNeighbor = currentEdge.weight;
                    const unsigned int alternativeDistanceFromCurrentNodeToNeighbor = dist[closestNodeIndex] + distanceFromCurrentNodeToNeighbor;
                    if (alternativeDistanceFromCurrentNodeToNeighbor < dist[currentNeighborIndex]) {
                        dist[currentNeighborIndex] = alternativeDistanceFromCurrentNodeToNeighbor;
                        prev[currentNeighborIndex] = closestNodeIndex;
                    };
                };

            };

            std::vector<unsigned int> path = {};
            path.reserve(adjList.size());

            unsigned int current = to;

            if (prev[current] != NULL_PREV || current == from) {
                while (current != from) {
                    path.push_back(current);
                    current = prev[current];
                };
                path.push_back(from);
            };

            std::string output = "";

            const Node& fromNode = adjList.at(from);
            const Node& toNode = adjList.at(to);

            output += "shortest route from " + fromNode.name + " to " + toNode.name + "\n\n";

            for (unsigned int i = path.size() - 1; i < path.size(); --i) {
                const unsigned int& currentNodeIndex = path.at(i);
                const Node& currentNode = adjList.at(currentNodeIndex);
                const std::string& currentNodeName = currentNode.name;
                output += currentNodeName + (currentNodeIndex < path.size() - 1 ? " -> " : "");
            };

            output += "\n\n" + std::string("Total distance: ") + std::to_string(dist[to]) + "km";

            return output;

            /*
            Shortest route from Kruger to Hwange

            Kruger → Limpopo → Chobe → Hwange

            Total distance: 410 km
            */

        };

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
                clearScreen();
                std::cout
                    << HEADER
                    << conservationNetworkGraph.adjListToString()
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
            case '3': {
                clearScreen();
                std::cout
                    << HEADER
                    << conservationNetworkGraph.BFSPathToString(0) << std::endl
                    << std::endl
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
            case '4': {
                clearScreen();
                std::cout
                    << HEADER
                    << conservationNetworkGraph.shortestPathToString(1, 4) << std::endl
                    << std::endl
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

