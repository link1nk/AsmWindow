;************************************************************************
; Window Library (Código Fonte)
;
; Autor: Lincoln Dias
;
; Criação:      11 Jan 2024
; Atualização:  13 Jan 2024
; Montador:   Nasm
;
; Descrição:  Essa biblioteca foi desenvolvida para facilitar a criação
;             de janelas e aplicações gráficas
;
;************************************************************************

extern GetModuleHandleA
extern RegisterClassExA
extern CreateWindowExA
extern DefWindowProcA
extern LoadImageA
extern DispatchMessageA
extern GetMessageA
extern IsDialogMessageA
extern PostQuitMessage
extern ShowWindow
extern TranslateMessage
extern UpdateWindow
extern GetClientRect


;-----------------------------------------+
;     Macros para WNDCLASSEXA - style     |
;-----------------------------------------+
%define CS_BYTEALIGNCLIENT 0x1000
%define CS_BYTEALIGNWINDOW 0x2000
%define CS_CLASSDC         0x0040
%define CS_DBLCLKS         0x0008
%define CS_DROPSHADOW      0x00020000
%define CS_GLOBALCLASS     0x4000
%define CS_HREDRAW         0x0002
%define CS_NOCLOSE         0x0200
%define CS_OWNDC           0x0020
%define CS_PARENTDC        0x0080
%define CS_SAVEBITS        0x0800
%define CS_VREDRAW         0x0001
;------------------------------------------


;-----------------------------------------+
;     Macros para Systen Icons - IDI      |
;-----------------------------------------+
%define IDI_APPLICATION   32512
%define IDI_ERROR         32513
%define IDI_QUESTION      32514
%define IDI_WARNING       32515
%define IDI_INFORMATION   32516
%define IDI_WINLOGO       32517
%define IDI_SHIELD        32518
;-----------------------------------------+


;-----------------------------------------+
;    Macros para System Cursors - IDC     |
;-----------------------------------------+
%define IDC_ARROW       32512
%define IDC_ARROW       32512
%define IDC_IBEAM       32513
%define IDC_WAIT        32514
%define IDC_CROSS       32515
%define IDC_UPARROW     32516
%define IDC_SIZE        32640
%define IDC_ICON        32641
%define IDC_SIZENWSE    32642
%define IDC_SIZENESW    32643
%define IDC_SIZEWE      32644
%define IDC_SIZENS      32645
%define IDC_SIZEALL     32646
%define IDC_NO          32648
%define IDC_HAND        32649
%define IDC_APPSTARTING 32650
%define IDC_HELP        32651
%define IDC_PIN         32671
%define IDC_PERSON      32672
;-----------------------------------------+


;-----------------------------------------+
;       Macros para Color Brushes         |
;-----------------------------------------+
%define COLOR_SCROLLBAR               0
%define COLOR_BACKGROUND              1
%define COLOR_ACTIVECAPTION           2
%define COLOR_INACTIVECAPTION         3
%define COLOR_MENU                    4
%define COLOR_WINDOW                  5
%define COLOR_WINDOWFRAME             6
%define COLOR_MENUTEXT                7
%define COLOR_WINDOWTEXT              8
%define COLOR_CAPTIONTEXT             9
%define COLOR_ACTIVEBORDER            10
%define COLOR_INACTIVEBORDER          11
%define COLOR_APPWORKSPACE            12
%define COLOR_HIGHLIGHT               13
%define COLOR_HIGHLIGHTTEXT           14
%define COLOR_BTNFACE                 15
%define COLOR_BTNSHADOW               16
%define COLOR_GRAYTEXT                17
%define COLOR_BTNTEXT                 18
%define COLOR_INACTIVECAPTIONTEXT     19
%define COLOR_BTNHIGHLIGHT            20
%define COLOR_3DDKSHADOW              21
%define COLOR_3DLIGHT                 22
%define COLOR_INFOTEXT                23
%define COLOR_INFOBK                  24
%define COLOR_HOTLIGHT                26
%define COLOR_GRADIENTACTIVECAPTION   27
%define COLOR_GRADIENTINACTIVECAPTION 28
%define COLOR_MENUHILIGHT             29
%define COLOR_MENUBAR                 30
;-----------------------------------------+

