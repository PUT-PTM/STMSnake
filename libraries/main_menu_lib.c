#include "main_menu_lib.h"
#include "snake_engine.h"
#include "snake_game.h"
#include "TimerConfig.h"
#include "ff.h"

#define HeightOfSelectionFrame 8
#define NumberOfPosition 4 		//main menu
#define NumberOfPositionSpeedMenu 5	//speed menu

MainMenuStatus StatusMenu;

void MainMenu()
    {
    InitStateButton(Button_None);
    SetMenuPositions(1);
    unsigned char TmpMenuPositons=GetMenuPositions();

    DrawGameMainMenu();
    DrawMenuSelectionFrame7x5(GetMenuPositions(),PCD8544_Pixel_Set);
    PCD8544_Refresh();

    while(1)
	{
	if(GetModifyFlag()==Modify)
	{
		switch(GetButtonState())
		{
		    case Button_Accept:
			{
			SetModifyFlag(NotModify);
			SetButtonState(Button_None);
			SelectAction(GetMenuPositions());

			SetModifyFlag(NotModify);
			TmpMenuPositons = 1;
			break;
			}

		    case Button_Up:
			{
			if(GetMenuPositions()==1)
			    {
			    TmpMenuPositons = NumberOfPosition;
			    }
			else
			    {
			    TmpMenuPositons = GetMenuPositions();
			    TmpMenuPositons--;
			    }
			break;
			}

		    case Button_Down:
			{
			if(GetMenuPositions()==NumberOfPosition)
			    {
			    TmpMenuPositons = 1;
			    }
			else
			    {
			    TmpMenuPositons = GetMenuPositions();
			    TmpMenuPositons++;
			    }
			break;
			}

		    default:
			{
			break;
			}
		}//switch
	SetMenuPositions(TmpMenuPositons);
	DrawGameMainMenu();
	DrawMenuSelectionFrame7x5(GetMenuPositions(),PCD8544_Pixel_Set);
	PCD8544_Refresh();
	SetModifyFlag(NotModify);
	    }//if

	}//while
    }//MainMenu

//NumberOfAction == Number of current position
void SelectAction(unsigned char NumberOfAction)
    {
    switch(NumberOfAction)
	{
	    case 1: //main game loop
		{
		StartSnakeGame();
		break;
		}
	    case 2: //difficulty menu
		{
		SpeedMenu();
		break;
		}
	    case 3: //high scores
		{
		HighScoresMenu();
		break;
		}
	    case 4: //reset scores
		{
			FATFS fatfs;
			f_mount( 0, &fatfs );
			f_unlink ("FILE1"); //physically removing the file from the SD card
			f_unlink ("FILE2");
			f_unlink ("FILE3");
			f_unlink ("FILE4");
			f_unlink ("FILE5");
		break;
		}
	}
    }

void HighScoresMenu()
    {
    InitStateButton(Button_None);
    SetMenuPositions(1);
    unsigned char TmpMenuPositons=GetMenuPositions();

    DrawSpeedMenu();
    DrawMenuSelectionFrame7x5(GetMenuPositions(),PCD8544_Pixel_Set);
    PCD8544_Refresh();

    while(1)
	{
	if(GetModifyFlag()==Modify)
	{
		switch(GetButtonState())
		{
		    case Button_Accept:
			{
			SetModifyFlag(NotModify);

			ChooseFileScore(GetMenuPositions());

			break;
			}

		    case Button_Up:
			{
			if(GetMenuPositions()==1)
			    {
			    TmpMenuPositons = NumberOfPositionSpeedMenu;
			    }
			else
			    {
			    TmpMenuPositons = GetMenuPositions();
			    TmpMenuPositons--;
			    }
			break;
			}

		    case Button_Down:
			{
			if(GetMenuPositions()==NumberOfPositionSpeedMenu)
			    {
			    TmpMenuPositons = 1;
			    }
			else
			    {
			    TmpMenuPositons = GetMenuPositions();
			    TmpMenuPositons++;
			    }
			break;
			}

		    default:
			{
			break;
			}
		}//switch

	if(GetButtonState()==Button_Accept) //exit from menu after set speed
	    {
	    SetModifyFlag(NotModify);
	    break; //out from while loop
	    }//if2

	SetMenuPositions(TmpMenuPositons);
	DrawSpeedMenu();
	DrawMenuSelectionFrame7x5(GetMenuPositions(),PCD8544_Pixel_Set);
	PCD8544_Refresh();
	SetModifyFlag(NotModify);
	    }//if1

	}//while

    }

void ChooseFileScore(unsigned char Position)
    {
    char * file;
    switch(Position)
	{
	case 1:
	    {
	    file = "FILE1";
	    DisplayScores(file);
	    break;
	    }
	case 2:
	    {
	    file = "FILE2";
	    DisplayScores(file);
	    break;
	    }
	case 3:
	    {
	    file = "FILE3";
	    DisplayScores(file);
	    break;
	    }
	case 4:
	    {
	    file = "FILE4";
	    DisplayScores(file);
	    break;
	    }
	case 5:
	    {
	    file = "FILE5";
	    DisplayScores(file);
	    break;
	    }

	}//switch

    volatile char flag=1;
    SetModifyFlag(NotModify);
    while(flag)
	{
	if(GetModifyFlag()==Modify && GetButtonState()==Button_Accept)
		{
		flag = 0;
		}//if
	}//while

    }//end function

