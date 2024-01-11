//---------------------------------------------------------------------------------------------------------------------
// a2.c
//
// This program simulates the operation of an arbitrary number of elevators in a hotel with an arbitrary number of floors.
// You can read more in detail about it in the README.md file.
//
// Group: Matthias_Bergmann
//
// Author: 12320035
//---------------------------------------------------------------------------------------------------------------------
//

#include <stdio.h>
#include <stdlib.h>

// Constants
const int MAX_NUMBER_OF_FLOORS = 10;
const int MIN_NUMBER_OF_FLOORS = 3;
const int MAX_NUMBER_OF_ELEVATORS = 5;
const int MIN_NUMBER_OF_ELEVATORS = 1;
const int MAX_ELEVATOR_CAPACITY = 9;
const int MIN_ELEVATOR_CAPACITY = 1;
const int MAX_NUMBER_OF_PEOPLE_WAITING = 20;
const int MIN_NUMBER_OF_PEOPLE_WAITING = 2;
const int MAX_HOTELNAME_LENGTH = 50;
const int ELEVATOR_DIRECTION_UP = 1;
const int ELEVATOR_DIRECTION_DOWN = -1;
const int TRUE = 1;
const int FALSE = 0;

#define MAX_SPLITSTRING_ARRAY_SIZE 40 // Define a maximum size for the array
#define MAX_GUEST_ON_FLOOR_INPUT 100
#define MAX_INITIAL_STATE_INPUT 5
#define AMOUNT_OF_SPACES_PER_ELEVATOR 5
#define MAX_SHOW_ALL_STEPS_INPUT 20
#define ASCII_BASE_CHAR_NUMBER 32

// Structs
struct _Person_
{
	int current_floor_;
	int destination_floor_;
	int is_inside_elevator_; // 1 for true, 0 for false
	int has_reached_destination_; // 1 for true, 0 for false
};
typedef struct _Person_ Person;

struct _Elevator_
{
	int capacity_;
	int current_floor_;
	int direction_; // 1 for up, -1 for down
	Person** person_list_;
};
typedef struct _Elevator_ Elevator;

// Function prototypes
void getHotelNameInput(char* hotel_name);
void getNumberOfFloorsInput(char* hotel_name, int* number_of_floors);
void getNumberOfElevatorsInput(char* hotel_name, int* number_of_elevators);
void getElevatorsCapacityInput(char* hotel_name, int* elevator_capacity);
void getNumberOfPeopleWaitingOnEachFloor(int* number_of_people_waiting);
Person** initializeListOfPeople(int number_of_floors, int number_of_people_waiting);
int* getDestinationFloorInput(int number_of_floors, int floor);
Elevator* initializeElevators(int number_of_elevators, int elevator_capacity, int number_of_floors);
int showInitialStateInput();
void printHotel(char* hotel_name, int number_of_floors, int number_of_elevators,
								Person** person_list, int number_of_people, Elevator* elevators, int elevator_capacity);
void printHotelNameDynamic(char* hotel_name, int number_of_elevators);
void printHotelElevators(const Elevator* elevators, int elevator_capacity, int current_floor, int current_elevator);
void printElevatorDecorator(int number_of_elevators, int full_line);
int startSimulationInput();
int shouldPrintAllSteps();
int allGuestsHaveReachedDestination(Person** person_list, int number_of_people, int number_of_floors);
void simulationStep(int number_of_floors, int number_of_elevators, Person** person_list, int number_of_guests,
										Elevator* elevators, int elevator_capacity);
void moveElevator(int number_of_elevators, Elevator* elevators);
void removePeopleFromElevator(int number_of_elevators, Elevator* elevators, int elevator_capacity);
void fillElevatorWithPeople(int number_of_elevators, int number_of_guests, int elevator_capacity,
														Elevator* elevators, Person** person_list, int number_of_floors);
void addPersonToElevator(int elevator_capacity, Elevator* elevators, Person** person_list,
												 int elevator_index, int person_index);
int getBestNextPersonIndex(Person** person_list, int number_of_people, int number_of_floors,
													 int current_floor, int elevator_direction);
void changeElevatorDirection(int number_of_floors, int number_of_elevators, Elevator* elevators);
void printElevatorState(Elevator* elevator, int number_of_elevators, int elevator_capacity);
int areAllElevatorsEmpty(const Elevator* elevator, int number_of_elevators, int elevator_capacity);
void printSortedPersonList(Person* person_list_per_floor, int number_of_people);
void printSortedElevatorPersonList(Person** person_list_per_floor, int number_of_people);
void freeAllocatedMemory(int number_of_floors, int number_of_elevators, Person** person_list, Elevator* elevators);

// String helper functions
int* splitStringByComma(char* input_string, int* size);
int areStringsEqual(char* first_string, char* second_string);
int getStringLength(const char* string);


