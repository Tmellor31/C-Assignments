#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_PARK_SIZE 20
#define SIZE 6
#define BOLLARD '#'
#define SPACE '.'
#define MAX_CARS 26
#define MAX_CARPARKS 50 

// cars must be of length 2
// use fscanf to populate the grid
// the file the user wants to use is always the last one entered - argv

struct CarPark
{
  char layout[MAX_PARK_SIZE][MAX_PARK_SIZE];
  int size;
  int height;
  int width;
};
typedef struct CarPark CarPark;

struct Car
{
  int length;
  bool orientation;
  char label;
  int startrow;
  int startcol;
};
typedef struct Car Car;
// make orientation an enum - for now true = vertical, false = horizontal

struct CarList
{
  Car cars[MAX_CARS];
  int total;
};
typedef struct CarList CarList;

struct CarParkTree
{
  CarPark carparks[MAX_CARPARKS];
  int total; 
};
typedef struct CarParkTree CarParkTree; 



void printpark(CarPark *park);
bool vehiclecheck(CarPark *park);
void test(void);
bool printcell(CarPark *park, int row, int col);
bool findcars(CarList *car_list, CarPark *park);
int find_car_position(CarList *car_list, char letter);
Car make_car(int length, char label, int startrow, int startcol);
int move_car_vertically(Car *car, CarPark *park);
int move_car_horizontally(Car *car, CarPark *park);
void update_car(Car *car, CarPark *park, int row, int col);
CarParkTree make_car_park_tree(void)
CarList make_car_list(void);

