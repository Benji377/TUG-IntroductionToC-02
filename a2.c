
#include <stdio.h>
#include <stdlib.h>

// Structs
struct Guest {
    int currentFloor;
    int destinationFloor;
    int isInsideElevator; // 1 for true, 0 for false
    int hasReachedDestination; // 1 for true, 0 for false
};

struct Elevator {
    int capacity;
    int currentFloor;
    int direction; // 1 for up, -1 for down
    struct Guest* guests;
};

// Function prototypes
void printHotel(char* hotelName, int numFloors, int numElevators, int elevatorCapacity, struct Guest** guests, const int* numGuests, struct Elevator** elevators);
void simulationStep(char* hotelName, int numFloors, int numElevators, int elevatorCapacity, struct Guest** guests, const int* numGuests, struct Elevator** elevators);
int areStringsEqual(const char* str1, const char* str2);
int stringLength(const char* str);
char** splitStringByComma(char* str);
int allGuestsHaveReachedDestination(struct Guest** guests, const int* numGuests);

int main() {
    char hotelName[50];
    int numFloors;
    int numElevators;
    int elevatorCapacity;
    int numPeopleWaiting;

    // Welcome message
    printf("____________________\n"
           " ____   ____   ____ \n"
           "|____  |____  |____|\n"
           "|____   ____| |     \n"
           "____________________\n\n"
           "Welcome to the\n"
           "Elevator Simulation Program!\n\n");

    // User inputs
    printf("Enter the name of the hotel:\n > ");
    scanf("%s", hotelName);


    do {
        printf("Enter the number of floors in hotel %s:\n > ", hotelName);
        scanf("%d", &numFloors);
        if (numFloors < 3 || numFloors > 10) {
            printf("Wrong input, the number of floors must be between 3 and 10!\n");
        }
    } while (numFloors < 3 || numFloors > 10);


    do {
        printf("Enter the number of elevators in hotel %s:\n > ", hotelName);
        scanf("%d", &numElevators);
        if (numElevators < 1 || numElevators > 5) {
            printf("Wrong input, the number of elevators must be between 1 and 5!\n");
        }
    } while (numElevators < 1 || numElevators > 5);


    do {
        printf("Enter the capacity of elevators in hotel %s:\n > ", hotelName);
        scanf("%d", &elevatorCapacity);
        if (elevatorCapacity < 1 || elevatorCapacity > 9) {
            printf("Wrong input, the capacity of elevators must be between 1 and 9 person(s)!\n");
        }
    } while (elevatorCapacity < 1 || elevatorCapacity > 9);


    do {
        printf("Enter the number of people waiting on each floor:\n > ");
        scanf("%d", &numPeopleWaiting);
        if (numPeopleWaiting < 2 || numPeopleWaiting > 20) {
            printf("Wrong input, the number of people waiting on each floor must be between 2 and 20!\n");
        }
    } while (numPeopleWaiting < 2 || numPeopleWaiting > 20);


    struct Guest** guests = (struct Guest**)malloc(numFloors * sizeof(struct Guest*));
    struct Elevator** elevators = (struct Elevator**)malloc(numElevators * sizeof(struct Elevator*));
    char*** guestsDestinations = (char***)malloc(numFloors * sizeof(char**));
    int* numGuests = (int*)malloc(numFloors * sizeof(int));

    for (int i = 0; i < numFloors; ++i) {
        guests[i] = (struct Guest*)malloc(numPeopleWaiting * sizeof(struct Guest));
        printf("Enter the destination floors of the people [floor: %d]:\n > ", i);

        char* input = (char*)malloc(100 * sizeof(char));
        scanf("%s", input);
        printf("Input: %s\n", input);
        char** splitInput = splitStringByComma(input);
        guestsDestinations[i] = splitInput;

        // For each guestDestination, create a Guest object and add it to the guests array
        for (int j = 0; j < numPeopleWaiting; ++j) {
            struct Guest *guest = (struct Guest*)malloc(sizeof(struct Guest));
            guest->currentFloor = i;
            guest->destinationFloor = atoi(splitInput[j]);
            guest->isInsideElevator = 0;
            guests[i][j] = *guest;
            printf("Guest %d-%d: %d\n", i, j, guests[i][j].destinationFloor);
            // increase the amount of guests on the current floor
            ++numGuests[i];
        }
    }

    // Initialize Elevator struct
    for (int i = 0; i < numElevators; ++i) {
        struct Elevator *elevator = (struct Elevator*)malloc(sizeof(struct Elevator));
        elevator->capacity = elevatorCapacity;
        // If i is an odd number, start at the top floor and go down, else start at the bottom floor and go up
        if (i % 2 != 0) {
            elevator->currentFloor = numFloors - 1;
            elevator->direction = -1;
        } else {
            elevator->currentFloor = 0;
            elevator->direction = 1;
        }
        elevator->guests = (struct Guest*)malloc(elevatorCapacity * sizeof(struct Guest));
        // Initialize all guests in the elevator to -1, which means that they are not inside the elevator
        for (int j = 0; j < elevatorCapacity; ++j) {
            struct Guest *guest = (struct Guest*)malloc(sizeof(struct Guest));
            guest->currentFloor = -1;
            guest->destinationFloor = -1;
            guest->isInsideElevator = 0;
            guest->hasReachedDestination = 0;
            elevator->guests[j] = *guest;
        }

        elevators[i] = elevator;
        printf("Elevator %d: %d\n", i, elevators[i]->capacity);
    }


    // Show initial state
    printf("Show the initial state? (\"yes\"/\"no\"):\n > ");
    char showInitialState[5];
    scanf("%s", showInitialState);
    if (areStringsEqual(showInitialState, "yes")) {
        printf("\n=================\n  INITIAL STATE\n=================\n");
        printHotel(hotelName, numFloors, numElevators, elevatorCapacity, guests, numGuests, elevators);
    }

    // Start simulation
    printf("Start the simulation? (\"start\"/\"skip\"):\n > ");
    char startSimulation[10];
    scanf("%s", startSimulation);
    if (areStringsEqual(startSimulation, "start")) {
        int printSteps = 0;
        while (1) {
            // Show all steps or end result
            printf("Show all steps of the simulation? (\"all steps\"/\"end result\"):\n > ");
            char showAllSteps[20];

            scanf(" %[^\n]s", showAllSteps);
            printf("Input: %s\n", showAllSteps);

            if (areStringsEqual(showAllSteps, "all steps")) {
                printSteps = 1;
                break;
            } else if (areStringsEqual(showAllSteps, "end result")) {
                break;
            } else {
                printf("Invalid input. Please enter either \"all steps\" or \"end result\".\n");
            }
        }


        // As long as not all guests have reached their destination, keep simulating
        while (!allGuestsHaveReachedDestination(guests, numGuests)) {
            simulationStep(hotelName, numFloors, numElevators, elevatorCapacity, guests, numGuests, elevators);
            if (printSteps == 1) {
                printHotel(hotelName, numFloors, numElevators, elevatorCapacity, guests, numGuests, elevators);
            }
        }
        printHotel(hotelName, numFloors, numElevators, elevatorCapacity, guests, numGuests, elevators);

        // Free allocated memory
        for (int i = 0; i < numFloors; ++i) {
            free(guests[i]);
        }
        free(guests);

        for (int i = 0; i < numElevators; ++i) {
            free(elevators[i]);
        }
        free(elevators);

        free(numGuests);
        free(guestsDestinations);

        return 0;

    } else {
        // Free allocated memory
        for (int i = 0; i < numFloors; ++i) {
            free(guests[i]);
        }
        free(guests);

        for (int i = 0; i < numElevators; ++i) {
            free(elevators[i]);
        }
        free(elevators);

        free(numGuests);
        free(guestsDestinations);

        printf("Simulation not started.\n");
        return 0;
    }
}

