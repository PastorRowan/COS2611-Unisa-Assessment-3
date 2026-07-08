# COS2611-Unisa-Assessment-3

## Wildlife Corridor Network System

### Project Overview

This C++ application models a conservation network of national parks connected by wildlife corridors. The program represents the network as a weighted graph and provides various analytical tools for exploring the connections between parks.

### Author Information
- **Name:** Rowan
- **Surname:** Van Zyl
- **Student** Number: 25939831
- **Module:** COS2611
- **Assignmnet:** Assessment 3

### Features

The program provides the following functionality through an interactive menu system:

1. **Display Conservation Network:** Shows all national parks and the wildlife corridors connecting them, including distances between parks
2. **Display Adjacency Matrix:** Presents the network as a matrix showing connections and distances between all parks
3. **Breadth-First Search (BFS) Traversal:** Performs a BFS traversal starting from a user-selected park
4. **Shortest Path Finder:** Uses Dijkstra's algorithm to find and display the shortest route between two parks
5. **Exit Program:** Gracefully terminates the application

### Technical Implementation

### Data Structures
- **NationalPark:** Structure containing park name, short name (2-letter code), and list of wildlife corridors
- **WildLifeCorridor:** Structure representing a connection to another park with a weight (distance in kilometers)
- **ConservationNetworkGraph:** Main class managing the graph as an adjacency list

### Algorithms Used
- **Breadth-First Search (BFS):** For graph traversal and path discovery
- **Dijkstra's Algorithm:** For finding the shortest path between two parks

### Key Methods
- isParkIndexValid(): Validates user input for park selection using regex
- toString(): Generates a human-readable representation of the entire network
- nationalParksToString(): Creates a tabular adjacency matrix display
- BFSPathToString(): Executes BFS traversal from a specified starting park
- shortestPathToString(): Computes and displays the shortest path between two parks

### Included National Parks

The network currently includes six national parks with their respective wildlife corridors:

1. Kruger (KR) - South Africa
2. Limpopo (LI) - Mozambique
3. Hwange (HW) - Zimbabwe
4. Chobe (CH) - Botswana
5. Etosha (ET) - Namibia
6. Kgalagadi (KG) - Botswana/South Africa

## Building and Running

### Prerequisites
- C++ compiler with C++11 or later support
- Standard Template Library (STL)
- I used the msys2 g++ compiler with default compiler flags for my testing. I cannot confirm whether other C++ compilers will produce the same results.

### Compilation
```
g++ 25939831_As3.cpp -o 25939831_As3.exe
```

### Execution
```
25939831_As3.exe
```

### Usage Guide
1. Run the program to see the main menu
2. Enter a number (1-5) to select an option
3. Follow the on-screen prompts for additional input when required
4. Press Enter to return to the main menu after viewing results
5. Select option 5 to exit the program

### Example Output
```
Programmer: Rowan Van Zyl, 25939831
Wildlife Corridor Network System
----------------------------------

1. Display conservation network
2. Display adjacency matrix
3. Perform BFS traversal
4. Find shortest path between parks
5. Exit

Status: Ok
```

### Error Handling

The program includes robust input validation:
- Menu selections are validated to ensure they are single characters (1-5)
- Park indices are validated using regex to ensure numeric input within range
- Error messages are displayed for invalid inputs
- The status indicator shows the current program state

### Code Quality Features
- **Const Correctness:** Methods and references use const where appropriate
- **Error Handling:** Comprehensive input validation and error messages
- **Cross-Platform Support:** Screen clearing works on Windows, Linux, and macOS
- **Documentation:** Comprehensive comments explaining each component
- **Memory Safety:** Use of STL containers and const references

### AI Assistance and Reflection
1. "How do I implement an adjacency list with weights in C++ for a graph?"
2. "Show me how to implement BFS traversal for a graph in C++"
3. "Can you help me implement Dijkstra's algorithm for finding the shortest path in a graph?"
4. "How can I generate and display an adjacency matrix from an adjacency list in C++?"
5. "What's the best way to create a menu-driven program in C++ with input validation?"
6. "How do I clear the screen in C++ for Windows, Linux and MacOS?"
7. "Can you show me how to use regular expressions in C++ for input validation?"

### Reflection

**What worked well:**
The AI-generated code for the graph data structure and BFS implementation worked very well. The adjacency list representation with weights was clear and easy to understand. The menu system structure provided by the AI was also helpful and served as a good foundation for the program.

**What did not work:**
The Dijkstra's algorithm implementation required significant debugging. The initial AI-generated version had issues with path reconstruction and didn't properly handle edge cases like disconnected graphs. The adjacency matrix generation code also needed modification to display distances correctly.

## Corrections Made:
1. Fixed the path reconstruction in Dijkstra's algorithm to correctly build the shortest path by following predecessor pointers
2. Added error handling for invalid park indices and unreachable destinations
3. Corrected the adjacency matrix formatting to properly display the table with tabs and park short names
4. Fixed the typo from "WildfLife" to "WildLife" throughout the code
5. Added the isParkIndexValid() function with regex validation for user input
6. Improved the BFS implementation to prevent infinite loops with visited tracking
7. Made the code const-correct by using const references where appropriate

The AI provided a solid foundation, but human oversight was essential for fixing bugs and ensuring the program correctly met all the assignment requirements.
