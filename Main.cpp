#undef UNICODE
#include <windows.h>
#include "resource.h"
#include "Game.cpp"
#include <string>
#include <vector>

//Removes secure warnings
#pragma warning(disable:4996)

//Functions
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
void Open_And_Wait_Names(HWND hDlg, BOOL dlg_Main);
void Set_Names(HWND hDlg);
void Store_Names(HWND hDlg, std::string &s_temp1, std::string &s_temp2);
LPCSTR Player_turn(HWND hDlg, int& i_temp_turn);
void Set_Font_Buttons(HWND hDlg);
void Show_Winner(HWND hDlg, int i_Player_Winner);
void Reset_Game(HWND hDlg);
void Update_Score(HWND hDlg);

//Global variables
Tic_Tac_Toe Game;
BOOL dlg_Main = FALSE;
int i_Player_turn, i_Player1_Score, i_Player2_Score;
std::string s_Player1, s_Player2; //Variables that store the name of the players
//Vector of type int that stores the handlers of the buttons that are used to paint "X" and "0"
std::vector<int> v_buttons = { IDC_BUTTON1 ,IDC_BUTTON2 , IDC_BUTTON3 , IDC_BUTTON4 ,IDC_BUTTON5 ,
                               IDC_BUTTON6 , IDC_BUTTON7 , IDC_BUTTON8 , IDC_BUTTON9 };

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow)
{
    static char szAppName[] = "Tic Tac Toe";
    HWND        hwnd;
    MSG         msg;
    WNDCLASSEX  wndclass;

    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wndclass);

    hwnd = CreateWindow(szAppName,
        "Tic Tac Toe Dialog",      // Title of dialog box
        WS_OVERLAPPEDWINDOW,       // Style of window
        CW_USEDEFAULT,             // Initial horizontal position
        CW_USEDEFAULT,             // Initial vertical position
        CW_USEDEFAULT,             // Initial width
        CW_USEDEFAULT,             // Initial height
        NULL,
        NULL,
        hInstance,
        NULL);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

//This function processes messages for the name window
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static HINSTANCE hInstance;

    switch (iMsg)
    {
    //Operations executed when the window is created
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        if (!dlg_Main) {
            DialogBox(hInstance, MAKEINTRESOURCE(IDC_APP),
                hwnd, DlgProc);
            PostMessage(hwnd, WM_CLOSE, 0, 0);  //Inserts a new message in the waiting queue
        }

        return 0;
    case WM_DESTROY:
        PostQuitMessage(0); //Inserts an ending message
        return 0;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    HWND hDialog;

    switch (iMsg) {

    //Initial settings
    case WM_INITDIALOG:
    {
        hDialog = hDlg;
        
        //Sets the Player1,the one with "X" to start first
        i_Player_turn = 1;
        i_Player1_Score = i_Player2_Score = 0;

        //Opens the name window and waits for it to be closed before continuing
        Open_And_Wait_Names(hDlg, dlg_Main);
        //Sets the names of the players
        Set_Names(hDlg);
        //Stores the names of the players in two strings
        Store_Names(hDlg, s_Player1, s_Player2);
        //Sets the font of "X" and "0" on the buttons
        Set_Font_Buttons(hDlg);

        return TRUE;
    }
    //Operations that are executed when the "X" button of the window is clicked
    case WM_CLOSE:
        dlg_Main = FALSE;
        //Function that closes the window
        EndDialog(hDlg, 0);
        return TRUE;

    //Operations that are executed when a button on the window is clicked
    case WM_COMMAND:
        switch (LOWORD(wParam)) {

        //Operations that are executed when the "New game" button is pressed
        case ID_NEW_GAME:
        {
            //Resets the score and the names of the players
            i_Player1_Score = i_Player2_Score = 0;
            s_Player1 = s_Player2 = "";

            //Resets the game
            Reset_Game(hDlg);
            //Opens the name window and waits for it to be closed before continuing
            Open_And_Wait_Names(hDlg, dlg_Main);
            //Sets the names of the players
            Set_Names(hDlg);
            //Stores the names of the players in two strings
            Store_Names(hDlg, s_Player1, s_Player2);

            return TRUE;
        }

        /*Operations that are executed when a button with no caption is pressed
        * 
        * Adds the element in the matrix
        * Game.Add_Element(1, i_Player_turn);
        * 
        * Sets the caption of the button to be "X" or "0" depending on which turn it is,
        * if i_Player_turn = 1,that means it's the turn of Player1,
        * else if i_Player_turn = -1,it's the turn of Player2.
        * SetDlgItemText(hDlg, IDC_BUTTON1, Player_turn(hDlg,i_Player_turn));
        * 
        * Function that disables the button,so it can't be pressed multiple times
        * EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), FALSE);
        * 
        * Function that shows the winner,if there is one
        * Show_Winner(hDlg,Game.Winner());
        */
        case IDC_BUTTON1:
            Game.Add_Element(1, i_Player_turn);

            SetDlgItemText(hDlg, IDC_BUTTON1, Player_turn(hDlg,i_Player_turn));
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), FALSE);

            Show_Winner(hDlg,Game.Winner());

            break;
        case IDC_BUTTON2:
            Game.Add_Element(2, i_Player_turn);

            SetDlgItemText(hDlg, IDC_BUTTON2, Player_turn(hDlg, i_Player_turn));
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON2), FALSE);

            Show_Winner(hDlg,Game.Winner());

            break;
        case IDC_BUTTON3:
            Game.Add_Element(3, i_Player_turn);

            SetDlgItemText(hDlg, IDC_BUTTON3, Player_turn(hDlg, i_Player_turn));
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON3), FALSE);

            Show_Winner(hDlg,Game.Winner());

            break;
        case IDC_BUTTON4:
            Game.Add_Element(4, i_Player_turn);

            SetDlgItemText(hDlg, IDC_BUTTON4, Player_turn(hDlg, i_Player_turn));
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON4), FALSE);

            Show_Winner(hDlg,Game.Winner());

            break;
        case IDC_BUTTON5:
            Game.Add_Element(5, i_Player_turn);

            SetDlgItemText(hDlg, IDC_BUTTON5, Player_turn(hDlg, i_Player_turn));
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON5), FALSE);

            Show_Winner(hDlg,Game.Winner());

            break;
        case IDC_BUTTON6:
            Game.Add_Element(6, i_Player_turn);

            SetDlgItemText(hDlg, IDC_BUTTON6, Player_turn(hDlg, i_Player_turn));
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON6), FALSE);

            Show_Winner(hDlg,Game.Winner());

            break;
        case IDC_BUTTON7:
            Game.Add_Element(7, i_Player_turn);

            SetDlgItemText(hDlg, IDC_BUTTON7, Player_turn(hDlg, i_Player_turn));
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON7), FALSE);

            Show_Winner(hDlg,Game.Winner());

            break;
        case IDC_BUTTON8:
            Game.Add_Element(8, i_Player_turn);

            SetDlgItemText(hDlg, IDC_BUTTON8, Player_turn(hDlg, i_Player_turn));
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON8), FALSE);

            Show_Winner(hDlg,Game.Winner());

            break;
        case IDC_BUTTON9:
            Game.Add_Element(9, i_Player_turn);

            SetDlgItemText(hDlg, IDC_BUTTON9, Player_turn(hDlg, i_Player_turn));
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON9), FALSE);

            Show_Winner(hDlg,Game.Winner());

            break;
        
        //Operations that are executed when the button exit is clicked
        case ID_EXIT:
        {
            dlg_Main = FALSE;
            EndDialog(hDlg, 0);
            return TRUE;
        }
        }
        break;
    }
    return FALSE;
}

