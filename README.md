# UE4 Map Check Validation

This plug-in allows you to call the MapCheckValidation commandlet to check for errors on your map.

## Usage

You can call commandlets using the `-run=` parameter. The MapCheckValidation commandlet only needs one parameter `-maps=` and you can separate the maps you want  to check with commas.

Ex:

`UE4Editor.exe -run=MapCheckValidation -project=PATH_TO_YOUR_UPROJECT -maps=Map1,Map2`