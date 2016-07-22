/*
Oscar Hedblad
03/12/2013
Marshall Tappen

DESCRIPTION: The program reads data from an input text file named "birthday.txt" and correctly sorts people
with respect to their date of birth. The program links the two people with the closest birthday together.
If there exists a tie; the program will sort by last name, alphabetically. If the two people not only share
the same birthday but also share the same last name, the program will sort alphabetically again, this time by first name.
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Declare struct "Person" */
typedef struct Person
{
	char fName[30];
	char lName[30];
	int birthday;
} Person;

/* Declaration of my functions */
void decreaseBirthday (Person *persons, int numPeople);
int convertBday (char month[], int day);
int findClosest (Person *persons, int index, int numPeople, int numDays);
void qsStruct(Person *persons, int left, int right);
void qsHelp(Person *persons, int array_size);
void sortlName(Person *persons, int numPeople);


int main(void)
{
/* Declaration of necessary variables */
	int i, j, k, l;
	char month[10];
	int day;
	int year;
	char qFirst[30];
	char qFinal[30];
	int found = 0;
	int index = 0;
	int closest = 0;
	int isLeapYear = 0;
	int numDays = 366;

	int num, numPeople, qNumber;

	FILE *ifp = fopen("birthday.txt", "r");

	fscanf(ifp, "%d", &num);

/* For-loop that controls the number of classes */
	for (i = 1; i <= num; i++)
	{
		printf("Class #%d:\n\n", i);

		fscanf(ifp, "%d", &numPeople);

/* Creates the struct array "persons" and allocate enough space */
		Person *persons = malloc((sizeof(Person)) * numPeople);
/* Sets leapYear back to zero and the number of days to 366 */
		isLeapYear = 0;
		numDays = 366;

/* For-loop that handles the number of persons */
		for (j = 0; j < numPeople; j++)
		{
			fscanf(ifp, "%s", persons[j].fName);
			fscanf(ifp, "%s", persons[j].lName);
			fscanf(ifp, "%s", month);
			fscanf(ifp, "%d", &day);
			fscanf(ifp, "%d", &year);

			if (!strcmp(month, "FEBRUARY") && day == 29)
				isLeapYear = 1;

/* Uses the convert function to convert the day of the birthday into an integer value */
			persons[j].birthday = convertBday(month, day);
		}

/* Checks leapYear; if equal to zero it sets the number of days to 365 */
		if (isLeapYear == 0)
			numDays = 365;

/* Uses the decreaseBirthday function; if it is NOT a leap year */
		if (isLeapYear == 0)
			decreaseBirthday(persons, numPeople);

/* Uses the quick sort function */
		qsHelp(persons, numPeople);

/* Uses the sortlName function to sort people by their last name, alphabetically */
		sortlName(persons, numPeople);

		fscanf(ifp, "%d", &qNumber);

/* For-loop that handles the number of queries */
		for (k = 0; k < qNumber; k++)
		{
			fscanf(ifp, "%s", qFirst);
			fscanf(ifp, "%s", qFinal);

/* Search for the index for queried person */
			l = 0;
			found = 0;

			while (l < numPeople && found != 1 ) {
				if (!strcmp(persons[l].fName, qFirst) && !strcmp(persons[l].lName, qFinal)) {
					found = 1;
					index = l;
				}
				l++;
			}

/* Use the function findClosest to find closest birthday */
			closest = findClosest(persons, index, numPeople, numDays);

/* Prints the output */
			printf("%s %s has the closest birthday to %s %s.\n", persons[closest].fName,
				   persons[closest].lName, persons[index].fName, persons[index].lName);
		}

		free(persons);

		printf("\n");
	}

	fclose(ifp);
	return 0;
}

/* Decreases all birthdays by ONE */
void decreaseBirthday (Person *persons, int numPeople)
{
	int i = 0;

	for (i = 0; i < numPeople; i++)
	{
		if (persons[i].birthday > 60)
			persons[i].birthday -= 1;
	}
}


/* Generates an integer value for birthday */
int convertBday (char month[], int day)
{
	int birthday = 0;

	if (strcmp(month, "JANUARY") == 0)
		birthday = 0 + day;
	else if (strcmp(month, "FEBRUARY") == 0)
		birthday = 31 + day;
	else if (strcmp(month, "MARCH") == 0)
		birthday = 60 + day;
	else if (strcmp(month, "APRIL") == 0)
		birthday = 91 + day;
	else if (strcmp(month, "MAY") == 0)
		birthday = 121 + day;
	else if (strcmp(month, "JUNE") == 0)
		birthday = 152 + day;
	else if (strcmp(month, "JULY") == 0)
		birthday = 182 + day;
	else if (strcmp(month, "AUGUST") == 0)
		birthday = 213 + day;
	else if (strcmp(month, "SEPTEMBER") == 0)
		birthday = 244 + day;
	else if (strcmp(month, "OCTOBER") == 0)
		birthday = 274 + day;
	else if (strcmp(month, "NOVEMBER") == 0)
		birthday = 305 + day;
	else if (strcmp(month, "DECEMBER") == 0)
		birthday = 335 + day;

	return birthday;
}


