//---------------------------------------------------------------------------------------------------------------------
// <filename>.c
//
// <Explanation of the program ...>
// <... May have multiple lines.>
//
// Group: <group name>
//
// Author: <student ID>
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
								Person **person_list, int number_of_people, Elevator *elevators);
int startSimulationInput();
int shouldPrintAllSteps();
int allGuestsHaveReachedDestination(Person **person_list, int number_of_people, int number_of_floors);
void simulationStep(int number_of_floors, int number_of_elevators,
										Person **person_list, int number_of_guests, Elevator *elevators);
void removePeopleFromElevator(int number_of_elevators, Elevator *elevators);
void fillElevatorWithPeople(int number_of_elevators, int number_of_guests, Elevator *elevators, Person **person_list);
void freeAllocatedMemory(int number_of_floors, Person **person_list, Elevator *elevators);
void changeElevatorDirection(int number_of_floors, int number_of_elevators, Elevator *elevators);

// String helper functions
int *splitStringByComma(char *input_string, int *size);
int areStringsEqual(char *first_string, char *second_string);

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
/// This is an example header comment. Here, you explain, what the
/// function does. Copypaste and adapt it!
///
/// @param numbers an array to check
/// @param length The length of the array. If this text is too long you can
///        continue at the next line
///
/// @return the highest number
//
int main()
{
	char hotel_name[50];
	int number_of_floors;
	int number_of_elevators;
	int elevator_capacity;
	int number_of_people_waiting;

	getHotelNameInput(hotel_name);
	getNumberOfFloorsInput(hotel_name, &number_of_floors);
	getNumberOfElevatorsInput(hotel_name, &number_of_elevators);
	getElevatorsCapacityInput(hotel_name, &elevator_capacity);
	getNumberOfPeopleWaitingOnEachFloor(&number_of_people_waiting);

	Person **person_list = initializeListOfPeople(number_of_floors, number_of_people_waiting);
	Elevator *elevator_list = initializeElevators(number_of_elevators, elevator_capacity, number_of_floors);


	// Show initial state
	int showInitialState = showInitialStateInput();
	if (showInitialState)
	{
		printHotel(hotel_name, number_of_floors, number_of_elevators, person_list,
							 number_of_people_waiting, elevator_list);
	}

	int start_simulation = startSimulationInput();
	if (start_simulation)
	{
		int printSteps = shouldPrintAllSteps();
		int steps_counter = 0;
		// As long as not all guests have reached their destination, keep simulating
		while (!allGuestsHaveReachedDestination(person_list, number_of_people_waiting, number_of_floors))
		{
			simulationStep(number_of_floors, number_of_elevators, person_list,
										 number_of_people_waiting, elevator_list);
			if (printSteps)
			{
				printHotel(hotel_name, number_of_floors, number_of_elevators, person_list,
									 number_of_people_waiting, elevator_list);
			}
			steps_counter++;
		}
		printf("\n=================\n   FINAL STATE\n=================\n");
		printHotel(hotel_name, number_of_floors, number_of_elevators, person_list,
							 number_of_people_waiting, elevator_list);

		printf("Simulation done in %i steps!", steps_counter);

		freeAllocatedMemory(number_of_floors, person_list, elevator_list);
		return 0;
	}
	else
	{
		freeAllocatedMemory(number_of_floors, person_list, elevator_list);
		return 0;
	}
}

void getHotelNameInput(char *hotel_name)
{
	printf("____________________\n"
				 " ____   ____   ____ \n"
				 "|____  |____  |____|\n"
				 "|____   ____| |     \n"
				 "____________________\n\n"
				 "Welcome to the\n"
				 "Elevator Simulation Program!\n\n");

	// User inputs
	printf("Enter the name of the hotel:\n > ");
	scanf("%s", hotel_name);
	// String to all uppercase without string.h library with ASCII table values
	// hotel_name is an array of characters, so it is a pointer to the first character
	for (int i = 0; hotel_name[i] != '\0'; ++i)
	{
		if (hotel_name[i] >= 'a' && hotel_name[i] <= 'z')
		{
			hotel_name[i] -= 32;
		}
	}
}

