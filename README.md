# MacroinvertebratesV3 #

## Wotizit? ##

A cross platform rewrite of the original EPSCoR iOS application; buildable for GNU/Linx, MacOS, Windows, iOS and Android.

## Wotzitdo? ##

For the biologist in all of us, the Vermont EPSCoR Macroinvertebrate Field Guide provides a glimpse of the life in Vermont, New York and Puerto Rico's streams. Search by stream name to discover what is living in each stream, then tap through animals to learn more.

## Developing ##

#### Adding initial data ####

The initial data is compiled into the application as a part of the resource files. Once the application has been run at least once, and a sync with the server has occured two data files, and a directory with images should have been generated. The location of this directory is platform dependent and is controlled by the combination of the application's calls to setOrganization* and Qt's QStandardPaths::writableLocation(QStandardPaths::DataLocation). On OSX that directory is "/Users/$USER/Library/Application Support/EPSCOR/Macroinvertebrates".

Copy the files invertebrate.data, stream.data and the directory images to the data directory in the project directory. Open the editor for media.qrc and add all files in the images directory.

You're done! The next time the application is compiled the initial data will be baked in.
