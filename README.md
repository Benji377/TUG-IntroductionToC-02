# Assignment 2 - Elevator Simulation Program

## Introduction
In Assignment 2 (A2) of the ESP practicals (KU), you will implement a program that simulates elevators in a hotel. Please make sure that you follow the assignment description strictly. Even slight differences can cause testcases to fail.

This document describes the structure of the assignment and the expected functionality of the program.

### Learning Goals
Arrays, Strings, Pointers, Memory Management/Heap

### Specifications

Some general specifications must be followed for every assignment in this course. A list of these specifications can be found here:
[Assessment of Assignments - Deductions](https://www.notion.so/coding-tugraz/Assessment-of-Assignments-How-do-you-get-your-points-d7cca6cc89a344e38ad74dd3ccb73cb3?pvs=4#e3203e50b1fc4ba787bdbf5fb46262ae). Please follow these specifications to avoid point deductions. As memory management is a central learning goal of this assignment, pay close attention to this section within the specifications.

Additionally, the following specifications must be followed for this assignment (A2) as well:
* **Attention:** The `<string.h>` library must **not** be used within this assignment! If `<string.h>` is used, this will result in a **deduction of up to -25 %**.
* **Attention:** `malloc()` (or `calloc()`) and `free()` must be used. ***All*** integer arrays whose size depends on entered values (e.g., number of floors and elevators, elevator capacity, people per floor) must be stored on the heap. If these arrays are not stored on the heap, this will result in a deduction of **up to -25 %**. For simplicity, you can use an array on the stack (e.g., ```char buffer[100];```) for string inputs (e.g., hotel name, storing yes/no, etc.). 

### Grading
Assignment 2 is worth **24 points**.

Information on how your assignment submission will be assessed can be found here: [Assessment of Assignments](https://www.notion.so/coding-tugraz/Assessment-of-Assignments-How-do-you-get-your-points-d7cca6cc89a344e38ad74dd3ccb73cb3).
Please read this carefully, as it also contains guidelines on what your final code should look like (Coding Standard, Style Guide).

Here you can find an overview of how your total grade for the KU will be calculated: [Procedure and Grading](https://www.notion.so/coding-tugraz/Procedure-and-Grading-59975fee4a9c4047867772cd20caf73d)

### Submission
- Deliverables: `a2.c`
- Push to your repository
- **Deadline: 6th December 2023, 23:59 (GMT+1)**

## Assignment Description

This program simulates the operation of an arbitrary number of elevators in a hotel with an arbitrary number of floors.
At the beginning, the user provides the program with parameters including the hotel name, the number of floors, the number of elevators and the position and destination floors of the guests waiting for the elevators.
Then, an illustration of the initial status of the elevators and guests is printed on the CLI (command line interface). 
Subsequently, the user can either start the simulation or terminate the program. 
The simulation runs in steps and, for each step, elevators move one floor upwards or downwards. 
Guests can enter and leave the elevators on each floor.
An illustration of each step is printed in the CLI. 
The simulation and, thus, the program are terminated once all the guests have arrived at their destination floor.

### General Notes
The notes given in this section are important for the entire assignment description:

- `\n` shouldn't be printed as separate characters, instead they represent the newline character.
- Note the leading and trailing spaces in the text fields! You can make them visible by selecting the text field.
- Words in angle brackets (such as `<HOTEL_NAME>`) should be replaced by calculated or given data. They are *not* part of the output itself! For a better understanding, you can look at the example outputs at the very bottom of this document.
- For each string-expected user input (marked with STRING in the title of the corresponding section), you can assume that an array of printable ASCII characters consisting of at least one character will be used. Other inputs like integers or non-printable characters will not be tested.
- The user input is **case-insensitive**. This means that `YES`, `yes` and `yEs` would all be valid inputs.
- For each integer-expected user input (marked with INT in the title of the corresponding section), you can assume that only integers will be used. You don't have to think about other inputs like characters or multiple integers separated by spaces. However, it is possible that values outside the valid integer range occur.
- You can assume that the **integer input does not have any extra whitespaces**. For **string input, you do not have to ignore the whitespaces**, but treat them as part of the input string instead.
- ` > ` (command prompt, always printed with a leading and trailing space) in the output indicates that the program should wait for user input at this point. Execution should only continue after the user has ended their input with `ENTER`.
- If the user's input is invalid, the corresponding error message should be printed (unless otherwise stated). After this, the question is asked again, the command prompt is printed, and the user can make a new input. This process is repeated as long as the input is invalid.
- Take care with your memory management! That means that when allocating memory on the heap, you always need to check that the system was able to provide that memory. If the program runs out of memory you should first free all memory previously allocated on the heap, then print the correct error message (`Out of memory! Program terminated!\n`) and terminate the program with the corresponding return value `-1`.

## Program Sequence


#### Welcome Message

At the beginning of the program, the user is welcomed with the following message:

```
____________________\n
 ____   ____   ____ \n
|____  |____  |____|\n
|____   ____| |     \n
____________________\n
\n
Welcome to the\n
Elevator Simulation Program!\n
\n
```

### User Inputs (Data)

---

<details>
<summary><h5>User Input: Hotel Name (STRING)</h5></summary>

The user is first prompted for the name of the hotel:

```
Enter the name of the hotel:\n
 > 
```
For the sake of simplicity, you can assume that the user's input will always fit into the upper part of the hotel illustration. Names that are longer will not be tested. Check out the State Output section (Simulation > Starting Phase > State Output) for better understanding!

> **Note:** From now on, <HOTEL_NAME> should always be replaced by the hotel name the user specified printed in CAPITAL letters!
</details>

<details>
<summary><h5>User Input: Number of Floors (INT)</h5></summary>

The user is prompted for the number of floors. The hotel must have at least 3 and no more than 10 floors. 

```
Enter the number of floors in hotel <HOTEL_NAME>:\n
 > 
```

If the user input is invalid, the following error message will be printed:

```
Wrong input, the number of floors must be between 3 and 10!\n
```
</details>

<details>
<summary><h5>User Input: Number of Elevators (INT)</h5></summary>

Next, the user is prompted for the number of elevators. The hotel must have at least 1 and no more than 5 elevators. 

```
Enter the number of elevators in hotel <HOTEL_NAME>:\n
 > 
```

If the user input is invalid, the following error message will be printed:

```
Wrong input, the number of elevators must be between 1 and 5!\n
```
</details>

<details>
<summary><h5>User Input: Elevator Capacity (INT)</h5></summary>

The elevators have a certain capacity (number of people that can fit at the same time). This capacity is the same for all elevators in the hotel.
An elevator has a minimum capacity of 1 person and a maximum capacity of 9 people. The user is required to enter the elevator capacity as follows:

```
Enter the capacity of elevators in hotel <HOTEL_NAME>:\n
 > 
```

If the user input is invalid, the following error message will be printed:

```
Wrong input, the capacity of elevators must be between 1 and 9 person(s)!\n
```
</details>

<details>
<summary><h5>User Input: Number of Waiting People (INT)</h5></summary>

On each floor, there is a number of people who are waiting for an elevator. For simplicity, this number is going to be the same for all floors.
On each floor, there are at least 2 and at most 20 people waiting for an elevator. The user is required to enter this number as follows:

```
Enter the number of people waiting on each floor:\n
 > 
```

If the user input is invalid, the following error message will be printed:

```
Wrong input, the number of people waiting on each floor must be between 2 and 20!\n
```
</details>

<details>
<summary><h5>User Input: Destination Floors</h5></summary>

For each of the floors, the user is required to enter the destination floor of each person:

```
Enter the destination floors of the people [floor: <FLOOR_NUMBER>]:\n
 > 
```
The user needs to enter the destination floors as a comma-separated list of integers. For example, for the first floor of a hotel with 5 floors where 5 people are waiting on each floor, it could look like this:

```
Enter the destination floors of the people [floor: 1]:\n
 > 0,3,2,4,4
```
Only a comma-separated list of integers is a valid input, and no other inputs will be tested. There are certain invalid input possibilities which you need to handle. Here is a table of possible violations and the corresponding messages:

| Violation                                                             | Error Message |
|-----------------------------------------------------------------------|------------|
| Entered destination floor is out of range (0 to NUMBER_OF_FLOORS - 1) | ```Wrong input, the destination floor <ENTERED_FLOOR> is out of range (0 to <NUMBER_OF_FLOORS - 1>)!\n```       |
| Entered destination floor is the current floor                            | ```Wrong input, the destination floor cannot be the current floor!\n```       |

For simplicity, the list entered is always <NUMBER_OF_PEOPLE_WAITING_ON_EACH_FLOOR> long. Longer or shorter lists are not tested. 

The input for a floor is each time read in its entirety, and each value is checked in the order it was entered, even if an error occurs. In such a case, a message is printed for each error. Then, the user is prompted to repeat the input for the current floor (`Enter the destination floors of ...`).

</details>

### Simulation

---

<details>
<summary><h4>Starting Phase</h4></summary>

Arrays shall be set up to store all the information about the hotel, the elevators and the guests. You are free to choose a configuration
of arrays that suits your program, but they **must all be stored on the heap**.

If the memory allocation fails, the following error message should be printed:

```
Out of memory! Program terminated!\n
```
Then, the program should terminate with the return value `-1` (see also Simulation > Final Phase > Return Values). All previously allocated memory must be freed before terminating the program.

##### User Input: Show Initial State (STRING)

After everything has been set up, the program should be ready to show an illustration of the initial state of the elevators and guests. 

First, the user is asked if they want to see it:

```
Show the initial state? ("yes"/"no"):\n
 > 
```

Valid inputs are `yes` or `no`. 
If the user input is invalid, the program should **not** print an error message and just repeat the question and the command prompt until a valid command is entered.

If the answer is `yes`, the initial state is shown. If the answer is `no`, the program directly proceeds to execute the Running Phase (Simulation > Running Phase).

The initial state is printed with the following header:

```
\n
=================\n
  INITIAL STATE\n
=================\n
\n 
```
It is followed by the current (initial) State Output (Simulation > Starting Phase > State Output). Take a look at the example initial output to see what that looks like (Simulation > Starting Phase > Example Initial Output).

##### State Output

Whenever the state of the elevators is printed, it should look like this:

```
++-----------------++\n
++  <HOTEL_NAME>   ++\n
++-----+-----+-----++\n
||     |[<P>]|     ||  (<D1>,<D2>,<...>,<Dn>)\n
||-   -+-   -+-   -||\n
||     |     |     ||  (<D1>,<D2>,<...>,<Dn>)\n
||-   -+-   -+-   -||\n
||[<P>]|     |[<P>]||  (<D1>,<D2>,<...>,<Dn>)\n
++-----+-----+-----++\n
Elevators:\n
0: (<ED1>,<ED2>,<...>,<EDec>)\n
1: (<ED1>,<ED2>,<...>,<EDec>)\n
2: (<ED1>,<ED2>,<...>,<EDec>)\n
\n
```

If all the elevators are empty, the section starting with ```Elevators:\n``` is not printed. In this case, only the hotel illustration is printed. The state output would then look like this:
  ```
  ++-----------------++\n
  ++  <HOTEL_NAME>   ++\n
  ++-----+-----+-----++\n
  ||     |[<P>]|     ||  (<D1>,<D2>,<...>,<Dn>)\n
  ||-   -+-   -+-   -||\n
  ||     |     |     ||  (<D1>,<D2>,<...>,<Dn>)\n
  ||-   -+-   -+-   -||\n
  ||[<P>]|     |[<P>]||  (<D1>,<D2>,<...>,<Dn>)\n
  ++-----+-----+-----++\n
  \n
  ```
> **Note:** : The hotel illustration depends on the number of elevators and floors. In the example above, we have 3 elevators and 3 floors. For the other hotel sizes, the illustration should be extended accordingly (see Example Outputs).

- Elevators are represented with `[<P>]`. `<P>` should be replaced by the number of people currently in the elevator.
- Elevators are numbered from left to right, starting with 0. This is visible in the elevator lists following the hotel illustration whenever there are people in the elevators.
- Each elevator has a fixed starting position. Elevators with even numbers have their starting position on the GROUND floor and will start going UPWARDS once the simulation starts.
Elevators with odd numbers start at the TOP floor and will start going DOWNWARDS once the simulation starts.
- The list of waiting people is printed in brackets to the right of each floor. The placeholders `<D1>` to `<Dn>` are replaced with the destination floors of the waiting people. The index "n" represents the number of people waiting on each floor.  
  > **Attention:** The list is sorted in **ascending** order of the destination floors. People who have already left their starting floor are shown in the list of waiting people with the symbol `-`, and these are printed before the waiting persons: ```(-,-,<Dx>,<...>,<Dz>)``` (see Example Outputs).
- The hotel name is centered in the top segment of the output. If an exact alignment is not possible, the hotel name starts one space to the left.
- If there are any people in the elevators, the list of elevators is printed with the list of people in them. The placeholders `<ED1>` to `<EDec>` are replaced with the destination floors of the people currently in the elevator. The index "ec" represents the elevator capacity.
  > **Attention:** The list is sorted in **ascending** order of the destination floors. For the empty places in the elevator, the symbol `-` is printed. Empty places are printed before taken places.

##### Example Initial Output

For example, for a simple hotel with 5 floors and 3 elevators, with 5 guests on each floor, the initial state may look like this:

```
\n
=================\n
  INITIAL STATE\n
=================\n
\n 
++-----------------++\n
++   SIMPLEHOTEL   ++\n
++-----+-----+-----++\n
||     | [0] |     ||  (0,1,2,2,3)\n
||-   -+-   -+-   -||\n
||     |     |     ||  (0,0,1,1,4)\n
||-   -+-   -+-   -||\n
||     |     |     ||  (0,1,3,3,4)\n
||-   -+-   -+-   -||\n
||     |     |     ||  (0,0,2,4,4)\n
||-   -+-   -+-   -||\n
|| [0] |     | [0] ||  (1,1,2,2,3)\n
++-----+-----+-----++\n
\n
```
</details>

<details>
<summary><h4>Running Phase</h4></summary>

##### User Input: Start Simulation (STRING)

After the input data has been provided to the program and, optionally, the initial state has been shown, the user is asked if they want to start the simulation:

```
Start the simulation? ("start"/"skip"):\n
 > 
```
Valid inputs are `start` or `skip`. 
If the user input is invalid, the program should **not** print an error message and just repeat the question and the command prompt until a valid command is entered.

If the input is `skip`, the program terminates with the return value `0` (see Simulation > Final Phase > Return Values).
If the input is `start`, the program continues with the next step. 

##### User Input: Results Display Options (STRING)

The user has the option to view an illustration of the hotel for each step of the simulation, or just the end result:

```
Show all steps of the simulation? ("all steps"/"end result"):\n
 > 
```

Valid inputs are `all steps` or `end result`. If the user input is invalid, the program should **not**
print an error message and just repeat the question and the command prompt until a valid command is entered.

##### Simulation Steps

The simulation runs in steps until all people have arrived at their destination floors. There is a simulation step counter which starts at `0` (for the initial state).

Each step of the simulation is executed in the following order: 

- The simulation step counter is increased by `1`. 
- If the user has chosen to show all steps, a step header with the number of the current step is printed:
```
=== Simulation Step <STEP_COUNTER> ===\n
```
- For each of the elevators (starting from the elevator number 0), the following procedure occurs:

  - If there are people in the elevator whose destination floor is the current floor, the people leave the elevator. These people are no longer needed in the rest of the program, so they can be deleted. For each of them the following message is printed:
    ```
    A person reached their destination floor <CURRENT_FLOOR> with elevator <ELEVATOR_NUMBER>.\n
    ```
  - People waiting on the current floor enter the elevator if their destination floor is in the current direction (up or down) of the elevator. People can only enter the elevator until the maximum capacity is reached.
  If there are more people meeting the specified criteria waiting for the elevator than the elevator can carry, those closest to their destination floors will board first. If multiple people have the same shortest distance and the elevator reaches its maximum capacity, the decision of who gets on and who waits can be made freely.
- Once the procedure is completed for all elevators, if the user has opted to display all steps, the current state is presented as outlined in State Output (Simulation > Starting Phase > State Output).

  For example:
  ``` 
  ++-----------------++\n
  ++   SIMPLEHOTEL   ++\n
  ++-----+-----+-----++\n
  ||     |     |     ||  (-,-,-,0,1)\n
  ||-   -+-   -+-   -||\n
  ||     | [3] |     ||  (-,0,0,1,4)\n
  ||-   -+-   -+-   -||\n
  ||     |     |     ||  (0,1,3,3,4)\n
  ||-   -+-   -+-   -||\n
  || [3] |     | [3] ||  (-,-,-,0,0)\n
  ||-   -+-   -+-   -||\n
  ||     |     |     ||  (-,-,-,-,-)\n
  ++-----+-----+-----++\n
  Elevators:\n
  0: (2,2,4)\n
  1: (1,2,2)\n
  2: (2,3,4)\n
  \n
  ```
- If all people have reached their destination floor, the simulation ends (see Simulation > Final Phase). Otherwise, for each of the elevators, the following occurs:

  - The elevator moves one floor in its current direction (UPWARDS/DOWNWARDS).
  - If an elevator is now on the GROUND/TOP floor, it changes direction.

</details>

<details>
<summary><h4>Final Phase</h4></summary>

The simulation terminates immediately after all people have reached their destination floors.
Now, the final state of the elevators should be shown as described in State Output (Simulation > Starting Phase > State Output). 
This final state is always printed, independently of whether the user has chosen to show all steps.
The list of elevators below the hotel illustration is not printed anymore, because all elevators are empty.

The final state is printed with this header:
```
\n
=================\n
   FINAL STATE\n
=================\n
\n
```
It is followed by the current (final) state (see Simulation > Starting Phase > State Output).

For example:
``` 
\n
=================\n
   FINAL STATE\n
=================\n
\n 
++-----------------++\n
++   SIMPLEHOTEL   ++\n
++-----+-----+-----++\n
||     |     |     ||  (-,-,-,-,-)\n
||-   -+-   -+-   -||\n
||     | [0] |     ||  (-,-,-,-,-)\n
||-   -+-   -+-   -||\n
||     |     |     ||  (-,-,-,-,-)\n
||-   -+-   -+-   -||\n
|| [0] |     | [0] ||  (-,-,-,-,-)\n
||-   -+-   -+-   -||\n
||     |     |     ||  (-,-,-,-,-)\n
++-----+-----+-----++\n
\n
```
After that the following message is printed:
```
Simulation done in <NUMBER_OF_STEPS_THE_SIMULATION_TOOK> steps!\n
```

Afterwards, the program should terminate and **return the number of steps taken by the simulation**. 

### Return Values

Here is an overview of the expected return values of the program:

|             Scenario              |        Return value        |
|:---------------------------------:|:--------------------------:|
| Memory allocation error occurred  |             -1             |
|      Simulation not started       |             0              |
| Simulation completed successfully | Number of simulation steps |

</details>

### Example Outputs

---

<details>
<summary><h4>Example 1</h4></summary>

```
____________________
 ____   ____   ____ 
|____  |____  |____|
|____   ____| |     
____________________

Welcome to the
Elevator Simulation Program!

Enter the name of the hotel:
 > RepeatInPUtHotEl
Enter the number of floors in hotel REPEATINPUTHOTEL:
 > 0
Wrong input, the number of floors must be between 3 and 10!
Enter the number of floors in hotel REPEATINPUTHOTEL:
 > 3
Enter the number of elevators in hotel REPEATINPUTHOTEL:
 > 9
Wrong input, the number of elevators must be between 1 and 5!
Enter the number of elevators in hotel REPEATINPUTHOTEL:
 > 3
Enter the capacity of elevators in hotel REPEATINPUTHOTEL:
 > 1
Enter the number of people waiting on each floor:
 > 200
Wrong input, the number of people waiting on each floor must be between 2 and 20!
Enter the number of people waiting on each floor:
 > 2
Enter the destination floors of the people [floor: 0]:
 > 3,4
Wrong input, the destination floor 3 is out of range (0 to 2)!
Wrong input, the destination floor 4 is out of range (0 to 2)!
Enter the destination floors of the people [floor: 0]:
 > 0,0
Wrong input, the destination floor cannot be the current floor!
Wrong input, the destination floor cannot be the current floor!
Enter the destination floors of the people [floor: 0]:
 > 1,2
Enter the destination floors of the people [floor: 1]:
 > 0,2
Enter the destination floors of the people [floor: 2]:
 > 0,1
Show the initial state? ("yes"/"no"):
 > yey
Show the initial state? ("yes"/"no"):
 > yes

=================
  INITIAL STATE
=================

++-----------------++
++REPEATINPUTHOTEL ++
++-----+-----+-----++
||     | [0] |     ||  (0,1)
||-   -+-   -+-   -||
||     |     |     ||  (0,2)
||-   -+-   -+-   -||
|| [0] |     | [0] ||  (1,2)
++-----+-----+-----++

Start the simulation? ("start"/"skip"):
 > strt
Start the simulation? ("start"/"skip"):
 > start
Show all steps of the simulation? ("all steps"/"end result"):
 > end
Show all steps of the simulation? ("all steps"/"end result"):
 > end result
A person reached their destination floor 1 with elevator 0.
A person reached their destination floor 1 with elevator 1.
A person reached their destination floor 2 with elevator 0.
A person reached their destination floor 0 with elevator 1.
A person reached their destination floor 2 with elevator 2.
A person reached their destination floor 0 with elevator 0.

=================
   FINAL STATE
=================

++-----------------++
++REPEATINPUTHOTEL ++
++-----+-----+-----++
||     | [0] |     ||  (-,-)
||-   -+-   -+-   -||
||     |     |     ||  (-,-)
||-   -+-   -+-   -||
|| [0] |     | [0] ||  (-,-)
++-----+-----+-----++

Simulation done in 5 steps!
```
</details>

<details>
<summary><h4>Example 2</h4></summary>

```
____________________
 ____   ____   ____ 
|____  |____  |____|
|____   ____| |     
____________________

Welcome to the
Elevator Simulation Program!

Enter the name of the hotel:
 > hOtel
Enter the number of floors in hotel HOTEL:
 > 3
Enter the number of elevators in hotel HOTEL:
 > 2
Enter the capacity of elevators in hotel HOTEL:
 > 1
Enter the number of people waiting on each floor:
 > 2
Enter the destination floors of the people [floor: 0]:
 > 1,2
Enter the destination floors of the people [floor: 1]:
 > 0,2
Enter the destination floors of the people [floor: 2]:
 > 0,1
Show the initial state? ("yes"/"no"):
 > yes

=================
  INITIAL STATE
=================

++-----------++
++   HOTEL   ++
++-----+-----++
||     | [0] ||  (0,1)
||-   -+-   -||
||     |     ||  (0,2)
||-   -+-   -||
|| [0] |     ||  (1,2)
++-----+-----++

Start the simulation? ("start"/"skip"):
 > start
Show all steps of the simulation? ("all steps"/"end result"):
 > all steps
=== Simulation Step 1 ===
++-----------++
++   HOTEL   ++
++-----+-----++
||     | [1] ||  (-,0)
||-   -+-   -||
||     |     ||  (0,2)
||-   -+-   -||
|| [1] |     ||  (-,2)
++-----+-----++
Elevators:
0: (1)
1: (1)

=== Simulation Step 2 ===
A person reached their destination floor 1 with elevator 0.
A person reached their destination floor 1 with elevator 1.
++-----------++
++   HOTEL   ++
++-----+-----++
||     |     ||  (-,0)
||-   -+-   -||
|| [1] | [1] ||  (-,-)
||-   -+-   -||
||     |     ||  (-,2)
++-----+-----++
Elevators:
0: (2)
1: (0)

=== Simulation Step 3 ===
A person reached their destination floor 2 with elevator 0.
A person reached their destination floor 0 with elevator 1.
++-----------++
++   HOTEL   ++
++-----+-----++
|| [1] |     ||  (-,-)
||-   -+-   -||
||     |     ||  (-,-)
||-   -+-   -||
||     | [1] ||  (-,-)
++-----+-----++
Elevators:
0: (0)
1: (2)

=== Simulation Step 4 ===
++-----------++
++   HOTEL   ++
++-----+-----++
||     |     ||  (-,-)
||-   -+-   -||
|| [1] | [1] ||  (-,-)
||-   -+-   -||
||     |     ||  (-,-)
++-----+-----++
Elevators:
0: (0)
1: (2)

=== Simulation Step 5 ===
A person reached their destination floor 0 with elevator 0.
A person reached their destination floor 2 with elevator 1.
++-----------++
++   HOTEL   ++
++-----+-----++
||     | [0] ||  (-,-)
||-   -+-   -||
||     |     ||  (-,-)
||-   -+-   -||
|| [0] |     ||  (-,-)
++-----+-----++


=================
   FINAL STATE
=================

++-----------++
++   HOTEL   ++
++-----+-----++
||     | [0] ||  (-,-)
||-   -+-   -||
||     |     ||  (-,-)
||-   -+-   -||
|| [0] |     ||  (-,-)
++-----+-----++

Simulation done in 5 steps!
```
</details>