//---------------------------------------------------------------------------------------------------------------------
///
/// The main function of the program. It takes the user input and calls the functions to initialize the hotel and
/// simulate the elevator operation. At the end, it also makes sure to free the allocated memory.
///
/// @return
///     0 - Simulation not started
///     -1 - Memory allocation error
///     number of steps - Simulation completed successfully
//
int main()
{
	char hotel_name[MAX_HOTELNAME_LENGTH];
	int number_of_floors;
	int number_of_elevators;
	int elevator_capacity;
	int number_of_people_waiting;
	// Collect input from the user
	getHotelNameInput(hotel_name);
	getNumberOfFloorsInput(hotel_name, &number_of_floors);
	getNumberOfElevatorsInput(hotel_name, &number_of_elevators);
	getElevatorsCapacityInput(hotel_name, &elevator_capacity);
	getNumberOfPeopleWaitingOnEachFloor(&number_of_people_waiting);
	// Initialize the struct arrays
	Person** person_list = initializeListOfPeople(number_of_floors, number_of_people_waiting);
	if (person_list == NULL)
	{
		freeAllocatedMemory(number_of_floors, number_of_elevators, person_list, NULL);
		return -1;
	}
	Elevator* elevator_list = initializeElevators(number_of_elevators, elevator_capacity, number_of_floors);
	if (elevator_list == NULL)
	{
		freeAllocatedMemory(number_of_floors, number_of_elevators, person_list, elevator_list);
		return -1;
	}
	if (showInitialStateInput())
	{
		printHotel(hotel_name, number_of_floors, number_of_elevators, person_list,
							 number_of_people_waiting, elevator_list, elevator_capacity);
		printf("\n");
	}
	if (startSimulationInput())
	{
		int print_all_steps = shouldPrintAllSteps();
		int steps_counter = 0;
		// As long as not all guests have reached their destination, keep simulating
		while (!allGuestsHaveReachedDestination(person_list, number_of_people_waiting, number_of_floors))
		{
			if (print_all_steps)
			{
				printf("=== Simulation Step %i ===\n", steps_counter + 1);
			}
			// Calculates the next step of the simulation
			simulationStep(number_of_floors, number_of_elevators, person_list, number_of_people_waiting,
										 elevator_list, elevator_capacity);
			if (print_all_steps) {
				// Prints the current state of the simulation
				printHotel(hotel_name, number_of_floors, number_of_elevators, person_list,
									 number_of_people_waiting, elevator_list, elevator_capacity);
				printElevatorState(elevator_list, number_of_elevators, elevator_capacity);
			}
			// Moves each elevator in their respective direction
			moveElevator(number_of_elevators, elevator_list);
			steps_counter++;
		}
		// Fix elevator position at the last run
		for (int i = 0; i < number_of_elevators; ++i)
		{
			elevator_list[i].current_floor_ -= elevator_list[i].direction_;
		}
		printf("\n=================\n   FINAL STATE\n=================\n\n");
		printHotel(hotel_name, number_of_floors, number_of_elevators, person_list,
							 number_of_people_waiting, elevator_list, elevator_capacity);
		printf("\nSimulation done in %i steps!\n", steps_counter);
		freeAllocatedMemory(number_of_floors, number_of_elevators, person_list, elevator_list);
		return steps_counter;
	}
	else
	{
		freeAllocatedMemory(number_of_floors, number_of_elevators, person_list, elevator_list);
		return 0;
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that takes the name of the hotel as input and converts it to uppercase, then saves it
/// in the hotel_name variable passed as a parameter.
///
/// @param hotel_name - The name of the hotel
//
void getHotelNameInput(char* hotel_name)
{
	printf("____________________\n"
				 " ____   ____   ____ \n"
				 "|____  |____  |____|\n"
				 "|____   ____| |     \n"
				 "____________________\n\n"
				 "Welcome to the\n"
				 "Elevator Simulation Program!\n\n");
	printf("Enter the name of the hotel:\n > ");
	fgets(hotel_name, MAX_HOTELNAME_LENGTH, stdin);
	hotel_name[getStringLength(hotel_name) - 1] = '\0';
	for (int i = 0; hotel_name[i] != '\0'; ++i)
	{
		if (hotel_name[i] >= 'a' && hotel_name[i] <= 'z')
		{
			hotel_name[i] -= ASCII_BASE_CHAR_NUMBER;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that takes the number of floors of the hotel as input and checks if it is in the range of
/// MIN_NUMBER_OF_FLOORS and MAX_NUMBER_OF_FLOORS. If it is, it saves it in the number_of_floors variable passed as a
/// parameter. Else it asks the user to enter a new number of floors.
///
/// @param hotel_name - The name of the hotel
/// @param number_of_floors - The number of floors of the hotel
//
void getNumberOfFloorsInput(char* hotel_name, int* number_of_floors)
{
	do {
		printf("Enter the number of floors in hotel %s:\n > ", hotel_name);
		scanf("%i", number_of_floors);
		while ((getchar()) != '\n');
		if ((*number_of_floors) < MIN_NUMBER_OF_FLOORS || (*number_of_floors) > MAX_NUMBER_OF_FLOORS)
		{
			printf("Wrong input, the number of floors must be between %i and %i!\n", MIN_NUMBER_OF_FLOORS,
						 MAX_NUMBER_OF_FLOORS);
		}
	} while ((*number_of_floors) < MIN_NUMBER_OF_FLOORS || (*number_of_floors) > MAX_NUMBER_OF_FLOORS);
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that takes the number of elevators of the hotel as input and checks if it is in the range of
/// MIN_NUMBER_OF_ELEVATORS and MAX_NUMBER_OF_ELEVATORS. If it is, it saves it in the number_of_elevators variable
/// passed as a parameter. Else it asks the user to enter a new number of elevators.
///
/// @param hotel_name - The name of the hotel
/// @param number_of_elevators - The number of elevators of the hotel
//
void getNumberOfElevatorsInput(char* hotel_name, int* number_of_elevators)
{
	do
	{
		printf("Enter the number of elevators in hotel %s:\n > ", hotel_name);
		scanf("%i", number_of_elevators);
		while ((getchar()) != '\n');
		if ((*number_of_elevators) < MIN_NUMBER_OF_ELEVATORS || (*number_of_elevators) > MAX_NUMBER_OF_ELEVATORS)
		{
			printf("Wrong input, the number of elevators must be between %i and %i!\n", MIN_NUMBER_OF_ELEVATORS,
						 MAX_NUMBER_OF_ELEVATORS);
		}
	} while ((*number_of_elevators) < MIN_NUMBER_OF_ELEVATORS || (*number_of_elevators) > MAX_NUMBER_OF_ELEVATORS);
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that takes the capacity of elevators of the hotel as input and checks if it is in the range of
/// MIN_ELEVATOR_CAPACITY and MAX_ELEVATOR_CAPACITY. If it is, it saves it in the elevator_capacity variable passed as a
/// parameter. Else it asks the user to enter a new capacity of elevators.
///
/// @param hotel_name - The name of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
//
void getElevatorsCapacityInput(char* hotel_name, int* elevator_capacity)
{
	do {
		printf("Enter the capacity of elevators in hotel %s:\n > ", hotel_name);
		scanf("%i", elevator_capacity);
		while ((getchar()) != '\n');
		if ((*elevator_capacity) < MIN_ELEVATOR_CAPACITY || (*elevator_capacity) > MAX_ELEVATOR_CAPACITY)
		{
			printf("Wrong input, the capacity of elevators must be between %i and %i person(s)!\n",
						 MIN_ELEVATOR_CAPACITY, MAX_ELEVATOR_CAPACITY);
		}
	} while ((*elevator_capacity) < MIN_ELEVATOR_CAPACITY || (*elevator_capacity) > MAX_ELEVATOR_CAPACITY);
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that takes the number of people waiting on each floor of the hotel as input and checks if it is in the
/// range of MIN_NUMBER_OF_PEOPLE_WAITING and MAX_NUMBER_OF_PEOPLE_WAITING. If it is, it saves it in the
/// number_of_people_waiting variable passed as a parameter. Else it asks the user to enter a new number of people
/// waiting on each floor.
///
/// @param number_of_people_waiting - The number of people waiting on each floor of the hotel
//
void getNumberOfPeopleWaitingOnEachFloor(int* number_of_people_waiting)
{
	do
	{
		printf("Enter the number of people waiting on each floor:\n > ");
		scanf("%i", number_of_people_waiting);
		while ((getchar()) != '\n');
		if ((*number_of_people_waiting) < MIN_NUMBER_OF_PEOPLE_WAITING ||
				(*number_of_people_waiting) > MAX_NUMBER_OF_PEOPLE_WAITING)
		{
			printf("Wrong input, the number of people waiting on each floor must be between %i and %i!\n",
						 MIN_NUMBER_OF_PEOPLE_WAITING, MAX_NUMBER_OF_PEOPLE_WAITING);
		}
	} while ((*number_of_people_waiting) < MIN_NUMBER_OF_PEOPLE_WAITING ||
					 (*number_of_people_waiting) > MAX_NUMBER_OF_PEOPLE_WAITING);
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that takes the number of floors and the number of people waiting on each floor as input and initializes
/// the list of people. It asks the user to enter the destination floors of the people on each floor, then creates a
/// Person struct for each person and saves it in the person_list array. The person_list array is an array of arrays,
/// where each array represents a floor and contains the people waiting on that floor.
///
/// @param number_of_floors - The number of floors of the hotel
/// @param number_of_people_waiting - The number of people waiting on each floor of the hotel
///
/// @return person_list - The list of people waiting on each floor of the hotel
/// 			 NULL - Memory allocation error
//
Person** initializeListOfPeople(int number_of_floors, int number_of_people_waiting)
{
	Person** person_list = malloc(number_of_floors * sizeof(Person));
	if (person_list == NULL)
	{
		printf("Out of memory! Program terminated!\n");
		return NULL;
	}
	for (int floor = 0; floor < number_of_floors; ++floor)
	{
		person_list[floor] = malloc(number_of_people_waiting * sizeof(Person));
		if (person_list[floor] == NULL)
		{
			for (int i = 0; i < floor; ++i)
			{
				free(person_list[i]);
			}
			free(person_list);
			printf("Out of memory! Program terminated!\n");
			return NULL;
		}
		int* splitInput = getDestinationFloorInput(number_of_floors, floor);
		if (splitInput == NULL)
		{
			for (int i = 0; i < number_of_floors; ++i)
			{
				free(person_list[i]);
			}
			free(person_list);
			printf("Out of memory! Program terminated!\n");
			return NULL;
		}
		for (int guest_on_floor = 0; guest_on_floor < number_of_people_waiting; ++guest_on_floor)
		{
			Person person = {floor, splitInput[guest_on_floor], 0, 0};
			person_list[floor][guest_on_floor] = person;
		}
		free(splitInput);
		splitInput = NULL;
	}
	return person_list;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A helper function to initialize the list of people that takes the number of floors and the current floor as input
/// and returns an array of destination floors for the people on the current floor. This should help with the
/// readability of the initializeListOfPeople function. It also checks if the input is valid and asks the user to enter
/// a new input if it is not.
///
/// @param number_of_floors - The number of floors of the hotel
/// @param floor - The current floor
///
/// @return splitInput - The array of destination floors
/// 			 NULL - Memory allocation error
//
int* getDestinationFloorInput(int number_of_floors, int floor)
{
	int* split_input;
	int input_size;
	int errors;
	do
	{
		printf("Enter the destination floors of the people [floor: %d]:\n > ", floor);
		char buffer[MAX_GUEST_ON_FLOOR_INPUT];
		fgets(buffer, sizeof(buffer), stdin);
		// Remove the trailing newline character if present
		size_t len = getStringLength(buffer);
		if (len > 0 && buffer[len - 1] == '\n')
		{
			buffer[len - 1] = '\0';
		}
		split_input = splitStringByComma(buffer, &input_size);
		if (split_input == NULL)
		{
			printf("Out of memory! Program terminated!\n");
			return NULL;
		}
		errors = 0;
		for (int i = 0; i < input_size; ++i)
		{
			if (split_input[i] == floor)
			{
				printf("Wrong input, the destination floor cannot be the current floor!\n");
				errors++;
			}
			else if (split_input[i] < 0 || split_input[i] >= number_of_floors)
			{
				printf("Wrong input, the destination floor %i is out of range (0 to %i)!\n",
							 split_input[i], number_of_floors - 1);
				errors++;
			}
		}
		if (errors > 0)
		{
			free(split_input);
			split_input = NULL;
		}
	} while (errors > 0);
	return split_input;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that takes the number of elevators, the capacity of elevators and the number of floors as input and
/// initializes the list of elevators. It creates an Elevator struct for each elevator and saves it in the elevators
/// array. It also determines the starting floor and direction of each elevator.
///
/// @param number_of_elevators - The number of elevators of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
/// @param number_of_floors - The number of floors of the hotel
///
/// @return elevators - The list of elevators of the hotel
/// 			 NULL - Memory allocation error
//
Elevator* initializeElevators(int number_of_elevators, int elevator_capacity, int number_of_floors)
{
	Elevator* elevators = malloc(number_of_elevators * sizeof(Elevator));
	if (elevators == NULL)
	{
		printf("Out of memory! Program terminated!\n");
		return NULL;
	}
	for (int i = 0; i < number_of_elevators; ++i)
	{
		Elevator elevator = {elevator_capacity, 0, 1, NULL};
		// If it's an odd number, start at the top floor and go down, else start at the bottom floor and go up
		if (i % 2 != 0)
		{
			elevator.current_floor_ = number_of_floors - 1;
			elevator.direction_ = ELEVATOR_DIRECTION_DOWN;
		}
		else
		{
			elevator.current_floor_ = 0;
			elevator.direction_ = ELEVATOR_DIRECTION_UP;
		}
		elevator.person_list_ = malloc(elevator_capacity * sizeof(Person));
		if (elevator.person_list_ == NULL)
		{
			printf("Out of memory! Program terminated!\n");
			return NULL;
		}
		for (int j = 0; j < elevator_capacity; ++j)
		{
			elevator.person_list_[j] = NULL;
		}
		elevators[i] = elevator;
	}
	return elevators;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that asks the user if they want to see the initial state of the simulation. If they do, it prints the
/// initial state header and then returns 1. If they don't, it returns 0. The main function is then responsible for
/// printing the initial state.
///
/// @return 1 - Show the initial state
///         0 - Don't show the initial state
//
int showInitialStateInput()
{
	char show_initial_state[MAX_INITIAL_STATE_INPUT];
	do
	{
		printf("Show the initial state? (\"yes\"/\"no\"):\n > ");
		scanf("%4s", show_initial_state);
		// empty the buffer
		while ((getchar()) != '\n');
	} while (!areStringsEqual(show_initial_state, "yes") &&
					 !areStringsEqual(show_initial_state, "no"));
	if (areStringsEqual(show_initial_state, "yes"))
	{
		printf("\n=================\n  INITIAL STATE\n=================\n\n");
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that prints the hotel with the current state of the elevators and the people waiting on each floor. It
/// calls the printHotelNameDynamic function to print the name of the hotel in the middle of the top line of the hotel.
/// It calls the printElevatorDecorator function to print the decorator of the hotel. It calls the
/// printHotelElevators function to print the elevators of the hotel. It calls the printSortedPersonList function to
/// print the people waiting on each floor of the hotel. This function is dynamic, which allows it to be called on
/// every step of the simulation.
///
/// @param hotel_name - The name of the hotel
/// @param number_of_floors - The number of floors of the hotel
/// @param number_of_elevators - The number of elevators of the hotel
/// @param person_list - The list of people waiting on each floor of the hotel
/// @param number_of_people - The number of people waiting on each floor of the hotel
/// @param elevators - The list of elevators of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
//
void printHotel(char* hotel_name, int number_of_floors, int number_of_elevators, Person** person_list,
								int number_of_people, Elevator* elevators, int elevator_capacity)
{
	printHotelNameDynamic(hotel_name, number_of_elevators);
	printElevatorDecorator(number_of_elevators, TRUE);
	for (int i = number_of_floors - 1; i >= 0; --i)
	{
		printf("||");
		for (int j = 0; j < number_of_elevators; ++j)
		{
			printHotelElevators(elevators, elevator_capacity, i, j);
		}
		printf("|  ");
		printSortedPersonList(person_list[i], number_of_people);
		printf("\n");
		if (i != 0)
		{
			printElevatorDecorator(number_of_elevators, FALSE);
		}
	}
	printElevatorDecorator(number_of_elevators, TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that prints the name of the hotel in the middle of the top line of the hotel. It calculates the number
/// of minus signs needed to fill the line and then prints the name of the hotel in the middle of the line.
///
/// @param hotel_name - The name of the hotel
/// @param number_of_elevators - The number of elevators of the hotel
//
void printHotelNameDynamic(char* hotel_name, int number_of_elevators)
{
	int number_of_minus = number_of_elevators * AMOUNT_OF_SPACES_PER_ELEVATOR + number_of_elevators - 1;
	int name_length = getStringLength(hotel_name);
	int remaining_space = number_of_minus - name_length;
	printf("++");
	for (int i = 0; i < number_of_minus; ++i)
	{
		printf("-");
	}
	printf("++\n");
	printf("++");
	if (remaining_space > 0)
	{
		if (remaining_space % 2 != 0)
		{
			for (int i = 0; i < remaining_space / 2; ++i)
			{
				printf(" ");
			}
			printf("%s", hotel_name);
			for (int i = 0; i < remaining_space / 2 + 1; ++i)
			{
				printf(" ");
			}
		} else {
			for (int i = 0; i < remaining_space / 2; ++i)
			{
				printf(" ");
			}
			printf("%s", hotel_name);
			for (int i = 0; i < remaining_space / 2; ++i)
			{
				printf(" ");
			}
		}
	}
	else
	{
		printf("%s", hotel_name);
	}
	printf("++\n");
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that prints the decorator of the hotel. It has two modes, one for a full line and one for a line with a
/// gap in the middle. It takes the number of elevators and the mode as input and prints the decorator accordingly.
///
/// @param number_of_elevators - The number of elevators of the hotel
/// @param full_line - 1 for a full line, 0 for a line with a gap in the middle
//
void printElevatorDecorator(int number_of_elevators, int full_line)
{
	if (full_line)
	{
		printf("++");
	} else {
		printf("||");
	}
	for (int j = 0; j < number_of_elevators; ++j)
	{
		if (full_line)
		{
			printf("-----");
		} else {
			printf("-   -");
		}
		if (j != number_of_elevators - 1)
		{
			printf("+");
		}
	}
	if (full_line)
	{
		printf("++\n");
	}
	else
	{
		printf("||\n");
	}
}


//---------------------------------------------------------------------------------------------------------------------
///
/// A function that prints the elevators of the hotel. Note that this function simply prints the elevator inside the
/// hotel as a box, not the elevators that are displayed beneath the hotel.
///
/// @param elevators - The list of elevators of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
/// @param current_floor - The current floor
/// @param current_elevator - The current elevator
//
void printHotelElevators(const Elevator* elevators, int elevator_capacity, int current_floor, int current_elevator)
{
	if (elevators[current_elevator].current_floor_ == current_floor)
	{
		printf(" [%d] |", elevator_capacity - elevators[current_elevator].capacity_);
	}
	else
	{
		printf("     |");
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function asks the user if they want to start the simulation. If they do, it returns 1.
/// If they don't, it returns 0. The main function is then responsible for starting the simulation.
///
/// @return 1 if the user wants to start the simulation
///         0 if the user wants to skip the simulation
//
int startSimulationInput()
{
	char start_simulation[10];
	do
	{
		printf("Start the simulation? (\"start\"/\"skip\"):\n > ");
		scanf("%9s", start_simulation);
		while ((getchar()) != '\n');
	} while (!areStringsEqual(start_simulation, "start") &&
					 !areStringsEqual(start_simulation, "skip"));
	if (areStringsEqual(start_simulation, "start"))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that asks the user if they want to see all steps of the simulation or just the end result. If they want
/// to see all steps, it returns 1. If they want to see just the end result, it returns 0. The main function is then
/// responsible for printing the steps of the simulation.
///
/// @return 1 if the user wants to see all steps of the simulation
///         0 if the user wants to see just the end result
//
int shouldPrintAllSteps()
{
	int print_steps = FALSE;
	while (1)
	{
		printf("Show all steps of the simulation? (\"all steps\"/\"end result\"):\n > ");
		char show_all_steps[MAX_SHOW_ALL_STEPS_INPUT];
		scanf(" %[^\n]19s", show_all_steps);
		while ((getchar()) != '\n');
		if (areStringsEqual(show_all_steps, "all steps"))
		{
			print_steps = TRUE;
			break;
		}
		else if (areStringsEqual(show_all_steps, "end result"))
		{
			break;
		}
	}
	return print_steps;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that checks if all guests have reached their destination. It loops through the list of people waiting on
/// each floor and checks if they have reached their destination floor. If they have, it returns 1. If they haven't, it
/// returns 0.
///
/// @param person_list - The list of people waiting on each floor of the hotel
/// @param number_of_people - The number of people waiting on each floor of the hotel
/// @param number_of_floors - The number of floors of the hotel
/// @return 1 if all guests have reached their destination
///         0 if not all guests have reached their destination
//
int allGuestsHaveReachedDestination(Person** person_list, int number_of_people, int number_of_floors)
{
	for (int i = 0; i < number_of_floors; ++i)
	{
		for (int j = 0; j < number_of_people; ++j)
		{
			if (person_list[i][j].has_reached_destination_ == FALSE)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Simulates one step of the elevator operation. It checks if the elevator is at the top or bottom floor and changes
/// its direction_ if it is. It checks if the guests in the elevator have reached their destination floor and removes
/// them from the elevator if they have. It checks if the guests on the same floor as the elevator are going in the
/// same direction_ as the elevator and adds them to the elevator if there is space.
///
/// @param number_of_floors - The number of floors of the hotel
/// @param number_of_elevators - The number of elevators of the hotel
/// @param person_list - The list of people waiting on each floor of the hotel
/// @param number_of_guests - The number of people waiting on each floor of the hotel
/// @param elevators - The list of elevators of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
/// @param show_steps - 1 if the user wants to see all steps of the simulation
//
void simulationStep(int number_of_floors, int number_of_elevators, Person** person_list, int number_of_guests,
										Elevator* elevators, int elevator_capacity)
{
	changeElevatorDirection(number_of_floors, number_of_elevators, elevators);
	removePeopleFromElevator(number_of_elevators, elevators, elevator_capacity);
	fillElevatorWithPeople(number_of_elevators, number_of_guests, elevator_capacity,
												 elevators, person_list, number_of_floors);
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that moves each elevator in their respective direction.
///
/// @param number_of_elevators - The number of elevators of the hotel
/// @param elevators - The list of elevators of the hotel
//
void moveElevator(int number_of_elevators, Elevator* elevators)
{
	for (int i = 0; i < number_of_elevators; ++i)
	{
		elevators[i].current_floor_ += elevators[i].direction_;
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that removes people from the elevator if they have reached their destination floor. It loops through the
/// list of people in the elevator and checks if they have reached their destination floor. If they have, it removes
/// them from the elevator and sets their is_inside_elevator_ flag to 0 and their has_reached_destination_ flag to 1.
/// Note that they are not deleted, they are simply removed from the elevator. This is intentional, as it allows us to
/// keep track of the people that have reached their destination and print them at the end of the simulation.
///
/// @param number_of_elevators - The number of elevators of the hotel
/// @param elevators - The list of elevators of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
/// @param show_steps - 1 if the user wants to see all steps of the simulation
//
void removePeopleFromElevator(int number_of_elevators, Elevator* elevators, int elevator_capacity)
{
	for (int i = 0; i < number_of_elevators; ++i)
	{
		for (int j = 0; j < elevator_capacity; ++j)
		{
			if (elevators[i].person_list_[j] == NULL)
			{
				continue;
			}
			if (elevators[i].person_list_[j]->destination_floor_ == elevators[i].current_floor_)
			{
				elevators[i].person_list_[j]->is_inside_elevator_ = FALSE;
				elevators[i].person_list_[j]->has_reached_destination_ = TRUE;
				printf("A person reached their destination floor %d with elevator %d.\n",
							 elevators[i].current_floor_, i);
				elevators[i].person_list_[j] = NULL;
				elevators[i].capacity_++;
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that adds people to the elevator if they are on the same floor as the elevator and are going in the same
/// direction as the elevator. It loops through the list of people waiting on each floor and adds them to the elevator
/// if they are on the same floor as the elevator and are going in the same direction as the elevator. It calls the
/// getBestNextPersonIndex function to get the index of the best next person to pick up. It calls the
/// addPersonToElevator function to add the person to the elevator.
///
/// @param number_of_elevators - The number of elevators of the hotel
/// @param number_of_guests - The number of people waiting on each floor of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
/// @param elevators - The list of elevators of the hotel
/// @param person_list - The list of people waiting on each floor of the hotel
//
void fillElevatorWithPeople(int number_of_elevators, int number_of_guests, int elevator_capacity, Elevator* elevators,
														Person** person_list, int number_of_floors)
{
	for (int i = 0; i < number_of_elevators; ++i)
	{
		for (int j = 0; j < number_of_guests; ++j)
		{
			int person_index = getBestNextPersonIndex(person_list, number_of_guests, number_of_floors,
																								elevators[i].current_floor_, elevators[i].direction_);
			if (person_index == -1)
			{
				continue;
			}
			addPersonToElevator(elevator_capacity, elevators, person_list, i, person_index);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that loops through the available elevator spots and adds the person to the elevator if there is an
/// available spot. It also sets the is_inside_elevator_ flag of the person to 1. If there is no available spot, it
/// does nothing.
///
/// @param elevator_capacity - The capacity of elevators of the hotel
/// @param elevators - The list of elevators of the hotel
/// @param person_list - The list of people waiting on each floor of the hotel
/// @param elevator_index - The index of the elevator
/// @param person_index - The index of the person
//
void addPersonToElevator(int elevator_capacity, Elevator* elevators, Person** person_list,
												 int elevator_index, int person_index)
{
	for (int k = 0; k < elevator_capacity; ++k)
	{
		if (elevators[elevator_index].person_list_[k] == NULL)
		{
			elevators[elevator_index].person_list_[k] = &person_list[elevators[elevator_index].current_floor_][person_index];
			person_list[elevators[elevator_index].current_floor_][person_index].is_inside_elevator_ = TRUE;
			elevators[elevator_index].capacity_--;
			return;
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------
///
/// A function that calculates the index of the best next person to pick up. It loops through the list of people
/// waiting on the current floor and returns the index of the person that is going in the same direction_ as the
/// elevator and is closest to the elevator.
///
/// @param person_list - The list of people waiting on each floor of the hotel
/// @param number_of_people - The number of people waiting on each floor of the hotel
/// @param number_of_floors - The number of floors of the hotel
/// @param current_floor - The current floor
/// @param elevator_direction - The direction_ of the elevator
///
/// @return - The index of the best next person to pick up
//
int getBestNextPersonIndex(Person** person_list, int number_of_people, int number_of_floors,
													 int current_floor, int elevator_direction)
{
	int best_next_person_index = -1;
	int best_next_person_distance = number_of_floors;
	for (int i = 0; i < number_of_people; ++i)
	{
		if (person_list[current_floor][i].current_floor_ == current_floor &&
				person_list[current_floor][i].is_inside_elevator_ == FALSE &&
				person_list[current_floor][i].has_reached_destination_ == FALSE)
		{
			if (elevator_direction == ELEVATOR_DIRECTION_UP)
			{
				if (person_list[current_floor][i].destination_floor_ > current_floor)
				{
					if (person_list[current_floor][i].destination_floor_ - current_floor < best_next_person_distance)
					{
						best_next_person_distance = person_list[current_floor][i].destination_floor_ - current_floor;
						best_next_person_index = i;
					}
				}
			}
			else if (elevator_direction == ELEVATOR_DIRECTION_DOWN)
			{
				if (person_list[current_floor][i].destination_floor_ < current_floor)
				{
					if (current_floor - person_list[current_floor][i].destination_floor_ < best_next_person_distance)
					{
						best_next_person_distance = current_floor - person_list[current_floor][i].destination_floor_;
						best_next_person_index = i;
					}
				}
			}
		}
	}
	return best_next_person_index;
}


//---------------------------------------------------------------------------------------------------------------------
///
/// A function that changes the direction_ of the elevator if it is at the top or bottom floor. If it is at the top
/// floor, it changes the direction_ to -1. If it is at the bottom floor, it changes the direction_ to 1. This has
/// also been shortly described when creating the struct at the top of this file.
///
/// @param number_of_floors - The number of floors of the hotel
/// @param number_of_elevators - The number of elevators of the hotel
/// @param elevators - The list of elevators of the hotel
//
void changeElevatorDirection(int number_of_floors, int number_of_elevators, Elevator* elevators)
{
	for (int i = 0; i < number_of_elevators; ++i)
	{
		if (elevators[i].current_floor_ == number_of_floors - 1)
		{
			elevators[i].direction_ = ELEVATOR_DIRECTION_DOWN;
		}
		else if (elevators[i].current_floor_ == 0)
		{
			elevators[i].direction_ = ELEVATOR_DIRECTION_UP;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that prints the current state of the elevators. It prints the Index of the elevator and the list of
/// destination floors of the people inside the elevator. It calls the printSortedElevatorPersonList function to print
/// the list of destination floors of the people inside the elevator dynamically. This ensures that the list of
/// destination floors is always sorted in ascending order.
///
/// @param elevator - The list of elevators of the hotel
/// @param number_of_elevators - The number of elevators of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
//
void printElevatorState(Elevator* elevator, int number_of_elevators, int elevator_capacity)
{
	int are_all_elevators_empty = areAllElevatorsEmpty(elevator, number_of_elevators, elevator_capacity);
	if (!are_all_elevators_empty)
	{
		printf("Elevators:\n");
		for (int i = 0; i < number_of_elevators; ++i)
		{
			printf("%i: ", i);
			printSortedElevatorPersonList(elevator[i].person_list_, elevator_capacity);
			printf("\n");
		}
	}
	printf("\n");
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A helper function that checks if all elevators are empty. It loops through the list of elevators and checks if they
/// are empty. If they are, it returns 1. If they are not, it returns 0.
///
/// @param elevator - The list of elevators of the hotel
/// @param number_of_elevators - The number of elevators of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
/// @return 1 if all elevators are empty
///         0 if not all elevators are empty
//
int areAllElevatorsEmpty(const Elevator* elevator, int number_of_elevators, int elevator_capacity)
{
	int are_all_elevators_empty = TRUE;
	for (int i = 0; i < number_of_elevators; ++i)
	{
		if (elevator[i].capacity_ != elevator_capacity)
		{
			are_all_elevators_empty = FALSE;
			break;
		}
	}
	return are_all_elevators_empty;
}

// --------------------------------------------------------------------------------------------------------------------
///
/// A function that prints the people waiting on a specific floor in a sorted manner. It starts by checking the amount
/// of people that have reached their destination floor or are inside an elevator. It then uses that number to loop
/// through the list of people again and print them as a "-". Then, trough a subtraction, it calculates the number of
/// people that are still waiting on the floor and prints them in ascending order.
///
/// @param person_list_per_floor - The list of people waiting on a specific floor
/// @param number_of_people - The number of people waiting on a specific floor
//
void printSortedPersonList(Person* person_list_per_floor, int number_of_people)
{
	printf("(");
	int person_finished_count = 0;
	for (int i = 0; i < number_of_people; ++i)
	{
		if (person_list_per_floor[i].has_reached_destination_ == TRUE ||
				person_list_per_floor[i].is_inside_elevator_ == TRUE)
		{
			person_finished_count++;
		}
	}
	int non_finished_people_count = number_of_people - person_finished_count;
	for (int i = 0; i < number_of_people; ++i)
	{
		if (person_list_per_floor[i].has_reached_destination_ == TRUE
				|| person_list_per_floor[i].is_inside_elevator_ == TRUE)
		{
			printf("-");
			person_finished_count--;
			if (person_finished_count != 0)
			{
				printf(",");
			}
		}
	}
	if (non_finished_people_count == 0)
	{
		printf(")");
	}
	else
	{
		if (non_finished_people_count != number_of_people)
		{
			printf(",");
		}
		int remaining_people_array[non_finished_people_count];
		int remaining_people_count = 0;
		for (int i = 0; i < number_of_people; ++i)
		{
			if (person_list_per_floor[i].has_reached_destination_ == FALSE &&
					person_list_per_floor[i].is_inside_elevator_ == FALSE)
			{
				remaining_people_array[remaining_people_count] = person_list_per_floor[i].destination_floor_;
				remaining_people_count++;
			}
		}
		// Sort the remaining people array in ascending order using bubble sort
		for (int i = 0; i < non_finished_people_count; ++i)
		{
			for (int j = 0; j < non_finished_people_count - i - 1; ++j)
			{
				if (remaining_people_array[j] > remaining_people_array[j + 1])
				{
					int temp = remaining_people_array[j];
					remaining_people_array[j] = remaining_people_array[j + 1];
					remaining_people_array[j + 1] = temp;
				}
			}
		}
		for (int i = 0; i < non_finished_people_count; ++i)
		{
			if (i == non_finished_people_count - 1)
			{
				printf("%i", remaining_people_array[i]);
			}
			else
			{
				printf("%i,", remaining_people_array[i]);
			}
		}
		printf(")");
	}
}

// --------------------------------------------------------------------------------------------------------------------
///
/// A function that prints the people inside the elevator in a sorted manner. It starts by checking the amount of people
/// that have reached their destination floor. It then uses that number to loop through the list of people again and
/// print them as a "-". Then, trough a subtraction, it calculates the number of people that are still inside the
/// elevator and prints them in ascending order.
///
/// @param person_list_per_floor - The list of people waiting on a specific floor
/// @param number_of_people - The number of people waiting on a specific floor
//
void printSortedElevatorPersonList(Person** person_list_per_floor, int number_of_people)
{
	printf("(");
	int person_finished_count = 0;
	for (int i = 0; i < number_of_people; ++i)
	{
		if (person_list_per_floor[i] == NULL || person_list_per_floor[i]->has_reached_destination_ == TRUE)
		{
			person_finished_count++;
		}
	}
	int non_finished_people_count = number_of_people - person_finished_count;
	for (int i = 0; i < number_of_people; ++i)
	{
		if (person_list_per_floor[i] == NULL || person_list_per_floor[i]->has_reached_destination_ == TRUE)
		{
			printf("-");
			person_finished_count--;
			if (person_finished_count != 0)
			{
				printf(",");
			}
		}
	}
	if (non_finished_people_count == 0)
	{
		printf(")");
	}
	else
	{
		if (non_finished_people_count != number_of_people)
		{
			printf(",");
		}
		int remaining_people_array[non_finished_people_count];
		int remaining_people_count = 0;
		for (int i = 0; i < number_of_people; ++i)
		{
			if (person_list_per_floor[i] != NULL && person_list_per_floor[i]->has_reached_destination_ == FALSE)
			{
				remaining_people_array[remaining_people_count] = person_list_per_floor[i]->destination_floor_;
				remaining_people_count++;
			}
		}
		// Sort the remaining people array in ascending order using bubble sort
		for (int i = 0; i < non_finished_people_count; ++i)
		{
			for (int j = 0; j < non_finished_people_count - i - 1; ++j)
			{
				if (remaining_people_array[j] > remaining_people_array[j + 1])
				{
					int temp = remaining_people_array[j];
					remaining_people_array[j] = remaining_people_array[j + 1];
					remaining_people_array[j + 1] = temp;
				}
			}
		}
		for (int i = 0; i < non_finished_people_count; ++i)
		{
			if (i == non_finished_people_count - 1)
			{
				printf("%i", remaining_people_array[i]);
			} else {
				printf("%i,", remaining_people_array[i]);
			}
		}
		printf(")");
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// String.h replacement function. Compares two strings and returns 1 if they are equal and 0 if they are not. It
/// converts the strings to lowercase before comparing them. It uses the ASCII table to convert the characters to
/// lowercase.
///
/// @param first_string - The first string
/// @param second_string - The second string to compare with the first string
/// @return
///     1 - Strings are equal
///     0 - Strings are not equal
//
int areStringsEqual(char* first_string, char* second_string)
{
	// Convert the strings to lowercase
	for (int i = 0; first_string[i] != '\0'; ++i)
	{
		if (first_string[i] >= 'A' && first_string[i] <= 'Z')
		{
			first_string[i] += ASCII_BASE_CHAR_NUMBER;
		}
	}
	for (int i = 0; second_string[i] != '\0'; ++i)
	{
		if (second_string[i] >= 'A' && second_string[i] <= 'Z')
		{
			second_string[i] += ASCII_BASE_CHAR_NUMBER;
		}
	}
	while (*first_string != '\0' && *second_string != '\0')
	{
		if (*first_string != *second_string)
		{
			return FALSE;
		}
		++first_string;
		++second_string;
	}
	return *first_string == '\0' && *second_string == '\0';
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that splits a string by comma and returns an array of integers. It takes the input string and a pointer
/// to the size of the array as input. It loops through the input string and converts each substring to an integer. It
/// then saves the integers in a fixed-size array. It then allocates a new array with the exact size needed and copies
/// the integers from the fixed-size array to the new array. It then returns the new array. While doing this, it also
/// updates the size of the array that was passed as a pointer.
///
/// @param input_string - The string to split
/// @param size - The size of the array
/// @return array - The array of integers
//
int* splitStringByComma(char* input_string, int* size)
{
	int array[MAX_SPLITSTRING_ARRAY_SIZE]; // Fixed-size array
	*size = 0; // Counter for the number of integers
	while (*input_string != '\0')
	{
		char* end_ptr;
		long value = strtol(input_string, &end_ptr, 10);
		if (input_string == end_ptr)
		{
			printf("Error converting string to integer\n");
			return NULL;
		}
		array[*size] = (int) value;
		(*size)++;
		while (*end_ptr && *end_ptr != ',')
		{
			end_ptr++;
		}
		if (*end_ptr == ',')
		{
			end_ptr++;
		}
		input_string = end_ptr;
	}
	// Allocate a new array with the exact size needed
	int* final_array = (int*)malloc(*size * sizeof(int));
	if (final_array == NULL)
	{
		printf("Out of memory! Program terminated!\n");
		return NULL;
	}
	// Copy the integers from the fixed-size array to the new array
	for (int i = 0; i < *size; ++i)
	{
		final_array[i] = array[i];
	}
	return final_array;
}


//---------------------------------------------------------------------------------------------------------------------
///
/// A function that returns the length of a string. It takes a string as input and loops through it until it reaches
/// the null terminator. It then returns the length of the string.
///
/// @param string - The string to get the length of
/// @return length - The length of the string
//
int getStringLength(const char* string)
{
	int length = 0;
	while (string[length] != '\0')
	{
		length++;
	}
	return length;
}

// --------------------------------------------------------------------------------------------------------------------
///
/// A function that frees the allocated memory. It frees the memory for the list of people and the list of elevators.
///
/// @param number_of_floors - The number of floors of the hotel
/// @param person_list - The list of people waiting on each floor of the hotel
/// @param elevators - The list of elevators of the hotel
//
void freeAllocatedMemory(int number_of_floors, int number_of_elevators, Person** person_list, Elevator* elevators)
{
	if (person_list != NULL)
	{
		for (int i = 0; i < number_of_floors; ++i)
		{
			free(person_list[i]);
		}
	}
	free(person_list);

	if (elevators != NULL)
	{
		for (int i = 0; i < number_of_elevators; ++i)
		{
			free(elevators[i].person_list_);
		}
	}
	free(elevators);
}
