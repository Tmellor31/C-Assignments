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

typedef enum
{
  Horizontal,
  Vertical
} Orientation;

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
CarParkTree make_car_park_tree(void);
CarList make_car_list(void);

int main(int argc, char *argv[])
{
  test();
  int count;
  FILE *fpin = fopen(argv[1], "r");

  if (fpin == NULL)
  {
    printf("Cannot open file\n");
    exit(EXIT_FAILURE);
  }

  if (argc != 2)
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
    count++;
    /*if (count == 1)
    {
      carpark.width = c;
    }
    if (count == 3)
    {
      carpark.height = c;
    }*/
    c = fgetc(fpin);
  }

  printf("test");
  fclose(fpin);
  return EXIT_SUCCESS;
}

// Things to do:Change size to be height and width read from file, change file reading to be its own function,
// and make a start on below

// CarPark *park = carparktree.car_parks[0];
//- Add parent to CarPark
//- Create create_child_carparks function that takes a CarPark and CarList and creates a new CarPark for each possible move of each Car. These CarParks need to be added to CarParkTree with reference to their parent
//- Create generate_carpark_tree which keeps creating CarParks until a CarPark exists which doesn't contiain Cars
//- Carparks must be unique

CarParkTree make_car_park_tree(void)
{
  CarParkTree park_tree;
  park_tree.total = 0;
  return park_tree;
}

bool create_car_park_children(CarParkTree *parktree, CarList *list, int position)
{
  CarPark *carpark = &(parktree->carparks[position]);
  for (int index = 0; index < list->total; index++)
  {
    Car *car = &(list->cars[index]);
    int horizontalstatus = move_car_horizontally(car, carpark);
    bool canmoveleft = horizontalstatus & 1;
    int verticalstatus = move_car_vertically(car, carpark);
    bool canmoveright = horizontalstatus & 2;
    if (canmoveleft)
    {
      createpark()
    }

    {
    }
  }
}

CarPark generate_carpark(CarPark *park, Car *car, int change)
{
  CarPark newcarpark;
  newcarpark.width = park->width;
  newcarpark.height = park->height;
  for (int row = 0; row < park->height; row++)
  {
    for (int col = 0; col < park->width; col++)
    {
      switch (change)
      {
      case 0:
      //move left 
      if (row == car->startrow)
        break;
      
      default:
        break;
      }
    }
  }
}

int move_car_vertically(Car *car, CarPark *park)
{
  int rowposition = car->startrow;
  int colposition = car->startcol;
  char letter = car->label;
  int moveflags = 0;
  if (car->orientation == Vertical)

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
  if (car->orientation == Horizontal)
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
    car->orientation = Vertical;
  }
  if (park->layout[row][col] == park->layout[row][col - 1])
  {
    car->orientation = Horizontal;
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
  CarParkTree carparktree = make_car_park_tree();
  CarList car_list = make_car_list();
  CarPark park2;
  strtopark(&(carparktree.carparks[0]), "#.####.BBB.##A...##A...##A...#######");
  strtopark(&park2, "#.####.BBB.##A...##A...##A...#######");
  CarPark *park = &(carparktree.carparks[0]);
  assert(printcell(park, 6, 6) == true);
  assert(vehiclecheck(park) == true);
  assert(findcars(&car_list, park) == true);
  assert(car_list.total == 2);
  assert(find_car_position(&car_list, 'B') == 0);
  assert(find_car_position(&car_list, 'A') == 1);
  assert(car_list.cars[0].orientation == Horizontal);
  assert(move_car_vertically(&(car_list.cars[0]), park) == 0);
  assert(move_car_horizontally(&(car_list.cars[0]), park) == 3);
  assert(move_car_horizontally(&(car_list.cars[1]), park) == 0);
  assert(move_car_vertically(&(car_list.cars[1]), park) == 0);
  // printpark(&park);
  iterate(park, &printcell);
  strtopark(park, "#.####.....##....##....##....#######");
  // assert(find_car_position(&cars, 'B') == -1);
  assert(vehiclecheck(park) == false);
  assert(vehiclecheck(&park2) == true);
}

// Use a while loop, compare the initial car park with the possible vehicle moves, and add any possible children to the end of the list.

// Can either store as a 2D array and it becomes easier to print, or store as an array of vehicles and its easier to solve.

// Ideas on how to add car park - can create a 'parent copy' before moving on to possible children.

// Since using array of structures, can simply check which element of the array was the parent.

// Exits move in different car parks

// Could maybe use malloc to store the extra car parks, and then use the pointer generated by this to access these extras.
