#ifndef _structcoordinate
#define _structcoordinate
#define MAX_LENGTH_SNAKE ((84/BOXDIM-2)*(48/BOXDIM-2)) //or 252 (21*12)


typedef struct
    {
     unsigned char x;
     unsigned char y;
 }Coordinate;


typedef struct
{
    Coordinate *SnakeSegments;//table with snake's segments [0]-tail [LenghtSnake-1]-head
    unsigned int PlayerPoints;
    unsigned char LengthSnake;
    unsigned char SnakeSpeed;
}StateGame;

typedef struct
{
    unsigned char ModifyFlag;
    unsigned char Button;
}StateButton;

typedef enum{
    NotModify=0,
    Modify = 1 //!NotModify
}ModifyFlagStatus;

typedef enum{
    Button_None=0,
    Button_Left=1,
    Button_Up=2,
    Button_Right=3,
    Button_Down=4,
    Button_Akcept=5,
}ButtonState;

typedef enum{
    Collisions=0,
    NoCollisions=1,
}CollisionsState;

typedef enum{
    StopGame=0,
    RunGame=1,
}GameStatus;


//StateGame struct
void InitStateGame(void);
void DeInitStateGame(void);
void AddSnakeSegmentAtTheEnd(Coordinate Segment);
void SetSnakeSegment(unsigned int Index,Coordinate Segment);
Coordinate GetSnakeSegment(unsigned int Index);
void SetLengthSnake(unsigned char Length);
unsigned char GetLengthSnake(void);
void SetPlayerPoints(unsigned int points);
unsigned char GetPlayerPoints(void);
void SetSnakeSpeed(unsigned char Speed);
unsigned char GetSnakeSpeed(void);
void SetSnakeHeadSegment(Coordinate Segment);
Coordinate GetSnakeHeadSegment(void);
void RemoveLastTailSegment(void);

CollisionsState CheckCollisions(Coordinate Segment);
CollisionsState CheckFruitCollisions(Coordinate SnakeSegment, Coordinate Fruit);
Coordinate CheckAndCorretReversedDirection(Coordinate NewHeadSegment);
Coordinate CheckAndCorretReversedDirection2(Coordinate NewHeadSegment);
Coordinate GenerateFruit(void);

//StateButton struct
void InitStateButton(void);
void InitStateButton2(ButtonState);
void SetButtonState(ButtonState But);
ButtonState GetButtonState(void);
void SetModifyFlag(ModifyFlagStatus Flag);
ModifyFlagStatus GetModifyFlag(void);

//other
char RunPause(void);
void TimerLoop(void);

void SaveResult(void);
void GetNickAndScores(unsigned char * TabPlayer);
void WriteResultToSd(unsigned char * Tab, char BytesToSave);
void CopyPlayerInTempTab(unsigned char *Tab, unsigned char SourcePlayer, unsigned char DestinationPlayer);

#endif