void printHotel(char* hotelName, int numFloors, int numElevators, int elevatorCapacity, struct Guest** guests, const int* numGuests, struct Elevator** elevators) {
    printf("\n++-----------++\n");
    printf("++   %s   ++\n", hotelName);
    printf("++-----+-----++\n");

    // For each floor, print the floor layout and if any of the elevators is at that floor number, print the elevator
    // The elevators have a number, which means that if the 3rd elevator is at the top floor, it should be printed in the 3rd column
    for (int i = numFloors - 1; i >= 0; --i) {
        // For each floor, check if any elevator is at the current floor and print its number
        printf("|");
        for (int j = 0; j < numElevators; ++j) {
            // If the elevator is at the current floor, print the amount of guests currently in the elevator
            // If the elevator is not at the current floor, print empty spaces
            if (elevators[j]->currentFloor == i) {
                printf("| [%d] |", elevators[j]->capacity - elevators[j]->guests[elevators[j]->capacity - 1].isInsideElevator);
            } else {
                printf("|     |");
            }
        }
        printf("|  (");

        // For each guest, check if they are on the current floor and print their destination floor
        for (int j = 0; j < numGuests[i]; ++j) {
            // If the guest is on the current floor, print their destination floor
            // If the guest is not on the current floor, print empty spaces
            if (guests[i][j].currentFloor == i) {
                // If the user is in the elevator, print a "-" instead of the destination floor
                if (guests[i][j].isInsideElevator == 1 || guests[i][j].hasReachedDestination == 1) {
                    printf("-");
                } else {
                    printf("%d", guests[i][j].destinationFloor);
                }
            }
            // Print a comma if the guest is not the last one on the current floor
            if (j != numGuests[i] - 1) {
                printf(",");
            }
        }
        printf(")\n");
        printf("||-   -+-   -||\n");
    }
    printf("++-----+-----++\n\n");
}

