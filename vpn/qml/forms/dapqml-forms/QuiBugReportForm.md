@page pageQuiBugReportForm QuiBugReportForm

## Properties

| name          | type | description |
| ------------- | ---- | ----------- |
| mode | int | current mode (see QuiBugReportForm.Mode) |
| formName | string | form name. used to connect interface via Manager |

## Signals

| name          | description |
| ------------- | ----------- |
| sigSend       | button send clicked (report input) |
| sigCancel     | button cancel clicked (report sending) |
| sigResultBack | button back clicked (report success\\error) |

## Functions

| name          | description |
| ------------- | ----------- |
| setmode | set form mode |
| setResultText | set report sending result text |

Three modes available

| name          | description |
| ------------- | ----------- |
| QuiBugReportForm.Mode.Write | report input screen |
| QuiBugReportForm.Mode.Loading | report is sending (spinner) |
| QuiBugReportForm.Mode.Result | report success\\error |

## Structure

Form is built using three separate Item's which only
visible if desired mode is active.

Button also change mode, so it is not necessary to call
setmode function from outside of the form.

Form contains resizer item for advanced styling technic.
