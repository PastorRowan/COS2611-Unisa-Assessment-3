
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

// Functions clears the screen
void clearScreen() {
    /*
    If macro _WIN32 is defined then system cls command will run (program compiled on Windows computer)
    else the command system clear will run (program compiled on Mac or Linux computer)
    Both commands do the same thing that is:
        Clear the command line
        Move the cursor to home position (top left)
    */
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
};

/*
An Edge represents one wildlife corridor in the graph

to:
    Stores the index of the neighbouring conservation area

weight:
    Stores the distance between the two parks in kilometres

Each edge connects one park to another
*/
struct Edge {
    const unsigned int to;
    const unsigned int weight;
};

// Used for convenience later
typedef std::vector<Edge> Edges;

/*
A Node represents a national park

name:
    Stores the name of the national park

shortName:
    Stores the name of the national park as the 2 first letters capitalised

edges:
    A vector of edge structs
    Represents all possible wildlife corridors between this national park and others
*/
struct Node {
    const std::string name;
    const std::string shortName;
    const Edges edges;
};

// Used for convenience later
typedef std::vector<Node> AdjList;

/*
    A conservation network graphs is a collection of national parks and their wildlife corridors (paths)
    between each other
    The national parks are represented as nodes
    The wildlife corridators are represented by edges

    adjList:
        An array of Nodes
*/
class ConservationNetworkGraph {

    private:

        const AdjList adjList;

    public:

        /*
            ConservationNetworkGraph constructor

            Parameters:
            takes an adjacency list as input to set as the adjacency list for the network
        */ 
        ConservationNetworkGraph(
            const AdjList adjListP
        ): adjList(adjListP) {};

        /*
            getAdjList
            
            Returns a constant reference to adjList (adjList cannot be modified using the reference)
        */
        const AdjList& getAdjList() {
            return adjList;
        };

        /*
            Returns a string to visually represent the entire conservation network (All national parks
            and wildlife corridors between them)
        */
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

        /*
            adjListToString

            Returns a string representing the adjacency list of the conservation network as a table
            All national parks are represented using their short name
            The 1st row and 1st column contain the national parks short names
            The intersection of 2 national parks represent an edge from the row park to the column park (
            Not also the other way around)
            A 0 entry means that a wilflife corridor does not exist
            A positive value means that a wildlife corridor does exist from row park to column park
            and the distance is the value.
        */
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

        /*
            BFSPathToString

            Returns a string representing the path taken in a breadth 1st search of the
            conservation network from the start index

            A breadth 1st search is a method of traversing an entire graph
            It uses a queue to keep track of which parks to visit next
            The starting park is visited 1st, then all of its neighbor parks
            before moving onto the neighbours of those parks
            A visited list is maitained to ensure that each park is visited
            only once (Allowing infinite repeated visits would cause an infinite loop could occur)

            Parameters:
            start - a constant reference to the start index of the breadth first search

        */
        std::string BFSPathToString(
            const unsigned int& start
        ) {

            if (start >= adjList.size()) {
                std::string errorMsg = "Start index '" + std::to_string(start) + "' is not from 0 to '" + std::to_string(adjList.size() - 1) + "' (inclusive)";
                return errorMsg;
            };

            std::queue<unsigned int> visiting;
            std::vector<bool> visited(adjList.size(), false);

            visiting.push(start);
            visited.at(start) = true;

            std::vector<unsigned int> visitedNodesIndicesOrder = {};
            visitedNodesIndicesOrder.reserve(adjList.size());

            while (!visiting.empty()) {
                const unsigned int& currentNodeIndex = visiting.front();
                const Node& currentNode = adjList.at(currentNodeIndex);
                const std::string& currentName = currentNode.shortName;
                visitedNodesIndicesOrder.push_back(currentNodeIndex);
                const Edges& currentEdges = currentNode.edges;
                visiting.pop();
                for (unsigned int i = 0; i < currentEdges.size(); ++i) {
                    const Edge& currentEdge = currentEdges.at(i);
                    const unsigned int& currentNeighborIndex = currentEdge.to;
                    if (!visited.at(currentNeighborIndex)) {
                        visiting.push(currentNeighborIndex);
                        visited.at(currentNeighborIndex) = true;
                    };
                };
            };

            std::string output = "";

            for (unsigned int i = 0; i < visitedNodesIndicesOrder.size(); ++i) {
                const unsigned int& currentNodeIndex = visitedNodesIndicesOrder.at(i);
                const Node& currentNode = adjList.at(currentNodeIndex);
                const std::string& currentNodeName = currentNode.name;
                output += currentNodeName + (i ==  visitedNodesIndicesOrder.size() - 1 ? "" : " -> ");
            };

            return output;

        };

