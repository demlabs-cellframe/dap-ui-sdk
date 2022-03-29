@page pageQuiSettingsForm QuiSettingsForm

## Properties

| name          | type | description |
| ------------- | ---- | ----------- |
| formName | string | form name. used to connect interface via Manager |

## Structure

Form is built using ListView with DapQmlButton and DapQmlLabel as delegate

Delegate choses when to display DapQmlButton or DapQmlLabel

If DapQmlButton is clicked, delegate executes settingsModel.exec static method
