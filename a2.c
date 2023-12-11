//---------------------------------------------------------------------------------------------------------------------
// a2.c
//
// This program simulates the operation of an arbitrary number of elevators in a hotel with an arbitrary number of floors.
// You can read more in detail about it in the README.md file.
//
// Group: Matthias Bergmann
//
// Author: 12320035
//---------------------------------------------------------------------------------------------------------------------
//

#include <stdio.h>
#include <stdlib.h>

// Structs
typedef struct _Person_
{
	int current_floor_;
	int destination_floor_;
	int is_inside_elevator_; // 1 for true, 0 for false
	int has_reached_destination_; // 1 for true, 0 for false
} Person;

typedef struct _Elevator_
{
	int capacity_;
	int current_floor_;
	int direction_; // 1 for up, -1 for down
	Person **person_list_;
} Elevator;

// Function prototypes
void getHotelNameInput(char *hotel_name);
void getNumberOfFloorsInput(char *hotel_name, int *number_of_floors);
void getNumberOfElevatorsInput(char *hotel_name, int *number_of_elevators);
void getElevatorsCapacityInput(char *hotel_name, int *elevator_capacity);
void getNumberOfPeopleWaitingOnEachFloor(int *number_of_people_waiting);
Person **initializeListOfPeople(int number_of_floors, int number_of_people_waiting);
int *getDestinationFloorInput(int number_of_floors, int floor);
Elevator *initializeElevators(int number_of_elevators, int elevator_capacity, int number_of_floors);
int showInitialStateInput();
void printHotel(char *hotel_name, int number_of_floors, int number_of_elevators,
								Person **person_list, int number_of_people, Elevator *elevators, int elevator_capacity);
void printHotelNameDynamic(char *hotel_name, int number_of_elevators);
void printHotelElevators(const Elevator *elevators, int elevator_capacity, int current_floor, int current_elevator);
void printHotelPersonList(Person *const *person_list, int number_of_people, int current_floor, int current_guest);
void printElevatorDecorator(int number_of_elevators, int full_line);
int startSimulationInput();
int shouldPrintAllSteps();
int allGuestsHaveReachedDestination(Person **person_list, int number_of_people, int number_of_floors);
void simulationStep(int number_of_floors, int number_of_elevators, Person **person_list, int number_of_guests,
										Elevator *elevators, int elevator_capacity, int show_steps);
void moveElevator(int number_of_elevators, Elevator *elevators);
void removePeopleFromElevator(int number_of_elevators, Elevator *elevators, int elevator_capacity,  int show_steps);
void fillElevatorWithPeople(int number_of_elevators, int number_of_guests, int elevator_capacity,
														Elevator *elevators, Person **person_list, int number_of_floors);
void addPersonToElevator(int elevator_capacity, Elevator *elevators, Person **person_list,
												 int elevator_index, int person_index);
int getBestNextPersonIndex(Person **person_list, int number_of_people, int number_of_floors,
													 int current_floor, int elevator_direction);
void changeElevatorDirection(int number_of_floors, int number_of_elevators, Elevator *elevators);
void printElevatorState(Elevator *elevator, int number_of_elevators, int elevator_capacity);
int areAllElevatorsEmpty(const Elevator *elevator, int number_of_elevators, int elevator_capacity);
void sortPersonList(Person **person_list, int number_of_people, int number_of_floors);
void freeAllocatedMemory(int number_of_floors, int number_of_elevators, Person **person_list, Elevator *elevators);

// String helper functions
int *splitStringByComma(char *input_string, int *size);
int areStringsEqual(char *first_string, char *second_string);
int getStringLength(const char *string);

// Constants
const int MAX_NUMBER_OF_FLOORS = 10;
const int MIN_NUMBER_OF_FLOORS = 3;
const int MAX_NUMBER_OF_ELEVATORS = 5;
const int MIN_NUMBER_OF_ELEVATORS = 1;
const int MAX_ELEVATOR_CAPACITY = 9;
const int MIN_ELEVATOR_CAPACITY = 1;
const int MAX_NUMBER_OF_PEOPLE_WAITING = 20;
const int MIN_NUMBER_OF_PEOPLE_WAITING = 2;


