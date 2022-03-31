@page pageQuiLoginForm QuiLoginForm

## Properties

| name          | type | description |
| ------------- | ---- | ----------- |
| formName | string | form name. used to connect interface via Manager |

## Signals

| name          | description |
| ------------- | ----------- |
| sigChooseServer | choose server button clicked |
| sigChooseSerial | enter serial key clicked |
| sigConnect | connect button clicked |
| sigObtainNewKey | buy serial clicked |
| sigSerialFillingIncorrect | entered serial is incorerct |

## Functions

| name          | description |
| ------------- | ----------- |
| setStatusMessage | change error label text |
| setSerial | change serial key button content |
| setConnectionEnabled | enable\\disable connect button |
| setKeyEnterEnabled | show\\hide serial enter button |
| setServer | change current chosen server name |

## Structure

Form is built using:
- Logo
- Error label
- Choose server
- Enter serial
- Connect button
- Obtain key link
