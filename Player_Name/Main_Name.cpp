#undef UNICODE
#include <windows.h>
#include "resource.h"

//Removes secure warnings
#pragma warning(disable:4996)
//Do not change,limit of player name should be 31,otherwise it will exceed the length of the editbox
#define MAXLEN_PLAYERNAME 31

//Functions
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
void Get_Names(HWND hDlg, char* szBuffer_Players);
void Send_Names(HWND hDlg, char* szBuffer_Players);

//Global variables
static UINT WM_GETFROMCLIP;
LPCTSTR GetFromClip = "GetFromClipboard";
HGLOBAL hGlobalMemory;
HWND hDialog, hwndReceive;
BOOL dlgActive = FALSE;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow)
{
    static char szAppName[] = "Name";
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
        "NameDialog",              // Title of dialog box
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
        WM_GETFROMCLIP = RegisterWindowMessage(GetFromClip);
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;

        if (!dlgActive)
        {
            DialogBox(hInstance, MAKEINTRESOURCE(IDD_NAME),
                hwnd, DlgProc);
            PostMessage(hwnd, WM_CLOSE, 0, 0);          //Inserts a new message in the waiting queue
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);         //Inserts an ending message
        return 0;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //Variable to store the names of the players
    char szBuffer_Players[MAXLEN_PLAYERNAME];

    switch (iMsg)
    {
    //Initial settings
    case WM_INITDIALOG:
        hDialog = hDlg;
        WM_GETFROMCLIP = RegisterWindowMessage(GetFromClip);

        return TRUE;
    //Operations that are executed when the "X" button of the window is clicked
    case WM_CLOSE:
        dlgActive = FALSE;
        //Function that closes the window
        EndDialog(hDlg, 0);
        return TRUE;
    //Operations that are executed at the destruction of the window
    case WM_DESTROY:
        /*If the main dialog box of the game is open,the following code
        * will get and send the names,otherwise a pop-up error will be shown */
        hwndReceive = FindWindow("Tic Tac Toe", "Tic Tac Toe Dialog");
        if (hwndReceive)
        {
            //Function that gets the name of the players
            Get_Names(hDlg, szBuffer_Players);
            //Function that sends the name of the players to clipboard
            Send_Names(hDlg, szBuffer_Players);
        }
        else
        {
            //Pop-up error
            MessageBox(hDlg, "Couldn't find the game", "ERROR", MB_OK | MB_ICONERROR);
        }

        //Inserts an ending message
        PostQuitMessage(0);
        return 0;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
            case ID_OK:
            {
                //Function that gets the name of the players
                Get_Names(hDlg, szBuffer_Players);
                //Function that sends the name of the players to clipboard
                Send_Names(hDlg, szBuffer_Players);

                //Function that closes the window
                EndDialog(hDlg, 0);

                return TRUE;
            }
        }
    }
    return FALSE;
}

//Function that gets the name of the players
void Get_Names(HWND hDlg,char *szBuffer_Players)
{
    char szBuffer_Player1[MAXLEN_PLAYERNAME], szBuffer_Player2[MAXLEN_PLAYERNAME];

    //szBuffer_Player1 gets the value typed in by the user in the editbox named IDC_PLAYER1
    GetDlgItemText(hDlg, IDC_PLAYER1, szBuffer_Player1, MAXLEN_PLAYERNAME);
    //szBuffer_Player2 gets the value typed in by the user in the editbox named IDC_PLAYER2
    GetDlgItemText(hDlg, IDC_PLAYER2, szBuffer_Player2, MAXLEN_PLAYERNAME);

    /*The following code is used to store the names of the players in szBuffer_Players
    * if the players didn't enter any name or only one of them did the default name for
    * the players is Player1 or Player2 depending on the case,the code also sets the score
    * to 0-0 */
    if (strlen(szBuffer_Player1))
    {
        strcpy(szBuffer_Players, szBuffer_Player1);
    }
    else
    {
        strcpy(szBuffer_Players, "Player1");
    }
    strcat(szBuffer_Players, "  0 - 0  ");
    if (strlen(szBuffer_Player2))
    {
        strcat(szBuffer_Players, szBuffer_Player2);
    }
    else
    {
        strcat(szBuffer_Players, "Player2");
    }
}

//Function that sends the name of the players to clipboard
void Send_Names(HWND hDlg, char *szBuffer_Players)
{
    char* pGlobalMemory = new char();

    /*The following code tries to find the main dialog box of the game
    * if it succeeds,the name of the players will be saved on clipboard */
    hwndReceive = FindWindow("Tic Tac Toe", "Tic Tac Toe Dialog");
    if (hwndReceive)
    {
        //Alocates memory on the heap
        hGlobalMemory = GlobalAlloc(GHND, strlen(szBuffer_Players) + 1);

        if (pGlobalMemory && hGlobalMemory)
        {
            pGlobalMemory = (char*)GlobalLock(hGlobalMemory);

            //Copies player names in heap memory
            for (UINT i = 0;i < strlen(szBuffer_Players);i++)
            {
                *(pGlobalMemory++) = szBuffer_Players[i];
            }
            GlobalUnlock(hGlobalMemory);
        }

        //Opens cliboard
        OpenClipboard(hDlg);
        //Transfers the names of the players
        SetClipboardData(CF_TEXT, hGlobalMemory);
        //Closes clipboard
        CloseClipboard();
    }
}