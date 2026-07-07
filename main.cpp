
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
struct WildfLifeCorridor;
struct NationalPark;
class ConservationNetworkGraph;

/*
clearScreen

Clears the terminal screen on Windows, Linux and MacOs
*/
void clearScreen() {
    /*
    If macro _WIN32 is defined then system cls command will run (program compiled on Windows computer)
    else the command system clear will run (program compiled on MacOs or Linux computer)
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

void waitForEnter() {

    std::cout << "Press enter to continue..." << std::endl;

    std::string dummyStr;
    std::getline(
        std::cin,
        dummyStr,
        '\n'
    );

};

/*
WildfLifeCorridor

Represents a wildlife corridor connecting one national park to another

to:
    Index of the destination national park in the adjacency list

weight:
    Distance between the 2 parks
*/
struct WildfLifeCorridor {
    const unsigned int to;
    const unsigned int weight;
};

// Used for convenience later
typedef std::vector<WildfLifeCorridor> WildfLifeCorridors;

/*
NationalPark

Represents a national park in the conservation network

name:
    Full name of the park

shortName:
    Name of the national park as the 2 1st letters capitalised

wildfLifeCorridors:
    List of all wildlife corridors between this national park and neighboring parks
*/
struct NationalPark {
    const std::string name;
    const std::string shortName;
    const WildfLifeCorridors wildfLifeCorridors;
};

/*
A list of national parks

Internally its a graph adjacency list
*/
typedef std::vector<NationalPark> NationalParks;

/*
ConservationNetworkGraph

Represents the entire conversation network as a graph

Each national park is represented by a NationalPark
Each wildlife corridor is represetned by a weighted edge

nationalParks:
    Stores the graph as an adjacency list
*/
class ConservationNetworkGraph {

    private:

        const NationalParks nationalParks;

    public:

        /*
        ConservationNetworkGraph constructor

        Parameters:
        nationalParksP - takes an adjacency list as input
        */ 
        ConservationNetworkGraph(
            const NationalParks nationalParksP
        ): nationalParks(nationalParksP) {};

        /*
        getNationalParks
        
        Returns a constant reference to the graph's adjacency list
        The returned reference cannot be used to modify the graph's adjacency list
        */
        const NationalParks& getNationalParks() {
            return nationalParks;
        };

        /*
        Returns a string to visually represent the entire conservation network (All national parks
        and wildlife corridors between them)
        */
        std::string toString() {

            // Build a human readable representation of the Conservation Network
            std::string output = "Conservation Network:\n\n";

            // Loop over all nodes in the adjacency list
            for (unsigned int i = 0; i < nationalParks.size(); ++i) {

                // Get current node data
                const NationalPark& currentNationalPark = nationalParks.at(i);
                const std::string& currentNationalParkName = currentNationalPark.name;
                const WildfLifeCorridors& currentNationalParkWildfLifeCorridors = currentNationalPark.wildfLifeCorridors;

                // Loop over the current node's edges
                for (unsigned int j = 0; j < currentNationalParkWildfLifeCorridors.size(); ++j) {

                    // Get current edge data
                    const WildfLifeCorridor& currentWildfLifeCorridor = currentNationalParkWildfLifeCorridors[j];
                    const unsigned int& currentNeighborIndex = currentWildfLifeCorridor.to;
                    const unsigned int& currentWildfLifeCorridorWeight = currentWildfLifeCorridor.weight;

                    // Get current neighbor data
                    const NationalPark& currentNeighbor = nationalParks.at(currentNeighborIndex);
                    const std::string& currentNeighborName = currentNeighbor.name;

                    // append edge data to console
                    output += currentNationalParkName + " -> " + currentNeighborName + " (" + std::to_string(currentWildfLifeCorridorWeight) + " km)\n";

                };

                output += '\n';

            };

            return output;

        };

        /*
        nationalParksToString

        Returns a string representation of the adjacency list of the conservation network as a table

        All national parks are represented using their short name

        Rows represents the starting park
        Columns represent the destination park

        The intersection of a row and column represents a wildlife corridor from the
        start park (row) to the destination park (column)

        A entry with a value of 0 means that a wilflife corridor does not exist
        A positive value represents the distance between the 2 parks (wildlife corridor exists)
        */
        std::string nationalParksToString() {

            // Build a human readable representation of the adjacency list
            std::string output = "";

            // Create the header row containing the short names of all parks
            for (unsigned int i = 0; i < nationalParks.size(); ++i) {
                const NationalPark& currentNationalPark = nationalParks.at(i);
                const std::string& currentNationalParkShortName = currentNationalPark.shortName;
                // Add park's short name to column
                output += '\t' + currentNationalParkShortName;
            };

            // Move console cursor to the 1st row of the table
            output += '\n';

            // Create one park row at a time by iterating over the nodes of the adajency matrix
            for (unsigned int i = 0; i < nationalParks.size(); ++i) {

                const unsigned int& currentNationalParkIndex = i;
                const NationalPark& currentNationalPark = nationalParks.at(currentNationalParkIndex);
                const std::string& currentNationalParkShortName = currentNationalPark.shortName;
                const WildfLifeCorridors& currentNationalParkWildfLifeCorridors = currentNationalPark.wildfLifeCorridors;

                // output the current nodes short name and move console cursor one tab space (next column in table)
                output += currentNationalParkShortName + '\t';

                // Create a row entry of the current park by checking whether an edge exists from it
                // to every other park
                for (unsigned int j = 0; j < nationalParks.size(); ++j) {

                    const unsigned int& currentOtherNationalParkIndex = j;

                    unsigned int weight = 0;

                    // Search the current park's edge list for a connection to the destination park
                    for (unsigned int k = 0; k < currentNationalParkWildfLifeCorridors.size(); ++k) {

                        const WildfLifeCorridor& edge = currentNationalParkWildfLifeCorridors.at(k);

                        // If edge from current park to column park exists
                        // then set the weight to the edges weight and break the loop
                        if (edge.to == currentOtherNationalParkIndex) {
                            weight = edge.weight;
                            break;
                        };

                    };

                    // Enter edge weight entry then move the cursor one tab space (next column in table)
                    output += std::to_string(weight) + '\t';

                };

                // Move console cursor to next row of the table
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
        before moving onto the neighbours of those parks, etc
        A visited list is maitained to ensure that each park is visited
        only once (Allowing infinite repeated visits would cause an infinite loop)

        Parameters:
        start - index of the starting park in the adjacency list
        */
        std::string BFSPathToString(
            const unsigned int& start
        ) {

            // Ensure the starting park exists in the graph
            // Return an error message if it does not
            if (start >= nationalParks.size()) {
                std::string errorMsg = "Start index '" + std::to_string(start) + "' is not from 0 to '" + std::to_string(nationalParks.size() - 1) + "' (inclusive)";
                return errorMsg;
            };

            // Queue of parks waiting to be visited
            std::queue<unsigned int> visiting;

            // Tracks whether each park has already been visited (prevent infinite loops)
            std::vector<bool> visited(nationalParks.size(), false);

            // Begin the BFS traversal from the starting park
            visiting.push(start);
            visited.at(start) = true;

            // Store the order in which parks are visited during the BFS traversal
            std::vector<unsigned int> visitedNationalParksIndicesOrder = {};
            visitedNationalParksIndicesOrder.reserve(nationalParks.size());

            // Continue traversing until there are no more parks left in the queue to visit
            while (!visiting.empty()) {

                const unsigned int& currentNationalParkIndex = visiting.front();
                const NationalPark& currentNationalPark = nationalParks.at(currentNationalParkIndex);
                const WildfLifeCorridors& currentWildfLifeCorridors = currentNationalPark.wildfLifeCorridors;

                // Remove the visited park from the queue
                visiting.pop();

                // Record the current park in the visitation order
                visitedNationalParksIndicesOrder.push_back(currentNationalParkIndex);

                // Queue a visit to each univisited neighbouring park
                for (unsigned int i = 0; i < currentWildfLifeCorridors.size(); ++i) {

                    const WildfLifeCorridor& currentWildfLifeCorridor = currentWildfLifeCorridors.at(i);
                    const unsigned int& currentNeighborIndex = currentWildfLifeCorridor.to;

                    // Add the neighbour to the queue and mark it as visited
                    if (!visited.at(currentNeighborIndex)) {
                        visiting.push(currentNeighborIndex);
                        visited.at(currentNeighborIndex) = true;
                    };

                };

            };

            // Build a human readable representation of the BFS traversal
            std::string output = "";

            // Convert the visitation order into a string separated by arrows
            for (unsigned int i = 0; i < visitedNationalParksIndicesOrder.size(); ++i) {
                const unsigned int& currentNationalParkIndex = visitedNationalParksIndicesOrder.at(i);
                const NationalPark& currentNationalPark = nationalParks.at(currentNationalParkIndex);
                const std::string& currentParkName = currentNationalPark.name;

                // Add park name
                output += currentParkName + (i ==  visitedNationalParksIndicesOrder.size() - 1 ? "" : " -> ");
            };

            return output;

        };

        /*
        shortestPathToString

        Returns a string representing the shortest path from one park to another

        Uses Dijkstra's algorithm

        Parameters:
        from - index of the starting park in the adjacency list
        to - index of the destination park in the adjacency list

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

            if (from >= nationalParks.size()) {
                std::string errorMsg = "from index '" + std::to_string(from) + "' is not from 0 to '" + std::to_string(nationalParks.size() - 1) + "' (inclusive)";
                return errorMsg;
            };

            if (to >= nationalParks.size()) {
                std::string errorMsg = "to index '" + std::to_string(to) + "' is not from 0 to '" + std::to_string(nationalParks.size() - 1) + "' (inclusive)";
                return errorMsg;
            };

            const unsigned int& INF = UINT_MAX;
            const unsigned int& NULL_PREV = UINT_MAX;

            std::vector<unsigned int> queue;
            queue.reserve(nationalParks.size());

            unsigned int dist[nationalParks.size()] = {};
            unsigned int prev[nationalParks.size()] = {};

            for (unsigned int i = 0; i < nationalParks.size(); ++i) {
                const unsigned int& currentNationalParkIndex = i;
                dist[currentNationalParkIndex] = INF;
                prev[currentNationalParkIndex] = NULL_PREV;
                queue.push_back(currentNationalParkIndex);
            };

            dist[from] = 0;

            while (!queue.empty()) {
                unsigned int closestNationalParkIndex = queue.front();
                for (unsigned int i = 0; i < queue.size(); ++i) {
                    const unsigned int& currentNationalParkIndex = queue.at(i);
                    const unsigned int& currentDistance = dist[currentNationalParkIndex];
                    if (dist[currentNationalParkIndex] < dist[closestNationalParkIndex]) {
                        closestNationalParkIndex = currentNationalParkIndex;
                    };
                };

                auto it = std::find(queue.begin(), queue.end(), closestNationalParkIndex);

                if (it != queue.end()) {
                    queue.erase(it);
                };

                const NationalPark& closestNationalPark = nationalParks.at(closestNationalParkIndex);
                const WildfLifeCorridors& wildfLifeCorridors = closestNationalPark.wildfLifeCorridors;

                for (unsigned int i = 0; i < wildfLifeCorridors.size(); ++i) {
                    const unsigned int& currentWildfLifeCorridorIndex = i;
                    const WildfLifeCorridor& currentWildfLifeCorridor = wildfLifeCorridors.at(currentWildfLifeCorridorIndex);
                    const unsigned int& currentNeighborIndex = currentWildfLifeCorridor.to;
                    const unsigned int& distanceFromCurrentNationalParkToNeighbor = currentWildfLifeCorridor.weight;
                    const unsigned int alternativeDistanceFromCurrentNationalParkToNeighbor = dist[closestNationalParkIndex] + distanceFromCurrentNationalParkToNeighbor;
                    if (alternativeDistanceFromCurrentNationalParkToNeighbor < dist[currentNeighborIndex]) {
                        dist[currentNeighborIndex] = alternativeDistanceFromCurrentNationalParkToNeighbor;
                        prev[currentNeighborIndex] = closestNationalParkIndex;
                    };
                };

            };

            std::vector<unsigned int> path = {};
            path.reserve(nationalParks.size());

            unsigned int current = to;

            if (prev[current] != NULL_PREV || current == from) {
                while (current != from) {
                    path.push_back(current);
                    current = prev[current];
                };
                path.push_back(from);
            };

            std::string output = "";

            const NationalPark& fromNationalPark = nationalParks.at(from);
            const NationalPark& toNationalPark = nationalParks.at(to);

            output += "shortest route from " + fromNationalPark.name + " to " + toNationalPark.name + "\n\n";

            for (unsigned int i = path.size() - 1; i < path.size(); --i) {
                const unsigned int& currentNationalParkIndex = path.at(i);
                const NationalPark& currentNationalPark = nationalParks.at(currentNationalParkIndex);
                const std::string& currentNationalParkName = currentNationalPark.name;
                output += currentNationalParkName + (i == 0 ? "" : " -> ");
            };

            output += "\n\n" + std::string("Total distance: ") + std::to_string(dist[to]) + "km";

            return output;

        };

};

/*
Main program loop

Repeatedly displays the main menu and waits for user input until
the user chooses to exit the program.

Each iteration:
    1. Clears the screen and displays the available menu options
    2. Displays the current program status message
    3. Receives and validates the user's menu selection
    4. Executes the corresponding operation using a switch statement

Available operations:
    1. Display the conservation network and all wildlife corridors
    2. Display the conservation network as an adjacency matrix
    3. Perform a breadth-first search (BFS) traversal from a selected park
    4. Find the shortest route between two parks using Dijkstra's algorithm
    5. Exit the program

The loop continues while the running flag remains true.
Selecting option 5 changes running to false, causing the program to terminate.
*/
int main() {

    /*
    Intialise conservationNetworkGraph to have all of the national parks
    and wildlife corridors
    */
    ConservationNetworkGraph conservationNetworkGraph({
        /*
        Kruger National Park
        Index = 0
        */
        {
            .name = "Kruger",
            .shortName = "KR",
            .wildfLifeCorridors = {
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
        Limpopo National Park
        Index = 1
        */
        {
            .name = "Limpopo",
            .shortName = "LI",
            .wildfLifeCorridors = {
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
            .wildfLifeCorridors = {
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
        Chobe National Park
        Index = 3
        */
        {
            .name = "Chobe",
            .shortName = "CH",
            .wildfLifeCorridors = {
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
        Etosha National Park
        Index = 4
        */
        {
            .name = "Etosha",
            .shortName = "ET",
            .wildfLifeCorridors = {
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
        Kgalagadi Transfrontier Park
        Index = 5
        */
        {
            .name = "Kgalagadi",
            .shortName = "KG",
            .wildfLifeCorridors = {
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

    // Header of every page
    const std::string HEADER = R"(
Programmer: Rowan Van Zyl, 25939831
Wildlife Corridor Network System
----------------------------------

)";

    // Default status message displayed in the menu
    const char DEFAULT_STATUS[] = "Ok";

    // Stores the user's raw input from the keyboard
    std::string input_str = "";

    // Stores the first character of the user's menu selection
    char input_char = ' ';

    // Current status message displayed to the user
    // (e.g. "Ok" or an error message)
    std::string status = DEFAULT_STATUS;

    // Controls the main program loop
    // The program continues running while this value is true and exits when false
    bool running  = true;

    do {

        // Clears the screen and prints the home menu
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

        // Resets status after every user enter
        status = DEFAULT_STATUS;

        // Stores home menu user input in input_str
        std::getline(
            std::cin,
            input_str,
            '\n'
        );

        // Shows an error message if the input is not only 1 character
        if (input_str.size() != 1) {
            status = "Error: input must be only 1 character";
            continue;
        };

        // Extract menu selection
        input_char = input_str[0];

        // Show different screens depending on user input
        switch (input_char) {
            // Displays the entire conservation network to user
            case '1': {
                clearScreen();
                std::cout
                    << HEADER
                    << conservationNetworkGraph.toString()
                ;
                waitForEnter();
                break;
            };
            // Displays the conservation networks adjacency matrix as a table to user
            // Consists of national parks and their wild life corridors
            case '2': {
                clearScreen();
                std::cout
                    << HEADER
                    << conservationNetworkGraph.nationalParksToString()
                ;
                waitForEnter();
                break;
            };
            // Displays a BFS traversal starting from a user inputted starting park index
            case '3': {

                // Clears screen and displays header with 'Parks:' heading
                clearScreen();
                std::cout
                    << HEADER
                    << "Parks:" << std::endl
                ;

                const NationalParks& nationalParks = conservationNetworkGraph.getNationalParks();

                // Displays all available national park indices to choose from
                for (unsigned int i = 0; i < nationalParks.size(); ++i) {
                    const NationalPark& currentNationalPark = nationalParks.at(i);
                    std::cout << i << ": " << currentNationalPark.name << std::endl;
                };

                std::cout
                    << std::endl
                    << "Start location: "
                ;

                // Stores user start park index input
                std::string inputStartParkStr = "";

                // Collects input
                std::getline(
                    std::cin,
                    inputStartParkStr,
                    '\n'
                );

                if (inputStartParkStr.size() != 1) {
                    std::cout
                        << std::endl
                        << "Error: input must be only one character" << std::endl
                        << std::endl
                    ;
                    waitForEnter();
                    break;
                };

                unsigned int startParkIndex = 0;

                // Converts 0-9 asci character to 0-9 unsigned int
                startParkIndex = inputStartParkStr[0] - '0';

                // Displays BFS traversal or error emssage if starting park index is out of range
                std::cout
                    << std::endl
                    << conservationNetworkGraph.BFSPathToString(startParkIndex) << std::endl
                    << std::endl
                ;
                waitForEnter();
                break;
            };
            // Displays the shortest path from a user inputted starting park index and a user inputted destination park index
            case '4': {

                // Clears screen and displays header with 'Parks:' heading
                clearScreen();
                std::cout
                    << HEADER
                    << "Parks:" << std::endl
                ;

                const NationalParks& nationalParks = conservationNetworkGraph.getNationalParks();

                // Displays all available national park indices to choose from
                for (unsigned int i = 0; i < nationalParks.size(); ++i) {
                    const NationalPark& currentNationalPark = nationalParks.at(i);
                    std::cout << i << " - " << currentNationalPark.name << std::endl;
                };

                std::cout
                    << std::endl
                    << "Start location: "
                ;

                // Stores user start park index input
                std::string inputStartParkStr = "";

                // Collects input
                std::getline(
                    std::cin,
                    inputStartParkStr,
                    '\n'
                );

                // inputStartParkStr

                unsigned int startParkIndex = 0;

                startParkIndex = inputStartParkStr[0] - '0';

                if (inputStartParkStr.size() != 1 || startParkIndex >= nationalParks.size()) {
                    std::cout
                        << std::endl
                        << "Error: input must be one character out of the valid options above" << std::endl
                        << std::endl
                    ;
                    waitForEnter();
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

                if (inputDestinationParkStr.size() != 1 || destinationParkIndex >= nationalParks.size()) {
                    std::cout
                        << std::endl
                        << "Error: input must be one character out of the valid options above" << std::endl
                        << std::endl
                    ;
                    waitForEnter();
                    break;
                };

                std::cout
                    << std::endl
                    << conservationNetworkGraph.shortestPathToString(startParkIndex, destinationParkIndex) << std::endl
                    << std::endl
                ;
                waitForEnter();
                break;
            };
            // Displays teh shorte
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
