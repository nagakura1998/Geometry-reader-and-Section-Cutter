#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
//#include <typeinfo>
#include <limits>
#include <typeindex>
#include <algorithm>
#include <Xlsx/Workbook.h>
#include <scratch/clsScratchTable.h>
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef QT_CORE_LIB
#include <QDateTime>
#endif

/*Based on example "Simple.cpp" from
  SimpleXlsxWriter
  Copyright (C) 2012-2021 Pavel Akimov <oxod.pavel@gmail.com>, Alexandr Belyak <programmeralex@bk.ru>
*/
using namespace SimpleXlsx;




int main()
{
    setlocale( LC_ALL, "" );
    time_t CurTime = time( NULL );

    CWorkbook book( "Incognito" );
    std::vector<ColumnWidth> ColWidth;
    ColWidth.push_back( ColumnWidth( 0, 3, 25 ) );
    CWorksheet & Sheet = book.AddSheet( "Unicode", ColWidth );

    Style style;
    style.horizAlign = ALIGN_H_CENTER;
    style.font.attributes = FONT_BOLD;
    const size_t CenterStyleIndex = book.AddStyle( style );

    Sheet.BeginRow();
    Sheet.AddCell( "Common test of Unicode support", CenterStyleIndex );
    Sheet.MergeCells( CellCoord( 1, 0 ), CellCoord( 1, 3 ) );
    Sheet.EndRow();

    Font TmpFont = book.GetFonts().front();
    TmpFont.attributes = FONT_ITALIC;
    Comment Com;
    Com.x = 300;
    Com.y = 100;
    Com.width = 100;
    Com.height = 30;
    Com.cellRef = CellCoord( 8, 1 );
    Com.isHidden = false;
    Com.AddContent( TmpFont, "Comment with custom style" );
    Sheet.AddComment( Com );

    Sheet.BeginRow().AddCell( "English language" ).AddCell( "English language" ).EndRow();
    Sheet.BeginRow().AddCell( "Russian language" ).AddCell( L"Русский язык" ).EndRow();
    Sheet.BeginRow().AddCell( "Chinese language" ).AddCell( L"中文" ).EndRow();
    Sheet.BeginRow().AddCell( "French language" ).AddCell( L"le français" ).EndRow();
    Sheet.BeginRow().AddCell( "Arabic language" ).AddCell( L"العَرَبِيَّة‎‎" ).EndRow();

    Sheet.AddEmptyRow();

    style.fill.patternType = PATTERN_NONE;
    style.font.theme = true;
    style.horizAlign = ALIGN_H_RIGHT;
    style.vertAlign = ALIGN_V_CENTER;

    style.numFormat.numberStyle = NUMSTYLE_MONEY;
    const size_t MoneyStyleIndex = book.AddStyle( style );
    Sheet.BeginRow().AddCell( "Money symbol" ).AddCell( 123.45, MoneyStyleIndex ).EndRow();

    Style stPanel;
    stPanel.border.top.style = BORDER_THIN;
    stPanel.border.bottom.color = "FF000000";
    stPanel.fill.patternType = PATTERN_SOLID;
    stPanel.fill.fgColor = "FFCCCCFF";
    const size_t PanelStyleIndex = book.AddStyle( stPanel );
    Sheet.AddEmptyRow().BeginRow();
    Sheet.AddCell( "Cells with border", PanelStyleIndex );
    Sheet.AddCell( "", PanelStyleIndex ).AddCell( "", PanelStyleIndex ).AddCell( "", PanelStyleIndex );
    Sheet.EndRow();

    style.numFormat.numberStyle = NUMSTYLE_DATETIME;
    style.font.attributes = FONT_NORMAL;
    style.horizAlign = ALIGN_H_LEFT;
    const size_t DateTimeStyleIndex = book.AddStyle( style );
    Sheet.AddEmptyRow().AddSimpleRow( "time_t", CenterStyleIndex );
    Sheet.AddSimpleRow( CellDataTime( CurTime, DateTimeStyleIndex ) );

    Style stRotated;
    stRotated.horizAlign = EAlignHoriz::ALIGN_H_CENTER;
    stRotated.vertAlign = EAlignVert::ALIGN_V_CENTER;
    stRotated.textRotation = 45;
    const size_t RotatedStyleIndex = book.AddStyle( stRotated );
    Sheet.AddSimpleRow( "Rotated text", RotatedStyleIndex, 3, 20 );
    Sheet.MergeCells( CellCoord( 14, 3 ), CellCoord( 19, 3 ) );

    /* Be careful with the style of date and time.
     * If milliseconds are specified, then the style used should take them into account.
     * Otherwise, Excel will round milliseconds and may change seconds.
     * See example below. */
    style.numFormat.formatString = "yyyy.mm.dd hh:mm:ss.000";
    const size_t CustomDateTimeStyleIndex = book.AddStyle( style );
    Sheet.AddSimpleRow( "Direct date and time", CenterStyleIndex );
    Sheet.AddSimpleRow( CellDataTime( 2020, 1, 1, 0, 0, 0, 500, CustomDateTimeStyleIndex ) );
    Sheet.AddSimpleRow( CellDataTime( 2020, 1, 1, 0, 0, 0, 500, DateTimeStyleIndex ) );
    Sheet.AddSimpleRow( CellDataTime( 2020, 1, 1, 0, 0, 0, 499, CustomDateTimeStyleIndex ) );
    Sheet.AddSimpleRow( CellDataTime( 2020, 1, 1, 0, 0, 0, 499, DateTimeStyleIndex ) );


    Sheet.AddEmptyRow().AddSimpleRow( "Windows SYSTEMTIME", CenterStyleIndex );
#ifdef _WIN32
    SYSTEMTIME lt;
    GetLocalTime( & lt );
    Sheet.AddSimpleRow( CellDataTime( lt, CustomDateTimeStyleIndex ) );
#endif


using namespace ScratchTable;




     clsScratchTable Tbl;
     clsCell cl;
     cl.Set(0,2,CurTime,CustomDateTimeStyleIndex);
     Tbl.Add(cl);
     cl.Set(0,1,"Entry");
     Tbl.Add(cl);
     cl.Set(0,0,"0:0");
     Tbl.Add(cl);
     cl.Set(2,2,"2:2");
     Tbl.Add(cl);
     cl.Set(3,0,"0:3, Long");
     Tbl.Add(cl);
     cl.SetLong(3,1,std::numeric_limits<int64_t>::max());
     Tbl.Add(cl);

     cl.Set(1,1,"1:1");
     Tbl.Add(cl);
     cl.Set(8,1,"8:1");
     Tbl.Add(cl);
#ifdef _WIN32
     cl.Set(1,12,lt,DateTimeStyleIndex);
     Tbl.Add(cl);
#endif
     cl.Set(1,11,"1:11");
     Tbl.Add(cl);
     Tbl.Add(cl.Set(7,7,"7:7"));
	 size_t Nr,Nc;
	 CWorksheet::SParams Params;
	 Tbl.GetDim(Nr,Nc);
	 Params.LastUsedCell = CellCoord( Nr, Nc );
	 CWorksheet & Sheet1 = book.AddSheet( "tbltst", Params);


     Tbl.InsertTo(Sheet1);
     Tbl.Del(0,2);

	 Tbl.GetDim(Nr,Nc);
	 Params.LastUsedCell = CellCoord( Nr, Nc );
	 CWorksheet & Sheet2 = book.AddSheet( "tbltstdel", Params);
     Tbl.InsertTo(Sheet2);

    Com.cellRef = CellCoord( 6, 0 );
    Com.isHidden = false;
    Com.AddContent( TmpFont, "Second" );
    Sheet.AddComment( Com );

    Com.cellRef = CellCoord( 1, 0 );
    Com.isHidden = false;
    Com.AddContent( TmpFont, "Second" );
    Sheet1.AddComment( Com );
#ifdef _WIN32
    LARGE_INTEGER startTime, addTime, insTime, wrTime;
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
#endif
{
    CWorksheet & Sheet3 = book.AddSheet( "tbllong", ColWidth );
    clsScratchTable TbL;
    TbL.Add(cl.Set(2,2,"Entry",0));
#ifdef _WIN32
     QueryPerformanceCounter(&startTime);
#endif
    for(size_t idxR=0;idxR<32000;++idxR){
     //TbL.Add(cl.Set(idxR,0,idxR,0));
        TbL.Add(cl.SetLong(idxR,0,static_cast<uint64_t>(idxR),0));
     TbL.Add(cl.Set(idxR,1,1.*idxR,0));
     TbL.Add(cl.Set(idxR,2,1./(1.+idxR),0));
    }
#ifdef _WIN32
      QueryPerformanceCounter(&addTime);
#endif
     TbL.Add(cl.Set(2,2,"Test",0));
     TbL.Add(cl.Set(2,2,"Test Passed",0));
     TbL.Del(1,2);
     TbL.InsertTo(Sheet3);
#ifdef _WIN32
     QueryPerformanceCounter(&insTime);
#endif

}


    if( book.Save( "SimpleScratch.xlsx" ) ) {
#ifdef _WIN32
     QueryPerformanceCounter(&wrTime);
     double daTime= (addTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;
     double diTime= (insTime.QuadPart - addTime.QuadPart) * 1000.0 / frequency.QuadPart;
     double dwTime= (wrTime.QuadPart - insTime.QuadPart) * 1000.0 / frequency.QuadPart;
     std::cout << "Times:\t"<<daTime<<"\t"<<diTime<<"\t"<<dwTime<< std::endl;
#endif
     std::cout << "The book has been saved successfully" << std::endl;
    }

    else std::cout << "The book saving has been failed" << std::endl;


    return 0;
}
