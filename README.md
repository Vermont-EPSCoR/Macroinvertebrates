# MacroinvertebratesV3 #

## Wotizit? ##

A cross platform rewrite of the original EPSCoR iOS application; buildable for GNU/Linux, MacOS, Windows, iOS and Android.

## Wotzitdo? ##

For the biologist in all of us, the Vermont EPSCoR Macroinvertebrate Field Guide provides a glimpse of the life in Vermont, New York and Puerto Rico's streams. Search by stream name to discover what is living in each stream, then tap through animals to learn more.

## Developing ##

#### Adding initial data ####

The initial data is compiled into the application as a part of the resource files. Once the application has been run at least once, and a sync with the server has occured two data files, and a directory with images should have been generated. The location of this directory is platform dependent and is controlled by the combination of the application's calls to setOrganization* and Qt's `QStandardPaths::writableLocation(QStandardPaths::DataLocation)`. On OSX that directory is `/Users/$USER/Library/Application Support/EPSCOR/Macroinvertebrates`.

Copy the files invertebrate.data, stream.data and the directory images to the data directory in the project directory. Open the editor for media.qrc and add all files in the images directory.

You're done! The next time the application is compiled the initial data will be baked in.

#### Quirks when testing on iOS Simulator ####

There is a setting in iOS Simulator to reset the device's stored data and settings, at least for this application that feature does not work. To manually remove those files have a look in the following directories:

    /Users/$USER/Library/Application Support/iPhone Simulator/2.2/Library/Application Support/EPSCOR/Macroinvertebrates

    /Users/$USER/Library/Application Support/iPhone Simulator/2.2/Library/Preferences

Your mileage may vary with these paths depending on the version of the iOS Simulator.

#### Android Signing Key ####

Pat has the Android signing key. Ask him if you want to post a new version to Google Play. Password is in KeePass.
