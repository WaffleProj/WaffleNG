#ifndef __TTF_H_
#define __TTF_H_

#include <Windows.h>

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define TTF_WORD(x) MAKEWORD(HIBYTE(x), LOBYTE(x))
#define TTF_LONG(x) MAKELONG(TTF_WORD(HIWORD(x)), TTF_WORD(LOWORD(x)))
#else
#define TTF_WORD(x) (x)
#define TTF_LONG(x) (x)
#endif

typedef struct _TT_OFFSET_TABLE
{
    FIXED   sfntVersion;
    USHORT  numTables;
    USHORT  searchRange;
    USHORT  entrySelector;
    USHORT  rangeShift;
} TT_OFFSET_TABLE, *LPTT_OFFSET_TABLE;

typedef struct _TT_TABLE_DIRECTORY
{
    ULONG   tag;        //table name
    ULONG   checkSum;   //Check sum
    ULONG   offset;     //Offset from beginning of file
    ULONG   length;     //length of the table in bytes
} TT_TABLE_DIRECTORY, *LPTT_TABLE_DIRECTORY;

typedef struct _TT_NAME_TABLE_HEADER
{
    USHORT  formatSelector; //format selector. Always 0
    USHORT  nNameRecord;    //Name Records count
    USHORT  offset;         //Offset for strings storage, from start of the table
} TT_NAME_TABLE_HEADER, *LPTT_NAME_TABLE_HEADER;

typedef struct _TT_NAME_RECORD
{
    USHORT  platformID;
    USHORT  platformEncodingID;
    USHORT  languageID;
    USHORT  nameID;
    USHORT  nString;
    USHORT  stringOffset;  //from start of storage area
} TT_NAME_RECORD, *LPTT_NAME_RECORD;

#define TT_LANG_ENGLISH MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)
#define TT_LANG_NEUTRAL MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL)

#define TT_LOCALE_ENGLISH MAKELCID(TT_LANG_ENGLISH, SORT_DEFAULT)
#define TT_LOCALE_NEUTRAL MAKELCID(TT_LANG_NEUTRAL, SORT_DEFAULT)

#define TT_TABLE_NAME   TTF_LONG('name')

#define TT_PLATFORM_ID_APPLE_UNICODE    0
#define TT_PLATFORM_ID_MACINTOSH        1
#define TT_PLATFORM_ID_ISO              2
#define TT_PLATFORM_ID_MICROSOFT        3

#define TT_NAME_ID_COPYRIGHT            0
#define TT_NAME_ID_FAMILY               1
#define TT_NAME_ID_SUBFAMILY            2
#define TT_NAME_ID_UNIQUE_IDENTIFIER    3
#define TT_NAME_ID_FULL_NAME            4
#define TT_NAME_ID_VERSION              5
#define TT_NAME_ID_POSTSCRIPT_NAME      6
#define TT_NAME_ID_TRADEMARK            7

#endif /* __TTF_H_ */