int main(int argc, char *argv[])
{
  test();
  FILE *fpin = fopen(argv[1], "r");

  if (fpin == NULL)
  {
    printf("Cannot open file\n");
    exit(EXIT_FAILURE);
  }

  if (argc < 2)
  {
    fprintf(stderr, "Usage : %s <file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  if (!fpin)
  {
    fprintf(stderr, "Cannot read from %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  char c = fgetc(fpin);
  while (c != EOF)
  {
    printf("%c\n", c);
    c = fgetc(fpin);
  }

  printf("test");
  fclose(fpin);
  return EXIT_SUCCESS;
}

/*bool read_park_from_file (CarPark *park)
{
  char *fgets(str, FILE * )
}*/

CarParkTree make_car_park_tree(void)
{
  CarParkTree park_tree; 
  park_tree.total = 0; 
  return park_tree;  
}

int move_car_vertically(Car *car, CarPark *park)
{
  int rowposition = car->startrow;
  int colposition = car->startcol;
  char letter = car->label;
  int moveflags = 0;
  if (car->orientation == true)

  {
    if (park->layout[rowposition - 1][colposition] == SPACE)
    { // can move up
      moveflags += 1;
    }
    if (park->layout[rowposition + car->length][colposition] == SPACE)
    { // can move down
      moveflags += 2;
    }
  }
  return moveflags;
}

int move_car_horizontally(Car *car, CarPark *park)
{
  int rowposition = car->startrow;
  int colposition = car->startcol;
  char letter = car->label;
  int moveflags = 0;
  if (car->orientation == false)
  {
    printf("%c\n", park->layout[rowposition][colposition - 1]);
    printf("%c\n", park->layout[rowposition][colposition + 1]);
    if (park->layout[rowposition][colposition - 1] == SPACE)
    { // can move left
      moveflags += 1;
    }
    if (park->layout[rowposition][colposition + car->length] == SPACE)
    { // can move right
      moveflags += 2;
    }
  }
  return moveflags;
}

int find_car_position(CarList *car_list, char letter)
{

  for (int index = 0; index < MAX_CARS; index++)
  {
    if (car_list->cars[index].label == letter)
    {
      return index;
    }
  }
  return -1;
}

bool findcars(CarList *car_list, CarPark *park)
{
  for (int row = 0; row < park->size; row++)
  {
    for (int col = 0; col < park->size; col++)
    {
      if (park->layout[row][col] != BOLLARD && park->layout[row][col] != SPACE)
      {
        int car_position = find_car_position(car_list, park->layout[row][col]);
        if (car_position == -1)
        {
          car_list->cars[car_list->total] = make_car(1, park->layout[row][col], row, col);
          car_list->total++;
        }
        else
        {
          update_car(&(car_list->cars[car_position]), park, row, col);
        }
      }
    }
  }
  return true;
}

CarList make_car_list(void)
{
  CarList car_list;
  car_list.total = 0;
  return car_list;
}

Car make_car(int length, char label, int startrow, int startcol)
{
  Car car;
  car.length = length;
  car.label = label;
  car.startrow = startrow;
  car.startcol = startcol;
  return car;
}

void update_car(Car *car, CarPark *park, int row, int col)
{
  (car->length)++;
  if (park->layout[row][col] == park->layout[row - 1][col])
  {
    car->orientation = true;
  }
  if (park->layout[row][col] == park->layout[row][col - 1])
  {
    car->orientation = false;
  }
}

void printpark(CarPark *park)
{
  int width;
  int height;
  for (int row = 0; row < SIZE; row++)
  {
    width++;
    for (int col = 0; col < SIZE; col++)
    {
      printf("%c", park->layout[row][col]);
      height++;
    }
    printf("\n");
  }
}

bool vehiclecheck(CarPark *park)
{ // function that checks for vehicles and if none are found the car park is not added.
  for (int row = 0; row < SIZE; row++)
  {
    for (int col = 0; col < SIZE; col++)
    {
      if (park->layout[row][col] != BOLLARD && park->layout[row][col] != SPACE)
      {
        return true;
      }
    }
  }
  return false;
}

bool iterate(CarPark *park, bool (*callback)(CarPark *park, int row, int col))
{ // Function that is used throughout the program to check or edit the car park.
  for (int row = 0; row < SIZE; row++)
  {
    for (int col = 0; col < SIZE; col++)
    {
      if (callback(park, row, col))
      {
        return true;
      }
    }
  }
  return false;
}

bool strtopark(CarPark *park, char *str)
{
  if (strlen(str) == 0)
  {
    return false;
  }
  if (strlen(str) % 2 != 0)
  {
    return false;
  }

  park->size = sqrt(strlen(str));
  int row, col;
  for (row = 0; row < park->size; row++)
  {
    for (col = 0; col < park->size; col++)
    {
      park->layout[row][col] = str[col + (row * park->size)];
    }
  }
  return true;
}

bool printcell(CarPark *park, int row, int col)
{ // todo: add a if statement that adds a \n after each row
  printf("%c", park->layout[row][col]);
  return true;
}

void test(void)
{

  CarList car_list = make_car_list();
  CarPark park;
  CarPark park2;

  strtopark(&park, "#.####.BBB.##A...##A...##A...#######");
  strtopark(&park2, "#.####.BBB.##A...##A...##A...#######");
  assert(printcell(&park, 6, 6) == true);
  assert(vehiclecheck(&park) == true);
  assert(findcars(&car_list, &park) == true);
  assert(car_list.total == 2);
  assert(find_car_position(&car_list, 'B') == 0);
  assert(find_car_position(&car_list, 'A') == 1);
  assert(car_list.cars[0].orientation == false);
  assert(move_car_vertically(&(car_list.cars[0]), &park) == 0);
  assert(move_car_horizontally(&(car_list.cars[0]), &park) == 3);
  assert(move_car_horizontally(&(car_list.cars[1]), &park) == 0);
  assert(move_car_vertically(&(car_list.cars[1]), &park) == 0);
  // printpark(&park);
  iterate(&park, &printcell);
  strtopark(&park, "#.####.....##....##....##....#######");
  // assert(find_car_position(&cars, 'B') == -1);
  assert(vehiclecheck(&park) == false);
  assert(vehiclecheck(&park2) == true);
}

// Use a while loop, compare the initial car park with the possible vehicle moves, and add any possible children to the end of the list.

// Can either store as a 2D array and it becomes easier to print, or store as an array of vehicles and its easier to solve.

// Ideas on how to add car park - can create a 'parent copy' before moving on to possible children.

// Since using array of structures, can simply check which element of the array was the parent.

// Exits move in different car parks

// Could maybe use malloc to store the extra car parks, and then use the pointer generated by this to access these extras.
