#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <Xlsx/Chart.h>
#include <Xlsx/Workbook.h>

using namespace SimpleXlsx;

int main()
{
    setlocale( LC_ALL, "" );

    // Main object - workbook
    CWorkbook Book( "Incognito" );

    // Data sheet to store data to be referenced
    CWorksheet & DataSheet = Book.AddSheet( "Data" );
    const int ColCount = 11;
    const double BarData[ ColCount ] = { 0.7, 0.98, 1.84, 0.3, 0.32, 0.46, 0.16, 1.42, 0.78, 1.74, 0.72 };
    const double LinearData[ ColCount ] = { 0.2, 1.18, 1.0, 0.7, 0.98, 1.84, 0.3, 0.32, 0.46, 0.16, 1.42 };
    DataSheet.BeginRow();
    for( int i = 0; i < ColCount; ++i )
        DataSheet.AddCell( BarData[ i ] );
    DataSheet.EndRow();
    DataSheet.BeginRow();
    for( int i = 0; i < ColCount; ++i )
        DataSheet.AddCell( LinearData[ i ] );
    DataSheet.EndRow();

    // Adding chart sheet to the workbook the reference to a newly created object is returned
    CChartsheet & ChartSheet = Book.AddChartSheet( "Line and bar chart", CHART_LINEAR );
    CChart & Chart = ChartSheet.Chart();
    Chart.SetDiagrammName( "Line and bar chart" );
    Chart.SetXAxisName( "Line chart x axis" );
    Chart.SetYAxisName( "Line chart y axis" );
    Chart.SetX2AxisName( "Bar chart x axis" );
    Chart.SetY2AxisName( "Bar chart y axis" );
    Chart.SetLegendPos( CChart::EPosition::POS_RIGHT_ASIDE );
    Chart.SetAddType( EChartTypes::CHART_BAR );
    // Set some additional parameters for bar chart
    Chart.SetTableDataState( CChart::TBL_DATA_N_KEYS );
    Chart.SetBarDirection( CChart::BAR_DIR_VERTICAL );
    Chart.SetBarGrouping( CChart::BAR_GROUP_CLUSTERED );
    // Switch X and X2 axis to cat - nothing about "categorical axis", just specific mode, default is false;
    Chart.SetXAxisToCat();
    Chart.SetX2AxisToCat();

    // Create line series object
    CChart::Series LineSeries;
    LineSeries.title = "Line series";
    LineSeries.JoinType = CChart::Series::joinSmooth;
    LineSeries.DashType = CChart::Series::dashSolid;
    LineSeries.LineColor = "4A7EBB";
    LineSeries.LineWidth = 2.0;
    // Leave category sequence (X axis) not specified (optional) - MS Excel will generate the default sequence automatically
    LineSeries.catSheet = NULL;
    // Specify range for values` sequence (Y axis)
    LineSeries.valAxisFrom = CellCoord( 2, 0 );
    LineSeries.valAxisTo = CellCoord( 2, ColCount - 1 );
    LineSeries.valSheet = & DataSheet; // don`t forget to set the pointer to the data sheet

    // Create bar series object
    CChart::Series BarSeries;
    BarSeries.title = "Bar series";
    BarSeries.barInvertIfNegative = false;
    // Leave category sequence (X axis) not specified (optional) - MS Excel will generate the default sequence automatically
    BarSeries.catSheet = NULL;
    // Specify range for values` sequence (Y axis)
    BarSeries.valAxisFrom = CellCoord( 1, 0 );
    BarSeries.valAxisTo = CellCoord( 1, ColCount - 1 );
    BarSeries.valSheet = & DataSheet; // don`t forget to set the pointer to the data sheet

    // Add series into the chart (you can add as many series as you wish into the same chart)
    Chart.AddSeries( LineSeries, true );
    Chart.AddSeries( BarSeries, false );

    // Setting marks for the vertical axes
    CChart::TickMark Marks;
    Marks.type = CChart::MARK_OUTSIDE;
    Marks.unit = 0.2;
    Chart.SetYAxisMajorTickMarks( Marks );
    Chart.SetY2AxisMajorTickMarks( Marks );
    Marks.type = CChart::MARK_INSIDE;
    Marks.unit = 0.04;
    Chart.SetYAxisMinorTickMarks( Marks );
    Chart.SetY2AxisMinorTickMarks( Marks );

    Book.SetActiveSheet( ChartSheet );
    if( Book.Save( "MixedLineAndBarChart.xlsx" ) ) std::cout << "The book has been saved successfully" << std::endl;
    else std::cout << "The book saving has been failed" << std::endl;
}