        /*
            shortestPathToString

            Returns a string representing the shortest path from one park to another

            Parameters:
            from - a constant reference to an index of the starting park within the adjacency list
            to - a constant reference to an index of the destination park within the adjacency list

            e.g:
                std::string shortestPath = 

            working on it:

            Shortest route from Kruger to Hwange

            Kruger → Limpopo → Chobe → Hwange

            Total distance: 410 km

        */
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
                output += currentNodeName + (i == 0 ? "" : " -> ");
            };

            output += "\n\n" + std::string("Total distance: ") + std::to_string(dist[to]) + "km";

            return output;

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
                { .to = 1, .weight = 120 },
                /*
                Kruger -> Hwange
                0 -> 2
                */
                { .to = 2, .weight = 800 },
                /*
                Kruger -> Kgalagadi
                0 -> 5
                */
                { .to = 5, .weight = 1100 },
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
                { .to = 0, .weight = 120 },
                /*
                Limpopo -> Hwange
                1 -> 2
                */
                { .to = 2, .weight = 850 }
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
                { .to = 0, .weight = 800 },
                /*
                Hwange -> Limpopo
                2 -> 1
                */
                { .to = 1, .weight = 850 },
                /*
                Hwange  -> Chobe
                2 -> 3
                */
                { .to = 3, .weight = 90 },
                /*
                Hwange -> Kgalagadi
                2 -> 5
                */
                { .to = 5, .weight = 900 }
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
                { .to = 2, .weight = 90 },
                /*
                Chobe -> Etosha
                3 -> 4
                */
                { .to = 4, .weight = 950 },
                /*
                Chobe -> Kgalagadi
                3 -> 5
                */
                { .to = 5, .weight = 900 }
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
                { .to = 5, .weight = 850 },
                /*
                Etosha -> Chobe
                4 -> 3
                */
                { .to = 3, .weight = 950 }
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
                { .to = 0, .weight = 1100 },
                /*
                Kgalagadi -> Etosha
                5 -> 4
                */
                { .to = 4, .weight = 850 },
                /*
                Kgalagadi -> Chobe
                5 -> 3
                */
                { .to = 3, .weight = 900 },
                /*
                Kgalagadi -> Hwange
                5 -> 3
                */
                { .to = 2, .weight = 900 }
            }
        }
    });

    const std::string HEADER = R"(
Programmer: Rowan Van Zyl, 25939831
Wildlife Corridor Network System
----------------------------------

)";

    const char DEFAULT_STATUS[] = "Ok";

    std::string input_str = "";
    char input_char = ' ';
    std::string status = DEFAULT_STATUS;
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

        status = DEFAULT_STATUS;

        std::getline(
            std::cin,
            input_str,
            '\n'
        );

        if (input_str.size() >= 2) {
            status = "Error: input must be only 1 character";
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
                    << "Parks:" << std::endl
                ;

                const AdjList& adjList = conservationNetworkGraph.getAdjList();

                for (unsigned int i = 0; i < adjList.size(); ++i) {
                    const Node& currentNode = adjList.at(i);
                    std::cout << i << ": " << currentNode.name << std::endl;
                };

                std::cout
                    << std::endl
                    << "Start location: "
                ;

                std::string inputStartParkStr = "";

                std::getline(
                    std::cin,
                    inputStartParkStr,
                    '\n'
                );

                unsigned int startParkIndex = 0;

                startParkIndex = inputStartParkStr[0] - '0';

                if (inputStartParkStr.size() != 1 || startParkIndex >= adjList.size()) {
                    std::cout
                        << std::endl
                        << "Error: input must be one character out of the valid options above" << std::endl
                        << std::endl
                        << "Press enter to continue..." << std::endl
                    ;
                    std::string dummyStr;
                    std::getline(
                        std::cin,
                        dummyStr,
                        '\n'
                    );
                    break;
                };

                std::cout
                    << std::endl
                    << conservationNetworkGraph.BFSPathToString(startParkIndex) << std::endl
                    << std::endl
                    << "Press enter to continue..." << std::endl
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
                    << "Parks:" << std::endl
                ;

                const AdjList& adjList = conservationNetworkGraph.getAdjList();

                for (unsigned int i = 0; i < adjList.size(); ++i) {
                    const Node& currentNode = adjList.at(i);
                    std::cout << i << " - " << currentNode.name << std::endl;
                };

                std::cout
                    << std::endl
                    << "Start location: "
                ;

                std::string inputStartParkStr = "";

                std::getline(
                    std::cin,
                    inputStartParkStr,
                    '\n'
                );

                unsigned int startParkIndex = 0;

                startParkIndex = inputStartParkStr[0] - '0';

                if (inputStartParkStr.size() != 1 || startParkIndex >= adjList.size()) {
                    std::cout
                        << std::endl
                        << "Error: input must be one character out of the valid options above" << std::endl
                        << std::endl
                        << "Press enter to continue..." << std::endl
                    ;
                    std::string dummyStr;
                    std::getline(
                        std::cin,
                        dummyStr,
                        '\n'
                    );
                    break;
                };

                std::cout
                    << "Destination location: "
                ;

                std::string inputDestinationParkStr = "";

                std::getline(
                    std::cin,
                    inputDestinationParkStr,
                    '\n'
                );

                unsigned int destinationParkIndex = 0;

                destinationParkIndex = inputDestinationParkStr[0] - '0';

                if (inputDestinationParkStr.size() != 1 || destinationParkIndex >= adjList.size()) {
                    std::cout
                        << std::endl
                        << "Error: input must be one character out of the valid options above" << std::endl
                        << std::endl
                        << "Press enter to continue..." << std::endl
                    ;
                    std::string dummyStr;
                    std::getline(
                        std::cin,
                        dummyStr,
                        '\n'
                    );
                    break;
                };

                std::cout
                    << std::endl
                    << conservationNetworkGraph.shortestPathToString(startParkIndex, destinationParkIndex) << std::endl
                    << std::endl
                    << "Press enter to continue..." << std::endl
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
                std::cout
                    << std::endl
                    << "Exiting program..." << std::endl
                ;
                running = false;
                break;
            };
            default: {
                status = "Error: input must be a character 1, 2, 3, 4 or 5";
                break;
            };
        };

    } while(running);

    return 0;

};