/* Identifies index with closest birthday */
int findClosest (Person *persons, int index, int numPeople, int numDays)
{
	int closest = 0;
	int difference1, difference2;

/* Case where only ONE Person in class */
	if (numPeople == 1)
		closest = 0;

/* First index */
	else if (index == 0){
		difference1 = (persons[0].birthday + numDays) - persons[numPeople-1].birthday;
		difference2 = persons[1].birthday - persons[0].birthday;

		if (difference1 > numDays/2)
			difference1 = numDays - difference1;
		if (difference2 > numDays/2)
			difference2 = numDays - difference2;

		if (difference1 < difference2)
			closest = numPeople-1;
		else
			closest = 1;
	}
/* Final index */
	else if (index == (numPeople-1)) {
		difference1 = persons[index].birthday - persons[index-1].birthday;
		difference2 = (persons[0].birthday + numDays) - persons[numPeople-1].birthday;

		if (difference1 > numDays/2)
			difference1 = numDays - difference1;
		if (difference2 > numDays/2)
			difference2 = numDays - difference2;

		if (difference1 < difference2)
			closest = index - 1;
		else
			closest = 0;
	}
/* Remaining indexes */
	else {
		difference1 = persons[index].birthday - persons[index-1].birthday;
		difference2 = persons[index+1].birthday - persons[index].birthday;

		if (difference1 > numDays/2)
			difference1 = numDays - difference1;
		if (difference2 > numDays/2)
			difference2 = numDays - difference2;

		if (difference1 < difference2)
			closest = index-1;
		else
			closest = index+1;
	}
	return closest;
}


/* Modified the basic quick sort function for integer arrays in order for it to be able to sort structs
with integer components in it */
void qsStruct(Person *persons, int left, int right)
{

	int fakePivot, l_hold, r_hold;
	Person pivot;

	l_hold = left;
	r_hold = right;
	pivot = persons[left];
	while (left < right)
	{
		while ((persons[right].birthday >= pivot.birthday) && (left < right))
			right--;
		if (left != right)
		{
			persons[left] = persons[right];
			left++;
		}
		while ((persons[left].birthday <= pivot.birthday) && (left < right))
			left++;
		if (left != right)
		{
			persons[right] = persons[left];
			right--;
		}
	}
	persons[left] = pivot;
	fakePivot = left;
	left = l_hold;
	right = r_hold;
	if (left < fakePivot)
		qsStruct(persons, left, fakePivot-1);
	if (right > fakePivot)
		qsStruct(persons, fakePivot+1, right);
}

/* Aid for the quick sort function (with better parameters) */
void qsHelp(Person *persons, int array_size)
{
	qsStruct(persons, 0, array_size - 1);
}



/* Sorts alphabetically according to last name */
void sortlName(Person *persons, int numPeople)
{
	int i, swap = 1, numSort = numPeople - 1;
	Person temp;

/* Handles the first tie-breaker: If two birthdays are equal, sort the people alphabetically by last name */
	while (swap) {
		swap = 0;
		for (i = 0; i < numSort; i++)
		{
			if (persons[i].birthday == persons[i+1].birthday)
			{
				if (strcmp (persons[i].lName, persons[i+1].lName) > 0)
				{
					temp = persons[i];
					persons[i] = persons[i+1];
					persons[i+1] = temp;
					swap = 1;
				}
			}
		}
		numSort--;
	}

/* Handles second tie-breaker: If the last name are equal, sort names alphabetically by first name */
	swap = 1;
	numSort = numPeople-1;

	while (swap) {
		swap = 0;
		for (i = 0; i < numSort; i++)
		{
			if (!strcmp(persons[i].lName, persons[i+1].lName) && persons[i].birthday == persons[i+1].birthday)
			{
				if (strcmp (persons[i].fName, persons[i+1].fName) > 0)
				{
					temp = persons[i];
					persons[i] = persons[i+1];
					persons[i+1] = temp;
					swap = 1;
				}
			}
		}
		numSort--;
	}
}
