@page pageQuiPurchaseForm QuiPurchaseForm

## Properties

| name          | type | description |
| ------------- | ---- | ----------- |
| formName | string | form name. used to connect interface via Manager |

## Signals

| name          | description |
| ------------- | ----------- |
| sig1month | 1 month plan clicked |
| sig6months | 6 months plan clicked |
| sig1year | 1 year plan clicked |

## Functions

| name          | description |
| ------------- | ----------- |
| btnClicked | sends signal for desired button |

## Structure

Form is built using:
- Model
- Resizer
- ListView with DapQmlButton as delegate