%define CW_USEDEFAULT 0x80000000


;-----------------------------------------+
;         Macros para Load Image          |
;-----------------------------------------+
%define IMAGE_BITMAP        0
%define IMAGE_ICON          1
%define IMAGE_CURSOR        2
%define IMAGE_ENHMETAFILE   3
%define LR_DEFAULTCOLOR     0x00000000
%define LR_MONOCHROME       0x00000001
%define LR_COLOR            0x00000002
%define LR_COPYRETURNORG    0x00000004
%define LR_COPYDELETEORG    0x00000008
%define LR_LOADFROMFILE     0x00000010
%define LR_LOADTRANSPARENT  0x00000020
%define LR_DEFAULTSIZE      0x00000040
%define LR_VGACOLOR         0x00000080
%define LR_LOADMAP3DCOLORS  0x00001000
%define LR_CREATEDIBSECTION 0x00002000
%define LR_COPYFROMRESOURCE 0x00004000
%define LR_SHARED           0x00008000
;-----------------------------------------+


;-----------------------------------------+
;       Macros para Window Messages       |
;-----------------------------------------+
%define WM_NULL                  0x0000
%define WM_CREATE                0x0001
%define WM_DESTROY               0x0002
%define WM_MOVE                  0x0003
%define WM_SIZE                  0x0005
%define WM_ACTIVATE              0x0006
%define WM_SETFOCUS              0x0007
%define WM_KILLFOCUS             0x0008
%define WM_ENABLE                0x000A
%define WM_SETREDRAW             0x000B
%define WM_SETTEXT               0x000C
%define WM_GETTEXT               0x000D
%define WM_GETTEXTLENGTH         0x000E
%define WM_PAINT                 0x000F
%define WM_CLOSE                 0x0010
%define WM_QUERYENDSESSION       0x0011
%define WM_QUERYOPEN             0x0013
%define WM_ENDSESSION            0x0016
%define WM_QUIT                  0x0012
%define WM_ERASEBKGND            0x0014
%define WM_SYSCOLORCHANGE        0x0015
%define WM_SHOWWINDOW            0x0018
%define WM_WININICHANGE          0x001A
%define WM_DEVMODECHANGE         0x001B
%define WM_ACTIVATEAPP           0x001C
%define WM_FONTCHANGE            0x001D
%define WM_TIMECHANGE            0x001E
%define WM_CANCELMODE            0x001F
%define WM_SETCURSOR             0x0020
%define WM_MOUSEACTIVATE         0x0021
%define WM_CHILDACTIVATE         0x0022
%define WM_QUEUESYNC             0x0023
%define WM_GETMINMAXINFO         0x0024
;-----------------------------------------+

;-----------------------------------------+
;         Macros para Show Window         |
;-----------------------------------------+
%define SW_HIDE             0
%define SW_SHOWNORMAL       1
%define SW_NORMAL           1
%define SW_SHOWMINIMIZED    2
%define SW_SHOWMAXIMIZED    3
%define SW_MAXIMIZE         3
%define SW_SHOWNOACTIVATE   4
%define SW_SHOW             5
%define SW_MINIMIZE         6
%define SW_SHOWMINNOACTIVE  7
%define SW_SHOWNA           8
%define SW_RESTORE          9
%define SW_SHOWDEFAULT      10
%define SW_FORCEMINIMIZE    11
%define SW_MAX              11
;-----------------------------------------+


;-----------------------------------------+
;       Macros para Window Styles         |
;-----------------------------------------+
%define WS_OVERLAPPED       0x00000000
%define WS_POPUP            0x80000000
%define WS_CHILD            0x40000000
%define WS_MINIMIZE         0x20000000
%define WS_VISIBLE          0x10000000
%define WS_DISABLED         0x08000000
%define WS_CLIPSIBLINGS     0x04000000
%define WS_CLIPCHILDREN     0x02000000
%define WS_MAXIMIZE         0x01000000
%define WS_CAPTION          0x00C00000
%define WS_BORDER           0x00800000
%define WS_DLGFRAME         0x00400000
%define WS_VSCROLL          0x00200000
%define WS_HSCROLL          0x00100000
%define WS_SYSMENU          0x00080000
%define WS_THICKFRAME       0x00040000
%define WS_GROUP            0x00020000
%define WS_TABSTOP          0x00010000
%define WS_MINIMIZEBOX      0x00020000
%define WS_MAXIMIZEBOX      0x00010000
%define WS_TILED            WS_OVERLAPPED
%define WS_ICONIC           WS_MINIMIZE
%define WS_SIZEBOX          WS_THICKFRAME
%define WS_TILEDWINDOW      WS_OVERLAPPEDWINDOW
%define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED     | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_THICKFRAME     | \
                             WS_MINIMIZEBOX    | \
                             WS_MAXIMIZEBOX)
