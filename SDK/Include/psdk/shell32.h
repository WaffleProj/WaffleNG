#ifndef __SDK_WAFFLE_PSDK_SHELL32_H_
#define __SDK_WAFFLE_PSDK_SHELL32_H_

typedef int (WINAPI *LPSHELLABOUTW)(
    _In_opt_    HWND hWnd,
    _In_        LPCTSTR szApp,
    _In_opt_    LPCTSTR szOtherStuff,
    _In_opt_    HICON hIcon
    );

#endif /* __SDK_WAFFLE_PSDK_SHELL32_H_ */