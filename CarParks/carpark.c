//Code sometimes doesn't work due to a segmentation fault, wasn't able to figure out why. 
//When this doesn't happen, it seems to successfully figure out the solution for the example given in test.

//The implementation is there for generating the carparks, establishing parents, solving the parks and then 
//printing the total number of moves, but there must be an issue somewhere I can't find :( 
//The read from file function should be working, although it isn't called. I think I focused too much on it in hindsight.


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_PARK_SIZE 20
#define BOLLARD '#'
#define SPACE '.'
#define MAX_CARS 26
#define MAX_CARPARKS 50

struct CarPark
{
  int parent;
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
bool parkhascars(CarPark *park);
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
bool create_car_park_children(CarParkTree *parktree, CarList *list, int position);
CarPark generate_carpark(CarPark *park, Car *car, int change, int parentid);
CarParkTree loadpark(char filename[], CarPark *park);
bool isParkUnique(CarParkTree *tree, CarPark *park);
bool arecarparks_equal(CarPark *park, CarPark *other);
int solve_carpark(CarParkTree *tree);

int main(int argc, char *argv[])
{
  test();
  if (argc < 2)
  {
    fprintf(stderr, "Usage : %s (-showifwanted) <file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  CarPark park;
  printf("%i\n", argc);
  loadpark(argv[argc - 1], &park);
  // if (argv [1] == '-show')
  //{
  //  print all steps
  //}
  // else
  //{
  // print just the steps taken to solve the park
  //}

  return 0;
}

CarParkTree loadpark(char filename[], CarPark *park)
{
  int count;
  FILE *fp = fopen(filename, "r");
  if (fp == NULL)
  {
    printf("Cannot open %s\n", filename);
    exit(EXIT_FAILURE);
  }
  if (!fp)
  {
    fprintf(stderr, "Cannot read from %s\n", filename);
    exit(EXIT_FAILURE);
  }

  char c;
  int line = 0;
  int i = 0;

  CarPark carpark;
  carpark.parent = -1;
  while ((c = fgetc(fp)) != EOF)
  {
    count++;
    if (count == 1)
    {
      carpark.width = c;
    }
    if (count == 3)
    {
      carpark.height = c;
    }
    if (c == '\n')
    {
      carpark.layout[line++][i] = '\0';
      i = 0;
    }
    else
    {
      carpark.layout[line][i++] = c;
    }
    printf("%c\n", c);
  }
  fclose(fp);
  CarParkTree tree = make_car_park_tree();
  tree.carparks[0] = carpark;
  tree.total++;
  return tree;
}

CarParkTree make_car_park_tree(void)
{
  CarParkTree park_tree;
  park_tree.total = 0;
  return park_tree;
}

int solve_carpark(CarParkTree *tree)
{
  for (int i = 0; i < tree->total; i++)
  {
    printf("total is %i\n", tree->total);
    CarPark *park = &(tree->carparks[i]);
    if (!parkhascars(park))
    {
      return i;
    }
    CarList carlist = make_car_list();
    findcars(&carlist, park);
    create_car_park_children(tree, &carlist, i);
  }
  return -1;
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
    bool canmoveup = verticalstatus & 1;
    bool canmovedown = verticalstatus & 2;
    if (canmoveleft)
    {
      CarPark child_carpark = generate_carpark(carpark, car, 0, position);
      if (isParkUnique(parktree, &child_carpark))
      {
        parktree->carparks[parktree->total] = child_carpark;
        parktree->total++;
      }
    }
    if (canmoveright)
    {
      CarPark child_carpark = generate_carpark(carpark, car, 1, position);
      if (isParkUnique(parktree, &child_carpark))
      {
        parktree->carparks[parktree->total] = child_carpark;
        parktree->total++;
      }
    }
    if (canmoveup)
    {
      CarPark child_carpark = generate_carpark(carpark, car, 2, position);
      if (isParkUnique(parktree, &child_carpark))
      {
        parktree->carparks[parktree->total] = child_carpark;
        parktree->total++;
      }
    }
    if (canmovedown)
    {
      CarPark child_carpark = generate_carpark(carpark, car, 3, position);
      if (isParkUnique(parktree, &child_carpark))
      {
        parktree->carparks[parktree->total] = child_carpark;
        parktree->total++;
      }
    }
  }
}

bool isParkUnique(CarParkTree *tree, CarPark *park)
{
  for (int i = 0; i < tree->total; i++)
  {
    CarPark *currentpark = &(tree->carparks[i]);
    if (arecarparks_equal(park, currentpark))
    {
      return false;
    }
  }
  return true;
}

bool arecarparks_equal(CarPark *park, CarPark *other)
{
  if ((park->height != other->height) && (park->width != other->width))
  {
    return false;
  }
  for (int row = 0; row < park->height; row++)
  {
    for (int col = 0; col < park->width; col++)
    {
      if (park->layout[row][col] != other->layout[row][col])
      {
        return false;
      }
    }
  }
  return true;
}

CarPark generate_carpark(CarPark *park, Car *car, int change, int parentid)
{
  CarPark newcarpark;
  newcarpark.width = park->width;
  newcarpark.height = park->height;
  newcarpark.parent = parentid;
  for (int row = 0; row < park->height; row++)
  {
    for (int col = 0; col < park->width; col++)
    {
      switch (change)
      {
      case 0:
        // move left
        if ((row == car->startrow) && (col == car->startcol - 1))
        {
          newcarpark.layout[row][col] = car->label;
        }
        else if ((row == car->startrow) && (col == car->startcol + car->length))
        {
          newcarpark.layout[row][col] = SPACE;
        }
        else
        {
          newcarpark.layout[row][col] = park->layout[row][col];
        }
        break;

      case 1:
        // move right
        if ((row == car->startrow) && (col == car->startcol + 1))
        {
          newcarpark.layout[row][col] = SPACE;
        }
        else if ((row == car->startrow) && (col == car->startcol + car->length + 1))
        {
          newcarpark.layout[row][col] = car->label;
        }
        else
        {
          newcarpark.layout[row][col] = park->layout[row][col];
        }
        break;
      case 2:
        // move up
        if ((row == car->startrow - 1) && (col == car->startcol))
        {
          newcarpark.layout[row][col] = car->label;
        }
        else if ((row == car->startrow + car->length) && (col == car->startcol))
        {
          newcarpark.layout[row][col] = SPACE;
        }
        else
        {
          newcarpark.layout[row][col] = park->layout[row][col];
        }
        break;
      case 3:
        // move down
        if ((row == car->startrow + 1) && (col == car->startcol))
        {
          newcarpark.layout[row][col] = SPACE;
        }
        else if ((row == car->startrow + car->length + 1) && (col == car->startcol))
        {
          newcarpark.layout[row][col] = car->label;
        }
        else
        {
          newcarpark.layout[row][col] = park->layout[row][col];
        }
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
  for (int row = 0; row < park->height; row++)
  {
    for (int col = 0; col < park->width; col++)
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
  for (int row = 0; row < park->height; row++)
  {
    for (int col = 0; col < park->width; col++)
    {
      printf("%c", park->layout[row][col]);
    }
    printf("\n");
  }
}

bool parkhascars(CarPark *park)
{ // function that checks for vehicles and if none are found the car park is not added.
  for (int row = 0; row < park->width; row++)
  {
    for (int col = 0; col < park->height; col++)
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
  for (int row = 0; row < park->width; row++)
  {
    for (int col = 0; col < park->height; col++)
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

  int row, col;
  for (row = 0; row < park->height; row++)
  {
    for (col = 0; col < park->width; col++)
    {
      park->layout[row][col] = str[col + (row * 6)];
    }
  }
  return true;
}

bool printcell(CarPark *park, int row, int col)
{
  printf("%c", park->layout[row][col]);
  return true;
}

void test(void)
{
  CarParkTree carparktree = make_car_park_tree();
  CarList car_list = make_car_list();
  CarPark park2;
  CarPark *park = &(carparktree.carparks[0]);
  carparktree.carparks[0].width = 6;
  carparktree.carparks[0].height = 6;
  park->parent = -1;
  carparktree.total++;
  strtopark(park, "#.####.BBB.##A...##A...##A...#######");
  assert(printcell(park, 6, 6) == true);
  printpark(park);
  assert(parkhascars(park) == true);
  assert(findcars(&car_list, park) == true);
  assert(find_car_position(&car_list, 'B') == 0);
  assert(find_car_position(&car_list, 'A') == 1);
  assert(car_list.cars[0].orientation == Horizontal);
  assert(move_car_vertically(&(car_list.cars[0]), park) == 0);
  assert(move_car_horizontally(&(car_list.cars[0]), park) == 3);
  assert(move_car_horizontally(&(car_list.cars[1]), park) == 0);
  assert(move_car_vertically(&(car_list.cars[1]), park) == 0);
  printf("number is %i\n", solve_carpark(&carparktree));
}