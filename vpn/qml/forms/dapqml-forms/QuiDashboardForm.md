@page pageQuiDashboardForm QuiDashboardForm

## Properties

| name          | type | description |
| ------------- | ---- | ----------- |
| formName | string | form name. used to connect interface via Manager |

## Signals

| name          | description |
| ------------- | ----------- |
| sigSwitchToggle | switch toggled (bool state). will be sended before sigConnectionStatusChangeRequested |
| sigServerClicked | "switch server" button clicked |
| sigConnectionStatusChangeRequested | will be sended on Switch clicked after sigSwitchToggle |

## Functions

| name          | description |
| ------------- | ----------- |
| setStatusText | change status text (bold, title) |
| setButtonState | change switch state |
| setErrorText | show error message (red, below title) |
| setStatusIndicator | change color of small icon close to connection time |
| setTimeText | change connection time |
| updateSpeedIndicators | change speed indicators texts |
| setServerInfo | change "switch server" button text |

## Structure

Form is built using:
- Title
- Indicator with Connection Time label
- Error label
- Switch
- Speed indicators
- Switch Server button