//Opens the name window and waits for it to be closed before continuing
void Open_And_Wait_Names(HWND hDlg, BOOL dlg_Main)
{
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    DWORD dwWaitForChild;

    //Creates a process of the name window so players can add their names
    BOOL b = CreateProcess("Debug\\Player_Name.exe",
        NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    /*If the creation of the process succeeds the main window will be blocked until the name window will be destroyed,
    * if the creation of the process failed,an error will pop-up saying "Try again!" */
    if (b)
    {
        dwWaitForChild = WaitForInputIdle(pi.hProcess, INFINITE);
        switch (dwWaitForChild)
        {
        case 0:
            break;
        case 0xFFFFFFFF:
            MessageBox(hDlg, "Try again!", "ERROR", MB_ICONERROR);
            break;
        }
        switch (WaitForSingleObject(pi.hProcess, INFINITE))
        {
        case WAIT_OBJECT_0:
            dlg_Main = FALSE;
            break;
        case WAIT_FAILED:
            MessageBox(hDlg, "Try again!", "ERROR", MB_ICONERROR);
            break;
        }
    }
    else
    {
        MessageBox(hDlg, "Try again!", "ERROR", MB_ICONERROR);
    }

    //Closes the handles of the process
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

//Sets the names of the players
void Set_Names(HWND hDlg)
{
    char* pMyCopy, * pClipMemory;
    BOOL bAvailable;
    HANDLE hClipMemory;

    bAvailable = IsClipboardFormatAvailable(CF_TEXT);
    if (bAvailable == FALSE)
    {
        return;
    }

    //Opens clipboard
    OpenClipboard(hDlg);
    //Obtains a handle to clipboard's memory
    hClipMemory = GetClipboardData(CF_TEXT);
    //Dynamic memory allocation 
    pMyCopy = (char*)malloc(GlobalSize(hClipMemory));
    //Obtains a pointer to clipboard's memory
    pClipMemory = (char*)GlobalLock(hClipMemory);

    //If pClipMemory is not NULL,it will copy the names from pClipMemory in pMyCopy
    if (pClipMemory)
    {
        strcpy(pMyCopy, pClipMemory);
    }

    GlobalUnlock(hClipMemory);
    //Closes clipboard
    CloseClipboard();

    //If pMyCopy is not NULL,it will set the names of the players and their score on the game window
    if (pMyCopy)
    {
        SetDlgItemTextA(hDlg, IDC_SCORE, pMyCopy);
    }
}

//Stores the names of the players in two strings
void Store_Names(HWND hDlg, std::string &s_temp1, std::string &s_temp2)
{
    /*The following code gets the line with the players names and score
     *and saves the names in two strings */
    char c_temp[100];
    GetDlgItemText(hDlg, IDC_SCORE, c_temp, 100);

    int i = 0;
    while (c_temp[i+2] != '0')
    {
        s_temp1 += c_temp[i];
        i++;
    }

    i += 9;
    while (c_temp[i] != ' ')
    {
        s_temp2 += c_temp[i];
        i++;
    }
}

//Decides who's turn it is and returns the value that will be on the button "X" or "0"
LPCSTR Player_turn(HWND hDlg,int &i_temp_turn)
{
    //If it is player1's turn
    if (i_temp_turn == 1)
    {
        //Makes the next to be player2's
        i_temp_turn = -1;

        //Disables the X button and enables the 0 button
        EnableWindow(GetDlgItem(hDlg, IDC_X), FALSE);
        EnableWindow(GetDlgItem(hDlg, IDC_0), TRUE);

        //Returns an LPCSTR that will be the caption of the button
        return "X";
    }
    //If it is player2's turn
    else
    {
        //Makes the next to be player1's
        i_temp_turn = 1;

        //Disables the 0 button and enables the X button
        EnableWindow(GetDlgItem(hDlg, IDC_0), FALSE);
        EnableWindow(GetDlgItem(hDlg, IDC_X), TRUE);

        //Returns an LPCSTR that will be the caption of the button
        return "0";
    }
}

//Sets the font of the buttons
void Set_Font_Buttons(HWND hDlg)
{
    HFONT hFont;
    HWND hEditCtrl;

    //Creates a nicer and bigger font
    hFont = CreateFontA(80, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        VARIABLE_PITCH, TEXT("Times New Roman"));

    //Sets the same font on all 9 buttons that are used to paint "X" and "0"
    for (int i = 0;i < v_buttons.size();i++)
    {
        hEditCtrl = GetDlgItem(hDlg, v_buttons[i]);
        SendMessage(hEditCtrl, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    }
}

//Shows the winner of the game,if there is one
void Show_Winner(HWND hDlg,int i_Player_Winner)
{
    /*If the winner is X,it will add 1 to Player1's score,then
     *a message box will appear with the caption: 
     *"The winner is [PLAYER1_NAME],starting a new game...",
     * after that the game will reset,keeping the score and the name of the players */
    if (i_Player_Winner == 3)
    {
        std::string Winner = "The winner is " + s_Player1 + ",starting a new game...";
        i_Player1_Score++;
        MessageBox(hDlg, Winner.c_str(), "Winner", MB_OK | MB_ICONQUESTION);
        Reset_Game(hDlg);
    }
    /*If the winner is 0,it will add 1 to Player2's score,then
    *a message box will appear with the caption:
    *"The winner is [PLAYER2_NAME],starting a new game...",
    * after that the game will reset,keeping the score and the name of the players */
    else if(i_Player_Winner == -3)
    {
        std::string Winner = "The winner is " + s_Player2 + ",starting a new game...";
        i_Player2_Score++;
        MessageBox(hDlg, Winner.c_str(), "Winner", MB_OK | MB_ICONQUESTION);
        Reset_Game(hDlg);
    }
    /*If the there's no winner,i_Player_Winner = -1,
     *a message box saying "Draw,starting a new game..." will appear
     *and the game will reset,keeping the score and the name of the players  */
    else if (i_Player_Winner == -1)
    {
        MessageBox(hDlg, "Draw,starting a new game...", "Draw", MB_OK | MB_ICONQUESTION);
        Reset_Game(hDlg);
    }
}

//Resets the game
void Reset_Game(HWND hDlg)
{
    //Clears the caption of the buttons used to paint "X" or "0" and enables them
    for (int i = 0;i < v_buttons.size();i++)
    {
        SetDlgItemText(hDlg, v_buttons[i], "");
        EnableWindow(GetDlgItem(hDlg, v_buttons[i]), TRUE);
    }
    //Resets the rounds and initialises the matrix with 0
    Game.Reset();

    //Updates the score 
    Update_Score(hDlg);
}

//Updates the score 
void Update_Score(HWND hDlg)
{
    /*The string s_score will store the name of player1,the score of player1,
     *the score of player2 and the name of player 2 */
    std::string s_score = s_Player1 + " " + std::to_string(i_Player1_Score) + " - " + std::to_string(i_Player2_Score) + " " + s_Player2;
    //Updates the editbox IDC_SCORE to the new score
    SetDlgItemText(hDlg, IDC_SCORE, s_score.c_str());
}