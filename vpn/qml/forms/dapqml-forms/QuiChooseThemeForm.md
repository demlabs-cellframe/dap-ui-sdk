@page pageQuiChooseThemeForm QuiChooseThemeForm

## Properties

| name          | type | description |
| ------------- | ---- | ----------- |
| formName | string | form name. used to connect interface via Manager |

## Signals

| name          | description |
| ------------- | ----------- |
| sigSelect | item clicked (int index, string name) |

## Functions

| name          | description |
| ------------- | ----------- |
| updateState | request theme update ( DapQmlStyle.requestRedraw() ) and start return timer |
| updateChecks | item clicked. this will prevent double checks and none checks |

## Structure

Form is built using:
- Theme model
- Resizer for items
- Listview with DapQmlRadioButton as delegate
