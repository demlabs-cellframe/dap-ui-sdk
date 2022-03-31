@page pageQuiChooseServerForm QuiChooseServerForm

## Properties

| name          | type | description |
| ------------- | ---- | ----------- |
| formName | string | form name. used to connect interface via Manager |
| items | Array | contains displayed items |

## Signals

| name          | description |
| ------------- | ----------- |
| sigSelect | item clicked (int index, string name) |

## Functions

| name          | description |
| ------------- | ----------- |
| setCurrentIndex | change current item index (int index) |

## Structure

Form is built using simple ListView with DapQmlRadioButton as delegate

Form contains resizer for ListView items