//---------------------------------------------------------------------------------------------------------------------
///
/// The main function of the program. It takes the user input and calls the functions to initialize the hotel and
/// simulate the elevator operation.
///
///
/// @return
///     0 - Simulation not started
///     -1 - Memory allocation error
///      number of steps - Simulation completed successfully
//
int main()
{
	char hotel_name[50];
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
	Person **person_list = initializeListOfPeople(number_of_floors, number_of_people_waiting);
	Elevator *elevator_list = initializeElevators(number_of_elevators, elevator_capacity, number_of_floors);
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
			sortPersonList(person_list, number_of_people_waiting, number_of_floors);
			if (print_all_steps)
			{
				printf("=== Simulation Step %i ===\n", steps_counter+1);
			}
			// Calculates the next step of the simulation
			simulationStep(number_of_floors, number_of_elevators, person_list,number_of_people_waiting,
										 elevator_list, elevator_capacity, print_all_steps);
			if (print_all_steps)
			{
				// Prints the current state of the simulation
				printHotel(hotel_name, number_of_floors, number_of_elevators, person_list, number_of_people_waiting,
									 elevator_list, elevator_capacity);
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
/// A function that takes the name of the hotel as input and converts it to uppercase.
///
/// @param hotel_name - The name of the hotel
//
void getHotelNameInput(char *hotel_name)
{
	printf("____________________\n"
				 " ____   ____   ____ \n"
				 "|____  |____  |____|\n"
				 "|____   ____| |     \n"
				 "____________________\n\n"
				 "Welcome to the\n"
				 "Elevator Simulation Program!\n\n");
	printf("Enter the name of the hotel:\n > ");
	scanf("%s", hotel_name);
	for (int i = 0; hotel_name[i] != '\0'; ++i)
	{
		if (hotel_name[i] >= 'a' && hotel_name[i] <= 'z')
		{
			hotel_name[i] -= 32;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that takes the number of floors of the hotel as input and checks if it is in the range of
/// MIN_NUMBER_OF_FLOORS and MAX_NUMBER_OF_FLOORS.
///
/// @param hotel_name - The name of the hotel
/// @param number_of_floors - The number of floors of the hotel
//
void getNumberOfFloorsInput(char *hotel_name, int *number_of_floors)
{
	do
	{
		printf("Enter the number of floors in hotel %s:\n > ", hotel_name);
		scanf("%i", number_of_floors);
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
/// MIN_NUMBER_OF_ELEVATORS and MAX_NUMBER_OF_ELEVATORS.
///
/// @param hotel_name - The name of the hotel
/// @param number_of_elevators - The number of elevators of the hotel
//
void getNumberOfElevatorsInput(char *hotel_name, int *number_of_elevators)
{
	do
	{
		printf("Enter the number of elevators in hotel %s:\n > ", hotel_name);
		scanf("%i", number_of_elevators);
		if ((*number_of_elevators) < MIN_NUMBER_OF_ELEVATORS || (*number_of_elevators) > MAX_NUMBER_OF_ELEVATORS) {
			printf("Wrong input, the number of elevators must be between %i and %i!\n", MIN_NUMBER_OF_ELEVATORS,
						 MAX_NUMBER_OF_ELEVATORS);
		}
	} while ((*number_of_elevators) < MIN_NUMBER_OF_ELEVATORS || (*number_of_elevators) > MAX_NUMBER_OF_ELEVATORS);
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that takes the capacity of elevators of the hotel as input and checks if it is in the range of
/// MIN_ELEVATOR_CAPACITY and MAX_ELEVATOR_CAPACITY.
///
/// @param hotel_name - The name of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
//
void getElevatorsCapacityInput(char *hotel_name, int *elevator_capacity)
{
	do
	{
		printf("Enter the capacity of elevators in hotel %s:\n > ", hotel_name);
		scanf("%i", elevator_capacity);
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
/// range of MIN_NUMBER_OF_PEOPLE_WAITING and MAX_NUMBER_OF_PEOPLE_WAITING.
///
/// @param number_of_people_waiting - The number of people waiting on each floor of the hotel
//
void getNumberOfPeopleWaitingOnEachFloor(int *number_of_people_waiting)
{
	do {
		printf("Enter the number of people waiting on each floor:\n > ");
		scanf("%i", number_of_people_waiting);
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
/// the list of people.
///
/// @param number_of_floors - The number of floors of the hotel
/// @param number_of_people_waiting - The number of people waiting on each floor of the hotel
///
/// @return person_list - The list of people waiting on each floor of the hotel
//
Person **initializeListOfPeople(int number_of_floors, int number_of_people_waiting)
{
	Person **person_list = (Person**) malloc(number_of_floors * sizeof(Person *));
	for (int floor = 0; floor < number_of_floors; ++floor)
	{
		person_list[floor] = malloc(number_of_people_waiting * sizeof(Person));
		int *splitInput = getDestinationFloorInput(number_of_floors, floor);
		for (int guest_on_floor = 0; guest_on_floor < number_of_people_waiting; ++guest_on_floor)
		{
			Person person = {floor, splitInput[guest_on_floor], 0, 0};
			person_list[floor][guest_on_floor] = person;
		}
		free(splitInput);
	}
	return person_list;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A helper function to initialize the list of people that takes the number of floors and the current floor as input
/// and returns an array of destination floors for the people on the current floor.
///
/// @param number_of_floors - The number of floors of the hotel
/// @param floor - The current floor
///
/// @return splitInput - The array of destination floors
//
int* getDestinationFloorInput(int number_of_floors, int floor)
{
	int *splitInput;
	int input_size;
	int errors;
	do
	{
		printf("Enter the destination floors of the people [floor: %d]:\n > ", floor);
		char buffer[100];
		scanf("%99s", buffer);
		// empty the buffer
		while ((getchar()) != '\n');
		splitInput = splitStringByComma(buffer, &input_size);
		errors = 0;

		//if (input_size != number_of_floors)
		//{
		//	printf("Wrong input, the number of destination floors must be %i!\n", number_of_floors);
		//	errors++;
		//}
		for (int i = 0; i < input_size; i++)
		{
			if (splitInput[i] == floor)
			{
				printf("Wrong input, the destination floor cannot be the current floor!\n");
				errors++;
			}
			else if (splitInput[i] < 0 || splitInput[i] >= number_of_floors)
			{
				printf("Wrong input, the destination floor %i is out of range (0 to %i)! \n",
							 splitInput[i], number_of_floors - 1);
				errors++;
			}
		}
	} while (errors > 0);
	return splitInput;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that takes the number of elevators, the capacity of elevators and the number of floors as input and
/// initializes the list of elevators.
///
/// @param number_of_elevators - The number of elevators of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
/// @param number_of_floors - The number of floors of the hotel
///
/// @return elevators - The list of elevators of the hotel
//
Elevator *initializeElevators(int number_of_elevators, int elevator_capacity, int number_of_floors)
{
	Elevator *elevators = malloc(number_of_elevators * sizeof(Elevator));
	for (int i = 0; i < number_of_elevators; ++i)
	{
		Elevator elevator = {elevator_capacity, 0, 1, NULL};
		// If it's an odd number, start at the top floor and go down, else start at the bottom floor and go up
		if (i % 2 != 0)
		{
			elevator.current_floor_ = number_of_floors - 1;
			elevator.direction_ = -1;
		}
		else
		{
			elevator.current_floor_ = 0;
			elevator.direction_ = 1;
		}
		elevator.person_list_ = malloc(elevator_capacity * sizeof(Person));
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
/// A function that asks the user if they want to see the initial state of the simulation.
///
/// @return 1 - Show the initial state
///         0 - Don't show the initial state
//
int showInitialStateInput()
{
	char showInitialState[5];
	do {
		printf("Show the initial state? (\"yes\"/\"no\"):\n > ");
		scanf("%s", showInitialState);
		// empty the buffer
		while ((getchar()) != '\n');
	} while (areStringsEqual(showInitialState, "yes") != 1 &&
						areStringsEqual(showInitialState, "no") != 1);
	if (areStringsEqual(showInitialState, "yes"))
	{
		printf("\n=================\n  INITIAL STATE\n=================\n\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that prints the hotel with the current state of the elevators and the people waiting on each floor.
///
/// @param hotel_name - The name of the hotel
/// @param number_of_floors - The number of floors of the hotel
/// @param number_of_elevators - The number of elevators of the hotel
/// @param person_list - The list of people waiting on each floor of the hotel
/// @param number_of_people - The number of people waiting on each floor of the hotel
/// @param elevators - The list of elevators of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
//
void printHotel(char *hotel_name, int number_of_floors, int number_of_elevators, Person **person_list,
								int number_of_people, Elevator *elevators, int elevator_capacity)
{
	printHotelNameDynamic(hotel_name, number_of_elevators);
	printElevatorDecorator(number_of_elevators, 1);
	for (int i = number_of_floors - 1; i >= 0; --i)
	{
		printf("||");
		for (int j = 0; j < number_of_elevators; ++j)
		{
			printHotelElevators(elevators, elevator_capacity, i, j);
		}
		printf("|  (");
		for (int j = 0; j < number_of_people; ++j)
		{
			printHotelPersonList(person_list, number_of_people, i, j);
		}
		printf(")\n");
		if (i != 0)
		{
			printElevatorDecorator(number_of_elevators, 0);
		}
	}
	printElevatorDecorator(number_of_elevators, 1);
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that prints the name of the hotel in the middle of the top line of the hotel.
///
/// @param hotel_name - The name of the hotel
/// @param number_of_elevators - The number of elevators of the hotel
//
void printHotelNameDynamic(char *hotel_name, int number_of_elevators)
{
	int number_of_minus = number_of_elevators * 5 + number_of_elevators-1;
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
			for (int i = 0; i < remaining_space / 2+1; ++i)
			{
				printf(" ");
			}
		}
		else {
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
/// A function that prints the decorator of the hotel.
///
/// @param number_of_elevators - The number of elevators of the hotel
/// @param full_line - 1 for a full line, 0 for a line with a gap in the middle
//
void printElevatorDecorator(int number_of_elevators, int full_line)
{
	if (full_line)
	{
		printf("++");
	}
	else
	{
		printf("||");
	}
	for (int j = 0; j < number_of_elevators; ++j)
	{
		if (full_line)
		{
			printf("-----");
		}
		else
		{
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
/// A function that prints the list of people waiting on each floor of the hotel.
///
/// @param person_list - The list of people waiting on each floor of the hotel
/// @param number_of_people - The number of people waiting on each floor of the hotel
/// @param current_floor - The current floor
/// @param current_guest - The current guest
//
void printHotelPersonList(Person *const *person_list, int number_of_people, int current_floor, int current_guest)
{
	if (person_list[current_floor][current_guest].current_floor_ == current_floor)
	{
		if (person_list[current_floor][current_guest].is_inside_elevator_ == 1 ||
				person_list[current_floor][current_guest].has_reached_destination_ == 1)
		{
			printf("-");
		}
		else
		{
			printf("%d", person_list[current_floor][current_guest].destination_floor_);
		}
	}
	// Print a comma if the guest is not the last one on the current floor
	if (current_guest != number_of_people - 1)
	{
		printf(",");
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that prints the elevators of the hotel.
///
/// @param elevators - The list of elevators of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
/// @param current_floor - The current floor
/// @param current_elevator - The current elevator
//
void printHotelElevators(const Elevator *elevators, int elevator_capacity, int current_floor, int current_elevator)
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
/// A function asks the user if they want to start the simulation.
///
/// @return 1 if the user wants to start the simulation
///         0 if the user wants to skip the simulation
//
int startSimulationInput()
{
	char startSimulation[10];
	do {
		printf("Start the simulation? (\"start\"/\"skip\"):\n > ");
		scanf("%9s", startSimulation);
		while ((getchar()) != '\n');
	} while (areStringsEqual(startSimulation, "start") != 1 &&
					 areStringsEqual(startSimulation, "skip") != 1);
	if (areStringsEqual(startSimulation, "start"))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that asks the user if they want to see all steps of the simulation or just the end result.
///
/// @return 1 if the user wants to see all steps of the simulation
///         0 if the user wants to see just the end result
//
int shouldPrintAllSteps()
{
	int printSteps = 0;
	while (1)
	{
		printf("Show all steps of the simulation? (\"all steps\"/\"end result\"):\n > ");
		char showAllSteps[20];
		scanf(" %[^\n]19s", showAllSteps);
		while ((getchar()) != '\n');
		if (areStringsEqual(showAllSteps, "all steps"))
		{
			printSteps = 1;
			break;
		} else if (areStringsEqual(showAllSteps, "end result"))
		{
			break;
		}
	}
	return printSteps;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that checks if all guests have reached their destination.
///
/// @param person_list - The list of people waiting on each floor of the hotel
/// @param number_of_people - The number of people waiting on each floor of the hotel
/// @param number_of_floors - The number of floors of the hotel
/// @return 1 if all guests have reached their destination
///         0 if not all guests have reached their destination
//
int allGuestsHaveReachedDestination(Person **person_list, int number_of_people, int number_of_floors)
{
	for (int i = 0; i < number_of_floors; ++i)
	{
		for (int j = 0; j < number_of_people; ++j)
		{
			if (person_list[i][j].has_reached_destination_ == 0)
			{
				return 0;
			}
		}
	}
	return 1;
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
void simulationStep(int number_of_floors, int number_of_elevators, Person **person_list, int number_of_guests,
										Elevator *elevators, int elevator_capacity, int show_steps)
{
	changeElevatorDirection(number_of_floors, number_of_elevators, elevators);
	removePeopleFromElevator(number_of_elevators, elevators, elevator_capacity, show_steps);
	fillElevatorWithPeople(number_of_elevators, number_of_guests, elevator_capacity, elevators, person_list, number_of_floors);
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that moves each elevator in their respective direction.
///
/// @param number_of_elevators
/// @param elevators
//
void moveElevator(int number_of_elevators, Elevator *elevators)
{
	for (int i = 0; i < number_of_elevators; ++i)
	{
		elevators[i].current_floor_ += elevators[i].direction_;
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that removes people from the elevator if they have reached their destination floor
///
/// @param number_of_elevators - The number of elevators of the hotel
/// @param elevators - The list of elevators of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
/// @param show_steps - 1 if the user wants to see all steps of the simulation
//
void removePeopleFromElevator(int number_of_elevators, Elevator *elevators, int elevator_capacity, int show_steps)
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
				elevators[i].person_list_[j]->is_inside_elevator_ = 0;
				elevators[i].person_list_[j]->has_reached_destination_ = 1;
				if (show_steps)
				{
					printf("A person reached their destination floor %d with elevator %d.\n",
								 elevators[i].current_floor_, i);
				}
				elevators[i].person_list_[j] = NULL;
				elevators[i].capacity_++;
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that adds people to the elevator if they are on the same floor as the elevator and are going in the same
/// direction_ as the elevator.
///
/// @param number_of_elevators - The number of elevators of the hotel
/// @param number_of_guests - The number of people waiting on each floor of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
/// @param elevators - The list of elevators of the hotel
/// @param person_list - The list of people waiting on each floor of the hotel
//
void fillElevatorWithPeople(int number_of_elevators, int number_of_guests, int elevator_capacity, Elevator *elevators,
														Person **person_list, int number_of_floors)
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
/// A function that loops through the list of people waiting on each floor and adds them to the elevator if they are on
/// the same floor as the elevator and are going in the same direction_ as the elevator.
///
/// @param elevator_capacity - The capacity of elevators of the hotel
/// @param elevators - The list of elevators of the hotel
/// @param person_list - The list of people waiting on each floor of the hotel
/// @param elevator_index - The index of the elevator
/// @param person_index - The index of the person
//
void addPersonToElevator(int elevator_capacity, Elevator *elevators, Person **person_list,
												 int elevator_index, int person_index)
{
	for (int k = 0; k < elevator_capacity; ++k)
	{
		if (elevators[elevator_index].person_list_[k] == NULL)
		{
			elevators[elevator_index].person_list_[k] = &person_list[elevators[elevator_index].current_floor_][person_index];
			person_list[elevators[elevator_index].current_floor_][person_index].is_inside_elevator_ = 1;
			elevators[elevator_index].capacity_--;
			return;
		}
	}
}

int getBestNextPersonIndex(Person **person_list, int number_of_people, int number_of_floors, int current_floor, int elevator_direction)
{
	int best_next_person_index = -1;
	int best_next_person_distance = number_of_floors;
	for (int i = 0; i < number_of_people; ++i)
	{
		if (person_list[current_floor][i].current_floor_ == current_floor &&
				person_list[current_floor][i].is_inside_elevator_ == 0 &&
				person_list[current_floor][i].has_reached_destination_ == 0)
		{
			if (elevator_direction == 1) {
				if (person_list[current_floor][i].destination_floor_ > current_floor) {
					if (person_list[current_floor][i].destination_floor_ - current_floor < best_next_person_distance) {
						best_next_person_distance = person_list[current_floor][i].destination_floor_ - current_floor;
						best_next_person_index = i;
					}
				}
			} else if (elevator_direction == -1) {
				if (person_list[current_floor][i].destination_floor_ < current_floor) {
					if (current_floor - person_list[current_floor][i].destination_floor_ < best_next_person_distance) {
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
/// A function that changes the direction_ of the elevator if it is at the top or bottom floor.
///
/// @param number_of_floors - The number of floors of the hotel
/// @param number_of_elevators - The number of elevators of the hotel
/// @param elevators - The list of elevators of the hotel
//
void changeElevatorDirection(int number_of_floors, int number_of_elevators, Elevator *elevators)
{
	for (int i = 0; i < number_of_elevators; ++i)
	{
		if (elevators[i].current_floor_ == number_of_floors - 1)
		{
			elevators[i].direction_ = -1;
		}
		else if (elevators[i].current_floor_ == 0)
		{
			elevators[i].direction_ = 1;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that prints the current state of the elevators. It prints the Index of the elevator and the list of
/// destination floors of the people inside the elevator.
///
/// @param elevator - The list of elevators of the hotel
/// @param number_of_elevators - The number of elevators of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
//
void printElevatorState(Elevator *elevator, int number_of_elevators, int elevator_capacity)
{
	int are_all_elevators_empty = areAllElevatorsEmpty(elevator, number_of_elevators, elevator_capacity);
	if (!are_all_elevators_empty)
	{
		printf("Elevators:\n");
		for (int i = 0; i < number_of_elevators; ++i)
		{
			if (elevator[i].capacity_ != elevator_capacity)
			{
				printf("%i: (", i);
				for (int j = 0; j < elevator_capacity; ++j)
				{
					if (elevator[i].person_list_[j] == NULL)
					{
						if (j == elevator_capacity - 1)
						{
							printf("-");
						}
						else
						{
							printf("-,");
						}
					} else {
						if (j == elevator_capacity - 1)
						{
							printf("%i", elevator[i].person_list_[j]->destination_floor_);
						}
						else
						{
							printf("%i,", elevator[i].person_list_[j]->destination_floor_);
						}
					}
				}
				printf(")\n");
			}
		}
	}
	printf("\n");
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A helper function that checks if all elevators are empty.
///
/// @param elevator - The list of elevators of the hotel
/// @param number_of_elevators - The number of elevators of the hotel
/// @param elevator_capacity - The capacity of elevators of the hotel
/// @return 1 if all elevators are empty
///         0 if not all elevators are empty
//
int areAllElevatorsEmpty(const Elevator *elevator, int number_of_elevators, int elevator_capacity)
{
	int are_all_elevators_empty = 1;
	for (int i = 0; i < number_of_elevators; ++i)
	{
		if (elevator[i].capacity_ != elevator_capacity)
		{
			are_all_elevators_empty = 0;
			break;
		}
	}
	return are_all_elevators_empty;
}

void swapPeople(Person *person1, Person *person2)
{
	Person temp = *person1;
	*person1 = *person2;
	*person2 = temp;
}

void sortPersonList(Person **person_list, int number_of_people, int number_of_floors)
{
	for (int i = 0; i < number_of_floors; ++i)
	{
		for (int j = 0; j < number_of_people; ++j)
		{
			for (int k = 0; k < number_of_people - j - 1; ++k)
			{
				if (person_list[i][k].destination_floor_ > person_list[i][k + 1].destination_floor_)
				{
					swapPeople(&person_list[i][k], &person_list[i][k + 1]);
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
///
/// String.h replacement function. Compares two strings and returns 1 if they are equal and 0 if they are not.
///
/// @param first_string - The first string
/// @param second_string - The second string to compare with the first string
/// @return
///     1 - Strings are equal
///     0 - Strings are not equal
//
int areStringsEqual(char *first_string, char *second_string)
{
	// Convert the strings to lowercase
	for (int i = 0; first_string[i] != '\0'; ++i)
	{
		if (first_string[i] >= 'A' && first_string[i] <= 'Z')
		{
			first_string[i] += 32;
		}
	}
	for (int i = 0; second_string[i] != '\0'; ++i)
	{
		if (second_string[i] >= 'A' && second_string[i] <= 'Z')
		{
			second_string[i] += 32;
		}
	}
	while (*first_string != '\0' && *second_string != '\0')
	{
		if (*first_string != *second_string)
		{
			return 0;
		}
		++first_string;
		++second_string;
	}
	return *first_string == '\0' && *second_string == '\0';
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that splits a string by comma and returns an array of integers.
///
/// @param input_string - The string to split
/// @param size - The size of the array
/// @return array - The array of integers
//
int *splitStringByComma(char *input_string, int *size)
{
	int* array = NULL;  // Initialize the array to NULL
	*size = 0;          // Initialize the size of the array to 0
	while (*input_string)
	{
		// Use strtol to extract integers from the string
		char *end_ptr;
		long value = strtol(input_string, &end_ptr, 10);
		if (input_string == end_ptr)
		{
			// Conversion failed
			fprintf(stderr, "Error converting string to integer\n");
			exit(-1);
			break;
		}
		(*size)++;
		// Reallocate memory for the array
		array = realloc(array, (*size) * sizeof(int));
		array[*size - 1] = (int)value;

		// Move to the next comma or the end of the string
		while (*end_ptr && *end_ptr != ',')
		{
			end_ptr++;
		}
		if (*end_ptr == '\0')
		{
			break;  // Exit the loop if the end of the string is reached
		}
		// Move past the comma
		end_ptr++;
		input_string = end_ptr;
	}
	return array;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// A function that returns the length of a string.
///
/// @param string - The string to get the length of
/// @return length - The length of the string
//
int getStringLength(const char *string)
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
/// @param number_of_floors - The number of floors of the hotel
/// @param person_list - The list of people waiting on each floor of the hotel
/// @param elevators - The list of elevators of the hotel
//
void freeAllocatedMemory(int number_of_floors, int number_of_elevators, Person **person_list, Elevator *elevators)
{
	for (int i = 0; i < number_of_floors; ++i)
	{
		free(person_list[i]);
	}
	free(person_list);

	for (int i = 0; i < number_of_elevators; ++i)
	{
		free(elevators[i].person_list_);
	}
	free(elevators);
}