%define WS_POPUPWINDOW      (WS_POPUP          | \
                             WS_BORDER         | \
                             WS_SYSMENU)
%define WS_CHILDWINDOW      (WS_CHILD)
;-----------------------------------------+


;-----------------------------------------+
;   Macros para Extended Window Styles    |
;-----------------------------------------+
%define WS_EX_DLGMODALFRAME       0x00000001
%define WS_EX_NOPARENTNOTIFY      0x00000004
%define WS_EX_TOPMOST             0x00000008
%define WS_EX_ACCEPTFILES         0x00000010
%define WS_EX_TRANSPARENT         0x00000020
%define WS_EX_MDICHILD            0x00000040
%define WS_EX_TOOLWINDOW          0x00000080
%define WS_EX_WINDOWEDGE          0x00000100
%define WS_EX_CLIENTEDGE          0x00000200
%define WS_EX_CONTEXTHELP         0x00000400
%define WS_EX_RIGHT               0x00001000
%define WS_EX_LEFT                0x00000000
%define WS_EX_RTLREADING          0x00002000
%define WS_EX_LTRREADING          0x00000000
%define WS_EX_LEFTSCROLLBAR       0x00004000
%define WS_EX_RIGHTSCROLLBAR      0x00000000
%define WS_EX_CONTROLPARENT       0x00010000
%define WS_EX_STATICEDGE          0x00020000
%define WS_EX_APPWINDOW           0x00040000
%define WS_EX_OVERLAPPEDWINDOW    (WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE)
%define WS_EX_PALETTEWINDOW       (WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST)
%define WS_EX_LAYERED             0x00080000
%define WS_EX_NOINHERITLAYOUT     0x00100000
%define WS_EX_NOREDIRECTIONBITMAP 0x00200000
%define WS_EX_LAYOUTRTL           0x00400000
%define WS_EX_COMPOSITED          0x02000000
%define WS_EX_NOACTIVATE          0x08000000
;-----------------------------------------+


struc WNDCLASSEXA
	.cbSize:        resd 1
	.style:         resd 1
    .lpfnWndProc:   resq 1
	.cbClsExtra:    resd 1
	.cbWndExtra:    resd 1
	.hInstance:     resq 1
	.hIcon:         resq 1
	.hCursor:       resq 1
	.hbrBackground: resq 1
	.lpszMenuName:  resq 1
	.lpszClassName: resq 1
	.hIconSm:       resq 1
endstruc
%define Size_WNDCLASSEXA 80


struc MSG
	.hwnd:     resq 1
	.message:  resd 1
	.wParam:   resq 1
	.lParam:   resq 1
	.time:     resd 1
	.pt:       resq 1
	.lPrivate: resd 1
endstruc
%define Size_MSG 44


struc RECT
	.left:   resd 1
	.top:    resd 1
	.right:  resd 1
	.bottom: resd 1
endstruc
%define Size_RECT 32


struc PAINTSTRUC
	.hdc:     resq 1
	.fErase:  resb 1
	.rcPaint: resd 4
	
	.rcPaint.left   equ PAINTSTRUC.rcPaint + RECT.left
	.rcPaint.top    equ PAINTSTRUC.rcPaint + RECT.top
	.rcPaint.right  equ PAINTSTRUC.rcPaint + RECT.right
	.rcPaint.bottom equ PAINTSTRUC.rcPaint + RECT.bottom
	
	.fRestore:    resb 1
	.fIncUpdate:  resb 1
	.rgbReserved: resb 32
endstruc
%define Size_PAINTSTRUC 89