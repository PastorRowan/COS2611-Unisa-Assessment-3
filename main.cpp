
/*
Name: Rowan
Surname: Van Zyl
Student Number: 25939831
Module: COS2611
Assignment: Assessment 3

Please see my github repo for more info: https://github.com/PastorRowan/COS2611-Unisa-Assessment-3
*/

#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <string>
#include <limits>
#include <algorithm>
#include <regex>

// Helper function Declarations
void clearScreen();
std::string getUserInput();
void waitForEnter();

/*
WildLifeCorridor

Represents a wildlife corridor connecting one national park to another

Properties:
to - Index of the destination national park in the adjacency list
weight - Distance between the 2 parks
*/
struct WildLifeCorridor {
    const unsigned int to;
    const unsigned int weight;
};

// List of wild life corridors
typedef std::vector<WildLifeCorridor> WildLifeCorridors;

/*
NationalPark

Represents a national park in the conservation network

Properties:
name - Full name of the park
shortName - Name of the national park as the 2 1st letters capitalised
WildLifeCorridors - List of all wildlife corridors between this national park and neighboring parks
*/
struct NationalPark {
    const std::string name;
    const std::string shortName;
    const WildLifeCorridors WildLifeCorridors;
};

// List of national parks
typedef std::vector<NationalPark> NationalParks;

/*
ConservationNetworkGraph

Represents the entire conversation network as a graph

Each national park is represented by a NationalPark
Each wildlife corridor is represetned by a weighted edge

Properties:
nationalParks - Stores the graph as an adjacency list
*/
class ConservationNetworkGraph {

    private:

        const NationalParks nationalParks;

    public:

        /*
        ConservationNetworkGraph

        Represents the entire conversation network as a graph

        Each national park is represented by a NationalPark
        Each wildlife corridor is represetned by a weighted edge

        nationalParks:
            Stores the graph as an adjacency list
        */
        ConservationNetworkGraph(
            const NationalParks nationalParksP
        ): nationalParks(nationalParksP) {};

        /*
        getNationalParks

        Returns a constant reference to the graph's adjacency list
        The returned reference cannot be used to modify the graph's adjacency list
        */
        const NationalParks& getNationalParks() const {
            return nationalParks;
        };

        // Returns true if park index string is valid, else returns false
        bool isParkIndexValid (
            const std::string& parkIndexString
        ) const;

        // Returns a string representation of the conservation network graph
        std::string toString() const;

        // Returns a string representation of the national parks
        std::string nationalParksToString() const;

        // Returns a string representation of the route taken from a breadth first search (BFS) starting at the starting park
        std::string BFSPathToString(
            const unsigned int& start
        ) const;

        // Returns a string represenetaiton of the shortest path between 2 parks
        std::string shortestPathToString(
            const unsigned int& from,
            const unsigned int& to
        ) const;

};

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

/*
getUserInput

Waits for the user to complete a line of input into the console

Returns the complete line as a string
*/
std::string getUserInput() {

    std::string userInputString = "";

    std::getline(
        std::cin,
        userInputString,
        '\n'
    );

    return userInputString;

};

/*
waitForEnter

Prompts user to press enter to continue and pauses program execution until the user presses enter on their keyboard
*/
void waitForEnter() {

    // Prompt
    std::cout << "Press enter to continue..." << std::endl;

    std::string dummyStr = "";
    std::getline(
        std::cin,
        dummyStr,
        '\n'
    );

};

/*
ConservationNetworkGraph::isParkIndexValid

Returns true if park index string is valid, else returns false

Parameters:
parkIndexString - park index string to test validity of
*/
bool ConservationNetworkGraph::isParkIndexValid (
    const std::string& parkIndexString
) const {

    // Regular expression that only matches strings containing numbers in the range of 0-999
    static const std::regex onlyDigitsRegEx("^[0-9]{1,3}$");

    // If regex does not match then return false
    if (!std::regex_match(parkIndexString, onlyDigitsRegEx)) {
        return false;
    };

    // Convert park index string into an index for comparison
    const unsigned long parkIndexAsUnsignedInt = std::stoul(parkIndexString);

    // If park index is greater than the size of national parks then return false
    if (parkIndexAsUnsignedInt >= nationalParks.size()) {
        return false;
    };

    // Else return true
    return true;

};