void getNumberOfFloorsInput(char *hotel_name, int *number_of_floors)
{
	do
	{
		printf("Enter the number of floors in hotel %s:\n > ", hotel_name);
		scanf("%d", number_of_floors);
		if ((*number_of_floors) < MIN_NUMBER_OF_FLOORS || (*number_of_floors) > MAX_NUMBER_OF_FLOORS)
		{
			printf("Wrong input, the number of floors must be between %i and %i!\n", MIN_NUMBER_OF_FLOORS,
						 MAX_NUMBER_OF_FLOORS);
		}
	} while ((*number_of_floors) < MIN_NUMBER_OF_FLOORS || (*number_of_floors) > MAX_NUMBER_OF_FLOORS);
}

void getNumberOfElevatorsInput(char *hotel_name, int *number_of_elevators)
{
	do
	{
		printf("Enter the number of elevators in hotel %s:\n > ", hotel_name);
		scanf("%d", number_of_elevators);
		if ((*number_of_elevators) < MIN_NUMBER_OF_ELEVATORS || (*number_of_elevators) > MAX_NUMBER_OF_ELEVATORS) {
			printf("Wrong input, the number of elevators must be between %i and %i!\n", MIN_NUMBER_OF_ELEVATORS,
						 MAX_NUMBER_OF_ELEVATORS);
		}
	} while ((*number_of_elevators) < MIN_NUMBER_OF_ELEVATORS || (*number_of_elevators) > MAX_NUMBER_OF_ELEVATORS);
}

void getElevatorsCapacityInput(char *hotel_name, int *elevator_capacity)
{
	do
	{
		printf("Enter the capacity of elevators in hotel %s:\n > ", hotel_name);
		scanf("%d", elevator_capacity);
		if ((*elevator_capacity) < MIN_ELEVATOR_CAPACITY || (*elevator_capacity) > MAX_ELEVATOR_CAPACITY)
		{
			printf("Wrong input, the capacity of elevators must be between %i and %i person(s)!\n",
						 MIN_ELEVATOR_CAPACITY, MAX_ELEVATOR_CAPACITY);
		}
	} while ((*elevator_capacity) < MIN_ELEVATOR_CAPACITY || (*elevator_capacity) > MAX_ELEVATOR_CAPACITY);
}

void getNumberOfPeopleWaitingOnEachFloor(int *number_of_people_waiting)
{
	do {
		printf("Enter the number of people waiting on each floor:\n > ");
		scanf("%d", number_of_people_waiting);
		if ((*number_of_people_waiting) < MIN_NUMBER_OF_PEOPLE_WAITING ||
				(*number_of_people_waiting) > MAX_NUMBER_OF_PEOPLE_WAITING)
		{
			printf("Wrong input, the number of people waiting on each floor must be between %i and %i!\n",
						 MIN_NUMBER_OF_PEOPLE_WAITING, MAX_NUMBER_OF_PEOPLE_WAITING);
		}
	} while ((*number_of_people_waiting) < MIN_NUMBER_OF_PEOPLE_WAITING ||
					 (*number_of_people_waiting) > MAX_NUMBER_OF_PEOPLE_WAITING);
}

Person **initializeListOfPeople(int number_of_floors, int number_of_people_waiting)
{
	Person **person_list = (Person**) malloc(number_of_floors * sizeof(Person *));

	for (int floor = 0; floor < number_of_floors; ++floor)
	{
		person_list[floor] = malloc(number_of_people_waiting * sizeof(Person));
		int *splitInput = getDestinationFloorInput(number_of_floors, floor);

		for (int guest_on_floor = 0; guest_on_floor < number_of_people_waiting; ++guest_on_floor)
		{
			Person *person = malloc(sizeof(Person));
			person->current_floor_ = floor;
			person->destination_floor_ = splitInput[guest_on_floor];
			person->is_inside_elevator_ = 0;
			person->has_reached_destination_ = 0;
			person_list[floor][guest_on_floor] = *person;
		}
		free(splitInput);
	}
	return person_list;
}