void simulationStep(char* hotelName, int numFloors, int numElevators, int elevatorCapacity, struct Guest** guests, const int* numGuests, struct Elevator** elevators) {

    // For each guest in the elevator, check if the guest has reached their destination floor
    // If the guest has reached their destination floor, remove them from the elevator
    for (int i = 0; i < numElevators; ++i) {
        for (int j = 0; j < elevators[i]->capacity; ++j) {
            if (elevators[i]->guests[j].destinationFloor == elevators[i]->currentFloor) {
                elevators[i]->guests[j].isInsideElevator = 0;
                elevators[i]->guests[j].hasReachedDestination = 1;
                // Print a message that the guest has reached their destination
                // A person reached their destination floor 1 with elevator 0.
                printf("A person reached their destination floor %d with elevator %d.\n", elevators[i]->currentFloor, i);

            }
        }
    }

    // For each guest on the same floor as the elevator, check if the elevator is going in their direction
    // If the elevator is going in their direction, check if there is space in the elevator
    // If there is space in the elevator, add the guest to the elevator
    for (int i = 0; i < numElevators; ++i) {
        for (int j = 0; j < numGuests[elevators[i]->currentFloor]; ++j) {
            if (elevators[i]->direction == 1 && guests[elevators[i]->currentFloor][j].destinationFloor > elevators[i]->currentFloor) {
                for (int k = 0; k < elevators[i]->capacity; ++k) {
                    if (elevators[i]->guests[k].isInsideElevator == 0 && elevators[i]->guests[k].hasReachedDestination == 0) {
                        elevators[i]->guests[k] = guests[elevators[i]->currentFloor][j];
                        guests[elevators[i]->currentFloor][j].isInsideElevator = 1;
                        break;
                    }
                }
            } else if (elevators[i]->direction == -1 && guests[elevators[i]->currentFloor][j].destinationFloor < elevators[i]->currentFloor) {
                for (int k = 0; k < elevators[i]->capacity; ++k) {
                    if (elevators[i]->guests[k].isInsideElevator == 0 && elevators[i]->guests[k].hasReachedDestination == 0) {
                        elevators[i]->guests[k] = guests[elevators[i]->currentFloor][j];
                        guests[elevators[i]->currentFloor][j].isInsideElevator = 1;
                        break;
                    }
                }
            }
        }
        // print each elevator on a new line and the current amount of guests in the elevator
        printf("Elevator %d: %d\n", i, elevators[i]->capacity - elevators[i]->guests[elevators[i]->capacity - 1].isInsideElevator);
    }

    // For each elevator, check if it is at the top or bottom floor and change its direction if it is
    for (int i = 0; i < numElevators; ++i) {
        if (elevators[i]->currentFloor == numFloors - 1) {
            elevators[i]->direction = -1;
        } else if (elevators[i]->currentFloor == 0) {
            elevators[i]->direction = 1;
        }
    }
    // For each elevator, go one step in the current direction
    for (int i = 0; i < numElevators; ++i) {
        elevators[i]->currentFloor += elevators[i]->direction;
    }
}

int areStringsEqual(const char* str1, const char* str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return 0;  // Strings are not equal
        }
        ++str1;
        ++str2;
    }
    printf("String are equal: %i\n", *str1 == '\0' && *str2 == '\0');
    return *str1 == '\0' && *str2 == '\0';  // Check if both strings reached the end simultaneously
}

// A function that takes a string, splits it by comma and returns an array of strings
char** splitStringByComma(char* str) {
    int numCommas = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] == ',') {
            ++numCommas;
        }
    }

    char** result = (char**)malloc((numCommas + 1) * sizeof(char*));
    int resultIndex = 0;
    int startIndex = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] == ',') {
            result[resultIndex] = (char*)malloc((i - startIndex + 1) * sizeof(char));
            for (int j = startIndex; j < i; ++j) {
                result[resultIndex][j - startIndex] = str[j];
            }
            result[resultIndex][i - startIndex] = '\0';
            ++resultIndex;
            startIndex = i + 1;
        }
    }
    result[resultIndex] = (char*)malloc((stringLength(str) - startIndex + 1) * sizeof(char));
    for (int j = startIndex; j < stringLength(str); ++j) {
        result[resultIndex][j - startIndex] = str[j];
    }
    result[resultIndex][stringLength(str) - startIndex] = '\0';

    return result;
}

// A function that takes a string and returns its length without using the string.h library
int stringLength(const char* str) {
    int length = 0;
    while (str[length] != '\0') {
        ++length;
    }
    return length;
}

// A function that checks if all guests have reached their destination. If true, return 1, else return 0
int allGuestsHaveReachedDestination(struct Guest** guests, const int* numGuests) {
    for (int i = 0; i < sizeof(numGuests); ++i) {
        for (int j = 0; j < numGuests[i]; ++j) {
            if (guests[i][j].hasReachedDestination == 0) {
                return 0;
            }
        }
    }
    return 1;
}