/*
Returns a string to visually represent the entire conservation network (All national parks
and wildlife corridors between them)
*/
std::string ConservationNetworkGraph::toString() const {

    // Build a human readable representation of the Conservation Network
    std::string output = "Conservation Network:\n\n";

    // Loop over all nodes in the adjacency list
    for (unsigned int i = 0; i < nationalParks.size(); ++i) {

        // Get current node data
        const NationalPark& currentNationalPark = nationalParks.at(i);
        const std::string& currentNationalParkName = currentNationalPark.name;
        const WildLifeCorridors& currentNationalParkWildLifeCorridors = currentNationalPark.WildLifeCorridors;

        // Loop over the current node's edges
        for (unsigned int j = 0; j < currentNationalParkWildLifeCorridors.size(); ++j) {

            // Get current edge data
            const WildLifeCorridor& currentWildLifeCorridor = currentNationalParkWildLifeCorridors[j];
            const unsigned int& currentNeighborIndex = currentWildLifeCorridor.to;
            const unsigned int& currentWildLifeCorridorWeight = currentWildLifeCorridor.weight;

            // Get current neighbor data
            const NationalPark& currentNeighbor = nationalParks.at(currentNeighborIndex);
            const std::string& currentNeighborName = currentNeighbor.name;

            // append edge data to console
            output += currentNationalParkName + " -> " + currentNeighborName + " (" + std::to_string(currentWildLifeCorridorWeight) + " km)\n";

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
std::string ConservationNetworkGraph::nationalParksToString() const {

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
        const WildLifeCorridors& currentNationalParkWildLifeCorridors = currentNationalPark.WildLifeCorridors;

        // output the current nodes short name and move console cursor one tab space (next column in table)
        output += currentNationalParkShortName + '\t';

        // Create a row entry of the current park by checking whether an edge exists from it
        // to every other park
        for (unsigned int j = 0; j < nationalParks.size(); ++j) {

            const unsigned int& currentOtherNationalParkIndex = j;

            unsigned int weight = 0;

            // Search the current park's edge list for a connection to the destination park
            for (unsigned int k = 0; k < currentNationalParkWildLifeCorridors.size(); ++k) {

                const WildLifeCorridor& edge = currentNationalParkWildLifeCorridors.at(k);

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
std::string ConservationNetworkGraph::BFSPathToString(
    const unsigned int& start
) const {

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
        const WildLifeCorridors& currentWildLifeCorridors = currentNationalPark.WildLifeCorridors;

        // Remove the visited park from the queue
        visiting.pop();

        // Record the current park in the visitation order
        visitedNationalParksIndicesOrder.push_back(currentNationalParkIndex);

        // Queue a visit to each univisited neighbouring park
        for (unsigned int i = 0; i < currentWildLifeCorridors.size(); ++i) {

            const WildLifeCorridor& currentWildLifeCorridor = currentWildLifeCorridors.at(i);
            const unsigned int& currentNeighborIndex = currentWildLifeCorridor.to;

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
*/
std::string ConservationNetworkGraph::shortestPathToString(
    const unsigned int& from,
    const unsigned int& to
) const {

    // Ensure the starting park index is valid
    if (from >= nationalParks.size()) {
        std::string errorMsg = "from index '" + std::to_string(from) + "' is not from 0 to '" + std::to_string(nationalParks.size() - 1) + "' (inclusive)";
        return errorMsg;
    };

    // Ensure the starting park index is valid
    if (to >= nationalParks.size()) {
        std::string errorMsg = "to index '" + std::to_string(to) + "' is not from 0 to '" + std::to_string(nationalParks.size() - 1) + "' (inclusive)";
        return errorMsg;
    };

    // Value representing an infinite distance
    static const unsigned int INF = UINT_MAX;

    // Sentinel value indicating that a park has no predecessor
    static const unsigned int NULL_PREV = UINT_MAX;

    // List of parks that have not yet been processed
    std::vector<unsigned int> queue;
    queue.reserve(nationalParks.size());

    // Stores the shortest known distance from the start park to each park
    std::vector<unsigned int> dist(nationalParks.size(), INF);

    // Stores the predecessor of each park in the shortest path
    std::vector<unsigned int> prev(nationalParks.size(), NULL_PREV);

    // Initially place every park into the unvisited queue
    for (unsigned int i = 0; i < nationalParks.size(); ++i) {
        const unsigned int& currentNationalParkIndex = i;
        queue.push_back(currentNationalParkIndex);
    };

    // The distance from the start park to itself is zero
    dist.at(from) = 0;

    // Continue until every park has been processed
    while (!queue.empty()) {

        // Find the unvisited park with the smallest known distance
        unsigned int closestNationalParkIndex = queue.front();
        for (unsigned int i = 0; i < queue.size(); ++i) {
            const unsigned int& currentNationalParkIndex = queue.at(i);
            const unsigned int& currentDistance = dist.at(currentNationalParkIndex);
            if (dist.at(currentNationalParkIndex) < dist.at(closestNationalParkIndex)) {
                closestNationalParkIndex = currentNationalParkIndex;
            };
        };

        // Remove the selected park from the unvisited queue
        auto it = std::find(queue.begin(), queue.end(), closestNationalParkIndex);
        if (it != queue.end()) {
            queue.erase(it);
        };

        // Skip if distance is INF (no path to this node)
        if (dist.at(closestNationalParkIndex) == INF) {
            continue;
        };

        const NationalPark& closestNationalPark = nationalParks.at(closestNationalParkIndex);
        const WildLifeCorridors& WildLifeCorridors = closestNationalPark.WildLifeCorridors;

        // Attempt to improve the shortest known distance to each neighbouring park
        for (unsigned int i = 0; i < WildLifeCorridors.size(); ++i) {

            const unsigned int& currentWildLifeCorridorIndex = i;
            const WildLifeCorridor& currentWildLifeCorridor = WildLifeCorridors.at(currentWildLifeCorridorIndex);

            const unsigned int& currentNeighborIndex = currentWildLifeCorridor.to;
            const unsigned int& distanceFromCurrentNationalParkToNeighbor = currentWildLifeCorridor.weight;

            // Distance to the neighbour if travelling through the current park
            const unsigned int alternativeDistance = dist.at(closestNationalParkIndex) + distanceFromCurrentNationalParkToNeighbor;

            // Update the neighbour's distance and predecessor if a shorter route was found
            if (alternativeDistance < dist.at(currentNeighborIndex)) {
                dist.at(currentNeighborIndex) = alternativeDistance;
                prev.at(currentNeighborIndex) = closestNationalParkIndex;
            };
        };

    };

    // Reconstruct the shortest path by following predecessor links backwards
    std::vector<unsigned int> path = {};
    path.reserve(nationalParks.size());

    unsigned int current = to;

    if (prev.at(current) != NULL_PREV || current == from) {
        while (current != from) {
            path.push_back(current);
            current = prev.at(current);
        };
        // Add the starting park
        path.push_back(from);
    };

    // Build a human readable representation of the shortest path
    std::string output = "";

    const NationalPark& fromNationalPark = nationalParks.at(from);
    const NationalPark& toNationalPark = nationalParks.at(to);

    output += "shortest route from " + fromNationalPark.name + " to " + toNationalPark.name + "\n\n";

    // Report when no path exists between the selected parks
    if (path.empty()) {
        output += "No path exists between " + fromNationalPark.name + " and " + toNationalPark.name;
        return output;
    };

    // Append each park in the path from start to destination
    for (unsigned int i = path.size() - 1; i < path.size(); --i) {

        const unsigned int& currentNationalParkIndex = path.at(i);
        const NationalPark& currentNationalPark = nationalParks.at(currentNationalParkIndex);
        const std::string& currentNationalParkName = currentNationalPark.name;

        output += currentNationalParkName + (i == 0 ? "" : " -> ");
    };

    // Show the total length of the shortest path
    output += "\n\n" + std::string("Total distance: ") + std::to_string(dist.at(to)) + "km";

    return output;

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
            .WildLifeCorridors = {
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
            .WildLifeCorridors = {
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
            .WildLifeCorridors = {
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
            .WildLifeCorridors = {
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
            .WildLifeCorridors = {
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
            .WildLifeCorridors = {
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
                5 -> 2
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
    const std::string DEFAULT_STATUS = "Ok";

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
        input_str = getUserInput();

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

                // Stores user inputted park index
                const std::string startParkIndexStr = getUserInput();

                if (!conservationNetworkGraph.isParkIndexValid(startParkIndexStr)) {
                    std::cout
                        << std::endl
                        << "Error: input start park index is not a valid option, returning to the home menu" << std::endl
                        << std::endl
                    ;
                    waitForEnter();
                    break;
                };

                const unsigned int startParkIndex = std::stoul(startParkIndexStr);

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

                // Stores user inputted start park index
                const std::string startParkIndexStr = getUserInput();

                if (!conservationNetworkGraph.isParkIndexValid(startParkIndexStr)) {
                    std::cout
                        << std::endl
                        << "Error: input start park index is not a valid option, returning to the home menu" << std::endl
                        << std::endl
                    ;
                    waitForEnter();
                    break;
                };

                // Converts start park index string to numeric index
                const unsigned int startParkIndex = std::stoul(startParkIndexStr);

                std::cout
                    << "Destination location: "
                ;

                // Stores user inputted destination park index
                const std::string destinationParkIndexStr = getUserInput();

                if (!conservationNetworkGraph.isParkIndexValid(destinationParkIndexStr)) {
                    std::cout
                        << std::endl
                        << "Error: input destination park index is not a valid option, returning to the home menu" << std::endl
                        << std::endl
                    ;
                    waitForEnter();
                    break;
                };

                // Converts destination park index string to numeric index
                const unsigned int destinationParkIndex = std::stoul(destinationParkIndexStr);

                std::cout
                    << std::endl
                    << conservationNetworkGraph.shortestPathToString(startParkIndex, destinationParkIndex) << std::endl
                    << std::endl
                ;
                waitForEnter();
                break;
            };
            // Exits the program and informs user
            case '5': {
                std::cout
                    << std::endl
                    << "Exiting program..." << std::endl
                ;
                running = false;
                break;
            };
            // Displays an error message if the entered menu option is invalid
            default: {
                status = "Error: input must be a character 1, 2, 3, 4 or 5";
                break;
            };
        };

    // Exits the main loop if running flag is false
    } while(running);

    // Returns 0 to indicate a graceful exit
    return 0;

};

/*
GenAI Prompts Used:

1. "How do I implement an adjacency list with weights in C++ for a graph?"
2. "Show me how to implement BFS traversal for a graph in C++"
3. "Can you help me implement Dijkstra's algorithm for finding the shortest path in a graph?"
4. "How can I generate and display an adjacency matrix from an adjacency list in C++?"
5. "What's the best way to create a menu-driven program in C++ with input validation?"
6. "How do I clear the screen in C++ for Windows, Linux and MacOS?"
7. "Can you show me how to use regular expressions in C++ for input validation?"

Reflection:

What worked well:
The AI-generated code for the graph data structure and BFS implementation worked
very well. The adjacency list representation with weights was clear and easy to
understand. The menu system structure provided by the AI was also helpful and
served as a good foundation for the program.

What did not work:
The Dijkstra's algorithm implementation required significant debugging. The
initial AI-generated version had issues with path reconstruction and didn't
properly handle edge cases like disconnected graphs. The adjacency matrix
generation code also needed modification to display distances correctly.

What corrections I made:
1. Fixed the path reconstruction in Dijkstra's algorithm to correctly build the 
shortest path by following predecessor pointers
2. Added error handling for invalid park indices and unreachable destinations
3. Corrected the adjacency matrix formatting to properly display the table with
 tabs and park short names
4. Fixed the typo from "WildfLife" to "WildLife" throughout the code
5. Added the isParkIndexValid() function with regex validation for user input
6. Improved the BFS implementation to prevent infinite loops with visited tracking
7. Made the code const-correct by using const references where appropriate

The AI provided a solid foundation, but human oversight was essential for fixing
bugs and ensuring the program correctly met all the assignment requirements.
*/