int *getDestinationFloorInput(int number_of_floors, int floor)
{
	int *splitInput;
	int input_size;
	int errors;
	do {
		printf("Enter the destination floors of the people [floor: %d]:\n > ", floor);

		char buffer[100];
		scanf("%99s", buffer);
		// empty the buffer
		while ((getchar()) != '\n');
		splitInput = splitStringByComma(buffer, &input_size);
		errors = 0;

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

Elevator *initializeElevators(int number_of_elevators, int elevator_capacity, int number_of_floors)
{
	Elevator *elevators = malloc(number_of_elevators * sizeof(Elevator));
	for (int i = 0; i < number_of_elevators; ++i)
	{
		Elevator *elevator = malloc(sizeof(Elevator));
		elevator->capacity_ = elevator_capacity;
		// If it's an odd number, start at the top floor and go down, else start at the bottom floor and go up
		if (i % 2 != 0)
		{
			elevator->current_floor_ = number_of_floors - 1;
			elevator->direction_ = -1;
		}
		else
		{
			elevator->current_floor_ = 0;
			elevator->direction_ = 1;
		}
		elevator->person_list_ = NULL;

		elevators[i] = *elevator;
	}
	return elevators;
}

int showInitialStateInput()
{
	char showInitialState[5];
	do {
		printf("Show the initial state? (\"yes\"/\"no\"):\n > ");
		scanf("%s", showInitialState);
		// empty the buffer
		while ((getchar()) != '\n');
	} while (areStringsEqual(showInitialState, "yes") != 1 ||
						areStringsEqual(showInitialState, "no") != 1);

	if (areStringsEqual(showInitialState, "yes"))
	{
		printf("\n=================\n  INITIAL STATE\n=================\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

void printHotel(char *hotel_name, int number_of_floors, int number_of_elevators,
								Person **person_list, int number_of_people, Elevator *elevators)
{
	printf("\n++-----------++\n");
	printf("++   %s   ++\n", hotel_name);
	printf("++-----+-----++\n");


	for (int i = number_of_floors - 1; i >= 0; --i)
	{
		printf("|");
		for (int j = 0; j < number_of_elevators; ++j)
		{
			if (elevators[j].current_floor_ == i)
			{
				printf("| [%d] |",
							 elevators[j].capacity_ - elevators[j].person_list_[elevators[j].capacity_ - 1]->is_inside_elevator_);
			}
			else
			{
				printf("|     |");
			}
		}
		printf("|  (");

		for (int j = 0; j < number_of_people; ++j)
		{
			if (person_list[i][j].current_floor_ == i)
			{
				if (person_list[i][j].is_inside_elevator_ == 1 || person_list[i][j].has_reached_destination_ == 1)
				{
					printf("-");
				}
				else
				{
					printf("%d", person_list[i][j].destination_floor_);
				}
			}
			// Print a comma if the guest is not the last one on the current floor
			if (j != number_of_people - 1)
			{
				printf(",");
			}
		}
		printf(")\n");
		printf("||-   -+-   -||\n");
	}
	printf("++-----+-----++\n\n");
}

int startSimulationInput()
{
	char startSimulation[10];

	do {
		printf("Start the simulation? (\"start\"/\"skip\"):\n > ");
		scanf("%9s", startSimulation);
		// empty the buffer
		while ((getchar()) != '\n');
	} while (areStringsEqual(startSimulation, "start") != 1 ||
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


int shouldPrintAllSteps()
{
	int printSteps = 0;
	while (1)
	{
		// Show all steps or end result
		printf("Show all steps of the simulation? (\"all steps\"/\"end result\"):\n > ");
		char showAllSteps[20];

		scanf(" %[^\n]19s", showAllSteps);
		// empty the buffer
		while ((getchar()) != '\n');

		if (areStringsEqual(showAllSteps, "all steps"))
		{
			printSteps = 1;
			break;
		} else if (areStringsEqual(showAllSteps, "end result"))
		{
			break;
		}
		else
		{
			printf("Invalid input. Please enter either \"all steps\" or \"end result\".\n");
		}
	}
	return printSteps;
}

// A function that checks if all guests have reached their destination. If true, return 1, else return 0
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

void simulationStep(int number_of_floors, int number_of_elevators,
										Person **person_list, int number_of_guests, Elevator *elevators)
{

	// For each guest in the elevator, check if the guest has reached their destination floor
	// If the guest has reached their destination floor, remove them from the elevator
	removePeopleFromElevator(number_of_elevators, elevators);

	// For each guest on the same floor as the elevator, check if the elevator is going in their direction_
	// If the elevator is going in their direction, check if there is space in the elevator
	// If there is space in the elevator, add the guest to the elevator
	fillElevatorWithPeople(number_of_elevators, number_of_guests, elevators, person_list);

	// For each elevator, check if it is at the top or bottom floor and change its direction_ if it is
	changeElevatorDirection(number_of_floors, number_of_elevators, elevators);
	// For each elevator, go one step in the current direction_
	for (int i = 0; i < number_of_elevators; ++i)
	{
		elevators[i].current_floor_ += elevators[i].direction_;
	}
}

void removePeopleFromElevator(int number_of_elevators, Elevator *elevators)
{
	for (int i = 0; i < number_of_elevators; ++i)
	{
		for (int j = 0; j < elevators[i].capacity_; ++j)
		{
			if (elevators[i].person_list_[j]->has_reached_destination_ == 1)
			{
				elevators[i].person_list_[j]->is_inside_elevator_ = 0;
				elevators[i].person_list_[j]->has_reached_destination_ = 1;
				printf("A person reached their destination floor %d with elevator %d.\n",
							 elevators[i].current_floor_, i);
				elevators[i].person_list_[j] = NULL;
			}
		}
	}
}

void fillElevatorWithPeople(int number_of_elevators, int number_of_guests, Elevator *elevators, Person **person_list)
{
	for (int i = 0; i < number_of_elevators; ++i)
	{
		for (int j = 0; j < number_of_guests; ++j)
		{
			if (elevators[i].direction_ == 1 &&
					person_list[elevators[i].current_floor_][j].destination_floor_ > elevators[i].current_floor_)
			{
				for (int k = 0; k < elevators[i].capacity_; ++k)
				{
					if (elevators[i].person_list_[k]->is_inside_elevator_ == 0 &&
							elevators[i].person_list_[k]->has_reached_destination_ == 0)
					{
						elevators[i].person_list_[k] = &person_list[elevators[i].current_floor_][j];
						person_list[elevators[i].current_floor_][j].is_inside_elevator_ = 1;
						break;
					}
				}
			}
			else if (elevators[i].direction_ == -1 &&
							 person_list[elevators[i].current_floor_][j].destination_floor_ < elevators[i].current_floor_)
			{
				for (int k = 0; k < elevators[i].capacity_; ++k)
				{
					if (elevators[i].person_list_[k]->is_inside_elevator_ == 0 &&
							elevators[i].person_list_[k]->has_reached_destination_ == 0) {
						elevators[i].person_list_[k] = &person_list[elevators[i].current_floor_][j];
						person_list[elevators[i].current_floor_][j].is_inside_elevator_ = 1;
						break;
					}
				}
			}
		}
	}
}

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

int areStringsEqual(char *first_string, char *second_string)
{
	while (*first_string != '\0' && *second_string != '\0')
	{
		if (*first_string != *second_string)
		{
			return 0;  // Strings are not equal
		}
		++first_string;
		++second_string;
	}
	printf("String are equal: %i\n", *first_string == '\0' && *second_string == '\0');
	return *first_string == '\0' && *second_string == '\0';  // Check if both strings reached the end simultaneously
}

// A function that takes a string, splits it by comma and returns an array of strings
int *splitStringByComma(char *input_string, int *size)
{
	int* array = NULL;  // Initialize the array to NULL
	*size = 0;          // Initialize the size of the array

	while (*input_string)
	{
		// Use strtol to extract integers from the string
		char *end_ptr;
		long value = strtol(input_string, &end_ptr, 10);

		// Check for conversion errors
		if (input_string == end_ptr)
		{
			// Conversion failed
			fprintf(stderr, "Error converting string to integer\n");
			break;
		}

		// Increment the size of the array
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



void freeAllocatedMemory(int number_of_floors, Person **person_list, Elevator *elevators)
{
	// Free allocated memory
	for (int i = 0; i < number_of_floors; ++i)
	{
		free(person_list[i]);
	}
	free(person_list);
	free(elevators);
}
