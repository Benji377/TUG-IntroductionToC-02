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
Elevator *initializeElevators(int number_of_elevators, int elevator_capacity, int number_of_floors);
int showInitialStateInput();
void printHotel(char *hotel_name, int number_of_floors, int number_of_elevators,
								Person **person_list, int number_of_people, Elevator *elevators);
int shouldPrintAllSteps();
int allGuestsHaveReachedDestination(Person **person_list, int number_of_people, int number_of_floors);
void simulationStep(int number_of_floors, int number_of_elevators,
										Person **person_list, int number_of_guests, Elevator *elevators);
void removePeopleFromElevator(int number_of_elevators, Elevator *elevators);
void fillElevatorWithPeople(int number_of_elevators, int number_of_guests, Elevator *elevators, Person **person_list);
void freeAllocatedMemory(int number_of_floors, Person **person_list, Elevator *elevators);
void changeElevatorDirection(int number_of_floors, int number_of_elevators, Elevator *elevators);

// String helper functions
int *splitStringByComma(char *input_string);
int stringLength(const char *input_string);
int areStringsEqual(const char *first_string, const char *second_string);


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
	if (showInitialState == 1)
	{
		printHotel(hotel_name, number_of_floors, number_of_elevators, person_list,
							 number_of_people_waiting, elevator_list);
	}

	// Start simulation
	printf("Start the simulation? (\"start\"/\"skip\"):\n > ");
	char startSimulation[10];
	scanf("%s", startSimulation);
	if (areStringsEqual(startSimulation, "start"))
	{
		int printSteps = shouldPrintAllSteps();


		// As long as not all guests have reached their destination, keep simulating
		while (!allGuestsHaveReachedDestination(person_list, number_of_people_waiting, number_of_floors))
		{
			simulationStep(number_of_floors, number_of_elevators, person_list,
										 number_of_people_waiting, elevator_list);
			if (printSteps == 1)
			{
				printHotel(hotel_name, number_of_floors, number_of_elevators, person_list,
									 number_of_people_waiting, elevator_list);
			}
		}
		printHotel(hotel_name, number_of_floors, number_of_elevators, person_list,
							 number_of_people_waiting, elevator_list);

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
}

void getNumberOfFloorsInput(char *hotel_name, int *number_of_floors)
{
	do
	{
		printf("Enter the number of floors in hotel %s:\n > ", hotel_name);
		scanf("%d", number_of_floors);
		if ((*number_of_floors) < 3 || (*number_of_floors) > 10)
		{
			printf("Wrong input, the number of floors must be between 3 and 10!\n");
		}
	} while ((*number_of_floors) < 3 || (*number_of_floors) > 10);
}

void getNumberOfElevatorsInput(char *hotel_name, int *number_of_elevators)
{
	do
	{
		printf("Enter the number of elevators in hotel %s:\n > ", hotel_name);
		scanf("%d", number_of_elevators);
		if ((*number_of_elevators) < 1 || (*number_of_elevators) > 5) {
			printf("Wrong input, the number of elevators must be between 1 and 5!\n");
		}
	} while ((*number_of_elevators) < 1 || (*number_of_elevators) > 5);
}

void getElevatorsCapacityInput(char *hotel_name, int *elevator_capacity)
{
	do
	{
		printf("Enter the capacity of elevators in hotel %s:\n > ", hotel_name);
		scanf("%d", elevator_capacity);
		if ((*elevator_capacity) < 1 || (*elevator_capacity) > 9)
		{
			printf("Wrong input, the capacity of elevators must be between 1 and 9 person(s)!\n");
		}
	} while ((*elevator_capacity) < 1 || (*elevator_capacity) > 9);
}

void getNumberOfPeopleWaitingOnEachFloor(int *number_of_people_waiting)
{
	do {
		printf("Enter the number of people waiting on each floor:\n > ");
		scanf("%d", number_of_people_waiting);
		if ((*number_of_people_waiting) < 2 || (*number_of_people_waiting) > 20)
		{
			printf("Wrong input, the number of people waiting on each floor must be between 2 and 20!\n");
		}
	} while ((*number_of_people_waiting) < 2 || (*number_of_people_waiting) > 20);
}

Person **initializeListOfPeople(int number_of_floors, int number_of_people_waiting)
{
	Person **person_list = (Person**) malloc(number_of_floors * sizeof(Person *));

	for (int floor = 0; floor < number_of_floors; ++floor)
	{
		person_list[floor] = malloc(number_of_people_waiting * sizeof(Person));
		printf("Enter the destination floors of the people [floor: %d]:\n > ", floor);

		char buffer[100];
		scanf("%s", buffer);
		int *splitInput = splitStringByComma(buffer);

		for (int guest_on_floor = 0; guest_on_floor < number_of_people_waiting; ++guest_on_floor)
		{
			Person *person = malloc(sizeof(Person));
			person->current_floor_ = floor;
			person->destination_floor_ = splitInput[guest_on_floor];
			person->is_inside_elevator_ = 0;
			person->has_reached_destination_ = 0;
			person_list[floor][guest_on_floor] = *person;
		}
	}
	return person_list;
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
	printf("Show the initial state? (\"yes\"/\"no\"):\n > ");
	char showInitialState[5];
	scanf("%s", showInitialState);
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


int shouldPrintAllSteps()
{
	int printSteps = 0;
	while (1)
	{
		// Show all steps or end result
		printf("Show all steps of the simulation? (\"all steps\"/\"end result\"):\n > ");
		char showAllSteps[20];

		scanf(" %[^\n]s", showAllSteps);
		printf("Input: %s\n", showAllSteps);

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

int areStringsEqual(const char *first_string, const char *second_string)
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

// A function that takes a string and returns its length without using the string.h library
int stringLength(const char *input_string)
{
	int length = 0;
	while (input_string[length] != '\0')
	{
		++length;
	}
	return length;
}

// A function that takes a string, splits it by comma and returns an array of strings
int *splitStringByComma(char *input_string)
{
	// A function that takes a string, splits it by the comma separator and returns an array of integers
	// Without using the string.h library
	int length = stringLength(input_string);
	int *split_string = malloc(length * sizeof(int));
	int split_string_index = 0;
	for (int i = 0; i < length; ++i)
	{
		if (input_string[i] != ',')
		{
			split_string[split_string_index] = input_string[i] - '0';
			++split_string_index;
		}
	}
	return split_string;
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
