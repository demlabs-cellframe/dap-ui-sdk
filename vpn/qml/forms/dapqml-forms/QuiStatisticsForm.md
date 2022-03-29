@page pageQuiStatisticsForm QuiStatisticsForm

## Properties

| name          | type | description |
| ------------- | ---- | ----------- |
| formName | string | form name. used to connect interface via Manager |
| leftColumnPos | int | coordinate for left row items (calculated on resize) |
| rightColumnPos | int | coordinate for left row items (calculated on resize) |

## Functions

| name          | description |
| ------------- | ----------- |
| updateIndicators | change all indicators text |
| setUptime | change uptime label text |
| resizeFont | resize font for provided DapQmlButton |
| moveColumns | calculate column positions |

## Structure

Form is built using:
- 2 resizers
- Graph provided by QmlSchedulePainter
- 2 rows of indicators