void DisplayScores(char * file)
    {
    FATFS fatfs;
    FIL Fil;
    FRESULT fresult;
    UINT ReadData,ReadData1,ReadData2,ReadData3;

    char n1[4],n2[4],n3[4],w1=0,w2=0,w3=0; //nX-player, wX-player score
    n1[3]=0;
    n2[3]=0;
    n3[3]=0;

    fresult = f_mount( 0, &fatfs );
    fresult = f_open( &Fil, (const char * )file, FA_OPEN_EXISTING |  FA_READ);
    if( fresult == FR_OK )
    {
            f_lseek(&Fil,0); 				 //move the cursor to the start of a file
            fresult = f_read( &Fil, n1, 3, &ReadData1); //read data
            fresult = f_read( &Fil, &w1, 1, &ReadData);
            fresult = f_read( &Fil, n2, 3, &ReadData2);
            fresult = f_read( &Fil, &w2, 1, &ReadData);
            fresult = f_read( &Fil, n3, 3, &ReadData3);
            fresult = f_read( &Fil, &w3, 1, &ReadData);

	    PCD8544_Clear();

	    unsigned char j, d, s;	// j-unity d-dozens s-hundreds
	    if(ReadData1!=0)
		{
		PCD8544_GotoXY(1,1+0*HeightOfSelectionFrame);
		PCD8544_Puts(n1,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		PCD8544_Puts("  ",PCD8544_Pixel_Set,PCD8544_FontSize_5x7);

		if(w1<=9)
		    {
		    PCD8544_Putc(w1+48,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    }
		else
		if(w1 >=10 && w1 <=99)
		    {
		    d=((char)(w1/10))+48;
		    j=(w1 % 10)+48;
		    PCD8544_Putc(d,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    PCD8544_Putc(j,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    }
		else
		if(w1>=100)
		    {
		    s=((char)(w1/100))+48;
		    d=((char)(w1/10))+48;
		    j=(w1 % 10)+48;

		    PCD8544_Putc(s,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    PCD8544_Putc(d,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    PCD8544_Putc(j,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    }
		}

	    if(ReadData2!=0)
	   	{
	    PCD8544_GotoXY(1,1+1*HeightOfSelectionFrame);
	    PCD8544_Puts(n2,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
	    PCD8544_Puts("  ",PCD8544_Pixel_Set,PCD8544_FontSize_5x7);

		if(w2<=9)
		    {
		    PCD8544_Putc(w2+48,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    }
		else
		if(w2 >=10 && w2 <=99)
		    {
		    d=((char)(w2/10))+48;
		    j=(w2 % 10)+48;
		    PCD8544_Putc(d,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    PCD8544_Putc(j,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    }
		else
		if(w2>=100)
		    {
		    s=((char)(w2/100))+48;
		    d=((char)(w2/10))+48;
		    j=(w2 % 10)+48;

		    PCD8544_Putc(s,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    PCD8544_Putc(d,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    PCD8544_Putc(j,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    }
	   	}

	    if(ReadData3!=0)
	   	{
	    PCD8544_GotoXY(1,1+2*HeightOfSelectionFrame);
	    PCD8544_Puts(n3,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
	    PCD8544_Puts("  ",PCD8544_Pixel_Set,PCD8544_FontSize_5x7);

		if(w3<=9)
		    {
		    PCD8544_Putc(w3+48,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    }
		else
		if(w3 >=10 && w3 <=99)
		    {
		    d=((char)(w3/10))+48;
		    j=(w3 % 10)+48;
		    PCD8544_Putc(d,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    PCD8544_Putc(j,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    }
		else
		if(w3>=100)
		    {
		    s=((char)(w3/100))+48;
		    d=((char)(w3/10))+48;
		    j=(w3 % 10)+48;

		    PCD8544_Putc(s,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    PCD8544_Putc(d,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    PCD8544_Putc(j,PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
		    }
	   	}

	    PCD8544_Refresh();
    }else
	{
	PCD8544_Clear();
	PCD8544_GotoXY(1,1+0*HeightOfSelectionFrame);
	PCD8544_Puts("Not existing",PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
	PCD8544_Refresh();

    }
    fresult = f_close( &Fil );
}

void SpeedMenu()
    {
    InitStateButton(Button_None);
    SetMenuPositions(1);
    unsigned char TmpMenuPositons=GetMenuPositions();

    DrawSpeedMenu();
    DrawMenuSelectionFrame7x5(GetMenuPositions(),PCD8544_Pixel_Set);
    PCD8544_Refresh();

    while(1)
	{
	if(GetModifyFlag()==Modify)
	{
		switch(GetButtonState())
		{
		    case Button_Accept:
			{
			SetModifyFlag(NotModify);

			SetSpeedGame(GetMenuPositions());

			break;
			}

		    case Button_Up:
			{
			if(GetMenuPositions()==1)
			    {
			    TmpMenuPositons = NumberOfPositionSpeedMenu;
			    }
			else
			    {
			    TmpMenuPositons = GetMenuPositions();
			    TmpMenuPositons--;
			    }
			break;
			}

		    case Button_Down:
			{
			if(GetMenuPositions()==NumberOfPositionSpeedMenu)
			    {
			    TmpMenuPositons = 1;
			    }
			else
			    {
			    TmpMenuPositons = GetMenuPositions();
			    TmpMenuPositons++;
			    }
			break;
			}

		    default:
			{
			break;
			}
		}//switch

	if(GetButtonState()==Button_Accept) //exit from menu after set speed
	    {
	    SetModifyFlag(NotModify);
	    break; //out from while loop
	    }//if2

	SetMenuPositions(TmpMenuPositons);
	DrawSpeedMenu();
	DrawMenuSelectionFrame7x5(GetMenuPositions(),PCD8544_Pixel_Set);
	PCD8544_Refresh();
	SetModifyFlag(NotModify);
	    }//if1

	}//while
    }//SpeedMenu

void DrawSpeedMenu()
    {
    PCD8544_Clear();
    PCD8544_GotoXY(1,1);
    PCD8544_Puts("VERY HARD",PCD8544_Pixel_Set,PCD8544_FontSize_5x7);

    PCD8544_GotoXY(1,1+HeightOfSelectionFrame);
    PCD8544_Puts("HARD",PCD8544_Pixel_Set,PCD8544_FontSize_5x7);

    PCD8544_GotoXY(1,1+2*HeightOfSelectionFrame);
    PCD8544_Puts("NORMAL",PCD8544_Pixel_Set,PCD8544_FontSize_5x7);

    PCD8544_GotoXY(1,1+3*HeightOfSelectionFrame);
    PCD8544_Puts("EASY",PCD8544_Pixel_Set,PCD8544_FontSize_5x7);

    PCD8544_GotoXY(1,1+4*HeightOfSelectionFrame);
    PCD8544_Puts("VERY EASY",PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
    }

void SetSpeedGame(unsigned char Position)
    {
    switch(Position)
	{
	case 1:
	    {
	    ConfTim3(100);
	    SetSnakeSpeed(1);
	    break;
	    }
	case 2:
	    {
	    ConfTim3(200);
	    SetSnakeSpeed(2);
	    break;
	    }
	case 3:
	    {
	    ConfTim3(350);
	    SetSnakeSpeed(3);
	    break;
	    }
	case 4:
	    {
	    ConfTim3(550);
	    SetSnakeSpeed(4);
	    break;
	    }
	case 5:
	    {
	    ConfTim3(750);
	    SetSnakeSpeed(5);
	    break;
	    }
	default:
	    {
	    ConfTim3(750);
	    SetSnakeSpeed(5);
	    break;
	    }
	}
    }

void DrawGameMainMenu()
    {
    PCD8544_Clear();
    PCD8544_GotoXY(1,1);
    PCD8544_Puts("Play",PCD8544_Pixel_Set,PCD8544_FontSize_5x7);

    PCD8544_GotoXY(1,1+HeightOfSelectionFrame);
    PCD8544_Puts("Difficulty",PCD8544_Pixel_Set,PCD8544_FontSize_5x7);

    PCD8544_GotoXY(1,1+2*HeightOfSelectionFrame);
    PCD8544_Puts("High scores",PCD8544_Pixel_Set,PCD8544_FontSize_5x7);

    PCD8544_GotoXY(1,1+3*HeightOfSelectionFrame);
    PCD8544_Puts("Reset scores",PCD8544_Pixel_Set,PCD8544_FontSize_5x7);
    }

void DrawMenuSelectionFrame7x5(unsigned char NumOfPosition, PCD8544_Pixel_t color)
    {
    unsigned char y0=(NumOfPosition-1)*HeightOfSelectionFrame;
    unsigned char y1=(NumOfPosition-1)*HeightOfSelectionFrame + HeightOfSelectionFrame;
    PCD8544_DrawRectangle(0,y0,83,y1,color);
    }

void SetMenuPositions(unsigned char Number)
    {
    StatusMenu.PositionNumber =Number;
    }

unsigned char GetMenuPositions()
    {
    return StatusMenu.PositionNumber;
    }

void WaitForAcceptButton()
    {
    while(1)
	{
	if(GetModifyFlag()==Modify)
	    {
	    if(GetButtonState()==Button_Accept)
		{
		SetModifyFlag(NotModify);
		break;
		}
	    else
		{
		SetModifyFlag(NotModify);
		}
	    SetModifyFlag(NotModify);
	    }//if 1

	}//while
    }
