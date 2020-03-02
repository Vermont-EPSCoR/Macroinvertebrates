#-------------------------------------------------
#
# Project created by QtCreator 2016-04-08T12:23:16
#
#-------------------------------------------------

QT       += core gui network xml svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Macroinvertebrates
TEMPLATE = app


SOURCES += application.cpp \
    deps/gumbo-parser/attribute.c \
    deps/gumbo-parser/char_ref.c \
    deps/gumbo-parser/error.c \
    deps/gumbo-parser/parser.c \
    deps/gumbo-parser/string_buffer.c \
    deps/gumbo-parser/string_piece.c \
    deps/gumbo-parser/tag.c \
    deps/gumbo-parser/tokenizer.c \
    deps/gumbo-parser/utf8.c \
    deps/gumbo-parser/util.c \
    deps/gumbo-parser/vector.c \
    deps/QGumboParser/qgumboattribute.cpp \
    deps/QGumboParser/qgumbodocument.cpp \
    deps/QGumboParser/qgumbonode.cpp \
    main.cpp \
    models/invertebrate.cpp \
    models/stream.cpp \
    parsers/invertebratehandler.cpp \
    parsers/streamhandler.cpp \
    views/aboutview.cpp \
    views/homeview.cpp \
    views/invertebrateview.cpp \
    views/settingsview.cpp \
    views/singlestreamview.cpp \
    views/streamview.cpp \
    webdatasynchronizer.cpp \
    views/mainwindow.cpp \
    models/invertebrateitemmodel.cpp


HEADERS  += application.h \
    deps/gumbo-parser/tag_enum.h \
    deps/gumbo-parser/tag_gperf.h \
    deps/gumbo-parser/tag_sizes.h \
    deps/gumbo-parser/tag_strings.h \
    deps/gumbo-parser/attribute.h \
    deps/gumbo-parser/char_ref.h \
    deps/gumbo-parser/char_ref.rl \
    deps/gumbo-parser/error.h \
    deps/gumbo-parser/gumbo.h \
    deps/gumbo-parser/insertion_mode.h \
    deps/gumbo-parser/parser.h \
    deps/gumbo-parser/string_buffer.h \
    deps/gumbo-parser/string_piece.h \
    deps/gumbo-parser/token_type.h \
    deps/gumbo-parser/tokenizer.h \
    deps/gumbo-parser/tokenizer_states.h \
    deps/gumbo-parser/utf8.h \
    deps/gumbo-parser/util.h \
    deps/gumbo-parser/vector.h \
    deps/QGumboParser/HtmlTag.h \
    deps/QGumboParser/qgumboattribute.h \
    deps/QGumboParser/qgumbodocument.h \
    deps/QGumboParser/qgumbonode.h \
    models/invertebrate.h \
    models/stream.h \
    parsers/invertebratehandler.h \
    parsers/streamhandler.h \
    views/aboutview.h \
    views/homeview.h \
    views/invertebrateview.h \
    views/settingsview.h \
    views/singlestreamview.h \
    views/streamview.h \
    webdatasynchronizer.h \
    views/mainwindow.h \
    models/invertebrateitemmodel.h

FORMS    += \
    views/aboutview.ui \
    views/homeview.ui \
    views/invertebrateview.ui \
    views/settingsview.ui \
    views/singlestreamview.ui \
    views/streamview.ui

INCLUDEPATH += "$$PWD/deps/gumbo-parser"
INCLUDEPATH += "$$PWD/deps/QGumboParser"

QMAKE_LFLAGS += -v

CONFIG += c++11               # For QGumbo
QMAKE_CXXFLAGS += -O3
QMAKE_CFLAGS += -std=c99 -O3  # For Gumbo
CONFIG += mobility
MOBILITY =

RESOURCES += \
    media.qrc

DISTFILES += \
    android/gradle/wrapper/gradle-wrapper.properties \
    deps/gumbo-parser/tag.in \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    media/vbig.icns \
    Info.plist \
    data/invertebrate.data \
    data/stream.data \
    data/images/0020028e6ea811bcff773ca23e4bd6a3b4a488b881e678fff3a3ff418c4e8533.jpg \
    data/images/0138e9b3c30a970560b55b68c810d2e2613a7c9a9f3e551faeacf61e5ea5c25b.jpg \
    data/images/01aae58bed327ed24edc31c1fc3f22d7fff125d65fe6e53918349939944fe834.jpg \
    data/images/02cb62fd00bb9d22229f1ada03018146b58833891fc123eec228d63ad3905018.jpg \
    data/images/039990b1dd53858c009e1b70db5b463f1b2f9236a4b251270f30f671d1c1dcf3.jpg \
    data/images/03b924ad95ebe7438a5147e68578cab87f11d22b0d879a5026bd6bf6ceb508b0.jpg \
    data/images/03c9b4582a7a2d227a1168b9dd66cdb4074b06ca0cc1be398fb4634b10685c13.jpg \
    data/images/03fdf4e4e181ae598bd4d0882f1deed9dc0ed3cd1bd8d90d330af667987c431f.jpg \
    data/images/0d3f86041c1c364b88c24574409c004ad38c3a63094f8f2e325834e38dd06566.jpg \
    data/images/0f2b6e78f42938ffb3d5d6e0df7970e75217b4b2905cf2dbaaffd13bcbf24519.jpg \
    data/images/150dccb0b96d368b411f33a5d50ecaf1725188fc949411dd314404cfc928542b.jpg \
    data/images/1586638cc49fc98ff040a628f0b97de7d0378c4ef3ff34d3eb6411ec4a57df25.jpg \
    data/images/17776c744c5fd37897bbbcb6ede6c77926ef5de8cf2a2961fa80246077d77245.jpg \
    data/images/1a0a58f1b91b50381978d609fed18332a47e49104de788413a7454d4930b8d05.jpg \
    data/images/1b7f606ac7b1f232e4106aa453623d0562025f0577abab61eefd77933113311d.jpg \
    data/images/1bb7ffdaef849e18f6fe0ca1892646186bc181563e950c64aff98103b94e25cc.jpg \
    data/images/1dbb3283046cee2b6c20914d220c8ca7e7a708109f90eca440f24d1536e7d98d.jpg \
    data/images/1ecbda2661f31da90472626a918b837d72c133ddecf2bcdc488b4196ffdba7a3.jpg \
    data/images/1fccb04b6947066dfe21c19ae01e2f0d85154a8fbbacb42ef655c21d5aec5463.jpg \
    data/images/217348f2f0190857665091e4e4273a5af321848b70f392d03e5befd8b4d32c45.jpg \
    data/images/239ad3679bbe7822007b331ac354fe2eb6ea194dc5b4429df5228ab7613afa3a.jpg \
    data/images/253bbaf3d54c753d14aa78eca933914c69ce77e1d34a55ff967a24df0d48058e.jpg \
    data/images/2d4e2c3a74c2c3618b4f0e58a3eabd4499566feb2310d03416971148cd93f3c1.jpg \
    data/images/2e314b9d3ef670c51fc7c694fcb802aeb6437b34305c43ee6516715c29e67a5b.jpg \
    data/images/2ee7247c281b365daff9768b3f9f271f5ae1550fd0b97d72b9f3871d7b903ff8.jpg \
    data/images/33e55cd82d6e858470908fe240642fed3e17c2d0df38d7f55cb6e990210e8d39.jpg \
    data/images/34e5f1df9600784590af153f050d4f68955df41e8cbd80db5156ab0224624db8.jpg \
    data/images/35239987fd13cb830acc7bdf01291c3fdcccc4d2fdd6d5134fa20268c105a67f.jpg \
    data/images/38588fd932f46dacb94c211b674265eb2ea93070fcbd1e5298b55da6905aee5a.jpg \
    data/images/39a54786f0fa6c1ce99d9371a3b40bedc0a9148a55614eb51fe80eefbc0552b4.jpg \
    data/images/40609c07001508f907fe745d143c013196e925f5116b6157b6390499d0eecd4c.jpg \
    data/images/40dfa17e372dbde49d9b9b4c07162d2a3064760dffeb6ce5ef9eae5d077b8cab.jpg \
    data/images/41bf16b52630071957d0c21faf2c8af8e6915093d5a7072383ce4e6f8b905036.jpg \
    data/images/42b45121b5d1d1000319318239d428435cfb8feda4a2fd42a792fa397661867d.jpg \
    data/images/4548ee0a6796e25e4ff9ab6b0484a12eb2a2efc77775b64c4f1ac3db7144da2a.jpg \
    data/images/4954899364722a9003e7774e5978b462f3ed99fcc5fb76fb6b0a3325a696c0ac.jpg \
    data/images/4ba7a9b012393dbfe0f602ca79edd4a7b5a190e68720727f2cc3280d8c829e86.jpg \
    data/images/4e32b4f5f34b90e05f671dd96310a8dbbd3f46152efd2c2150a94cafbb4ead5a.jpg \
    data/images/52058de2a95a4d4962a9a4271fc392d0f480b0b6ebf54f315bc1fb61879b8d90.jpg \
    data/images/56064d5ff1b2661fe4100a6d6d8a60549115685ea54d1cd819746974b8fe9ae6.jpg \
    data/images/58ccf8fcd4dd6ce7387ce75e64c7f9104ef2dbad9436c0a1dc35be7a1e3261d0.jpg \
    data/images/59654aa9312bf72bc62d7f2ec143c33acbf9fd048febcb54f1f9e5d30a712beb.jpg \
    data/images/597ca2f6844dcd5ac9db4dbf4426cf2843fb3ce4c0560c8fc6d118f94f6de614.jpg \
    data/images/5a958f81d80cb28670acdd7cca0e8bd0e5502c8ca38c313210dc29a8206de5f6.jpg \
    data/images/5b3901e4918b33ecab13a6142b68eb91947afb25bf1fc4296e64417ef88f51c2.jpg \
    data/images/5b5f2bf78b5ca9920370a8811cbf3045941960c378af9fa3da91fa9a64dd1379.jpg \
    data/images/5be1881273622877ec9cf75309e4eb2e099f43daf1c5a449e6a7782cbdc76325.jpg \
    data/images/5c79591620772949dda38e869faa0c0d08e6a49ddefcd3c680b7ebe64c3b094b.jpg \
    data/images/5cd69b83b834df58b626205753f4ad96de4f5be19443796ab2eded287773bf89.jpg \
    data/images/5d07cab2f0fc8a4983113a11bd307c5762a004847cc94aebef575d3936d6f481.jpg \
    data/images/5d41e9bb35fdd7323002646f1784a2d2e785ea86f43a426ee19345c19ca7bc96.jpg \
    data/images/66b69b3468188b8cd0458e1b15477ebf5f3ef1a2cdc02b56cb32c0ffeaef850a.jpg \
    data/images/66c3c2d7a113f2ea4734c857ab0ce4a92edb114984dc21a162e0846f2d797c8a.jpg \
    data/images/6cd406f1698e2defbd9a608380a41bab3cdc1376c786cd6cd39ae132e3087779.jpg \
    data/images/6f078b1d02c3100450bd3b90030e0097428e3de44abc264137f4dfd5da24b815.jpg \
    data/images/6f46c31c52475e866d790cd8d2a12ba99c20fe5f037952bb44a81da044ea2057.jpg \
    data/images/70261e8e87ceb9aedf282eda30ea2b4d3f6c1c53d4a0721ed4a3a07508f2eaa2.jpg \
    data/images/742c9e13c6e412118827e7dd2caaf1a647e8466936afc4c75ce20afccb766566.jpg \
    data/images/7648ce09f455b8556d0504d1daf9d95b79abeb0042f54174b5d4962d43d18346.jpg \
    data/images/77aa62efde9fbc80df8a52faed2a0efb3a9c754b54788a34417811bc31449ac1.jpg \
    data/images/7a182c538e54371c58c86588850c3dbc92b03b772a2de458d04bb172db507c0f.jpg \
    data/images/7bd01a6743d946f613bc586cdffe78596cfd9a002c1ab7724934791f192a2bd6.jpg \
    data/images/8258d8154489298a60d838a29e73c5ac49d2ec6b3abdbbadbf3701af42605b33.jpg \
    data/images/87836987cbedee74ae16373283cf7978e6f02cd68b9ea9b0b5f762f6a1c54ec0.jpg \
    data/images/87d97ec2b0d51a0a9d55c31028476c20456eaf332822de62b29950cfc79758b2.jpg \
    data/images/8906bdf969150f73e16234280dab2312fd25df183abc20d53c340fa6191446b5.jpg \
    data/images/89d04fa56e3951bc81fa36958e0654371682642c5ab205a181e45ca525835725.jpg \
    data/images/8aaa7e9d650e85832d7c459602baa45c908af91b31f3d46d4b51e859973d4df2.jpg \
    data/images/8b52edb4167dbefa9cfbc4935918832d211d977173f9f5de4cc5739f0872899e.jpg \
    data/images/8e39fcd877bc6c1484b34391e5e4bde78f31e006d566201f72b3e791e6d84833.jpg \
    data/images/8e512047a32f39abe1125aff8677cc6cb44d7efc6e9d2908cd76fad378088f51.jpg \
    data/images/90858bf5b49945ba665c7197eed00e91bbb2d1d21e1cf3aba74839dae0d87660.jpg \
    data/images/94914d432aba7cc1719808063ec91ac611e4ec7b6e8c651a9fbfa271e0d911b2.jpg \
    data/images/953cebc03bc4601e2a5c8a602c38354c6378096d9d10f45ae35ad0a57d07e170.jpg \
    data/images/978f7cdd6ea7d4b82ecf240fa8debad8924429d6a08fe21b8649560a33a25b16.jpg \
    data/images/97f228946d76201c3b1960b713d96b9b81c4b20fe231b74f8ce3bf5455331127.jpg \
    data/images/9ac4e6be096eee69b5f201703a385da994c2b8bf10195296acbc19d25d783727.jpg \
    data/images/9c7fcca14693354229b36235000e13c9242f7927717ecd712a527369b6216619.jpg \
    data/images/9fdfa42a54a160603c3b49780845ad7e51af123595470bfdfd3fad1564932481.jpg \
    data/images/a646aeecad0d889fb5e50ef62c1d932ac31aa5a6371816126194e98896408b42.jpg \
    data/images/a6a6f8e2e30afc4b6fa987e8e4baa2ecb78a8c9ecc6345a42bb2caae9a14d3f9.jpg \
    data/images/a74bfec57a682d11d1c5d6a8a751eb77b849b699d7e3e5b546b7e1c6a782f41e.jpg \
    data/images/ae81065e59199dd7cd6c92474bda6eea2f6353a47a6a1912f5902736935cb126.jpg \
    data/images/aeb662a5c0eddc22d8821bde7a5d683036de2e62bf2ba9049daa3c28fe2c863e.jpg \
    data/images/aebbaf97b8602129a988cc86990d2798b2b2b3ce2e17ff94db56d0fb1c04a8cc.jpg \
    data/images/b0f1c380b24577ff9755c3f298b2450667ff0fe61346edc6b48c2e7b8b177053.jpg \
    data/images/b1fbab7fd594bd8f469a05c9d063894e62241b2d940ba009c414913f18685949.jpg \
    data/images/b316e825af9b7ba6ce87a89be0e7ff384427daa0c6c722a8d22fab7b6bcda09f.jpg \
    data/images/b365b0daecb5373b81443dcd895197e57c091198e2688178af2a6c31576fa767.jpg \
    data/images/b3a6dac036bc3ef756f296ed0e05ed9575eaa6319cf7cb4f4a71b44f712cc74c.jpg \
    data/images/b545ee84f4b4dbaf93feefac3191e86616db986941e962671edc15e774e69c4e.jpg \
    data/images/b5c6ed8281b77a46333351aec117bce0d0eadeed19bdb4ea00ac77748ea5596c.jpg \
    data/images/b6bbc08e8af68907e8206bf621932984a2c31296000998d0a83675d83104a1c6.jpg \
    data/images/b87c67cef171cb350eb2bd447702ddeb3a97d275b082ce851bd7f70e891f5ea7.jpg \
    data/images/ba2a4eb7975694b40cb9cfeab55ddeb62c13fba7fdab07e170c2f707b2bf108e.jpg \
    data/images/bb8d1c7c6c0794226e957dfa78506895f27992590212f5833cf62c8010f89e89.jpg \
    data/images/bcb48e545c97e02a43dc4828a4de667e12c7c5b7cfa7fa5c54a2d726dd8b4a3a.jpg \
    data/images/c115f16d4a0b491b277ac1ad72e838bd7654eadd65f93bd3e96fd1d39c7e8ce9.jpg \
    data/images/c1897db486e1b6ba18cb14fe91b5979538553cf890ac4a48c7f13fd723389ace.jpg \
    data/images/c66769cd9bf2af6ee7da0a2d12e4c71110003699cec2ac19bb04892f2eed3ace.jpg \
    data/images/c9f0d581b273fc200cc9667d5c8a541164ba6c6bc87b67f1d2dd9c8545b26b45.jpg \
    data/images/cad9d56a1b5d4f5652d1d3a5ea0edf45dc2512698c05db3036f9ed786b8d116e.jpg \
    data/images/cc6113211f59cb0a56048cfd465841d53683835c64ed7a718259b8a292b9ac4b.jpg \
    data/images/ce2876ccc495e0ff1697e91789618572ea3b2d5b44340f1e315125f83099549e.jpg \
    data/images/d0391417ec0fc2af5aee5d9bb12f1c91830e7553af598ef5034422cca10c52db.jpg \
    data/images/d50d8c297b467b0245ab6d4c888f53ebc38001b9c9aea3c476412e4952ebe426.jpg \
    data/images/d82271b7c478b34d3e1e50508a44fba5fcad51a5fd09d6dc87925cfbcbe862ea.jpg \
    data/images/d8edc97575430cfd8f5d5c16a396a253d6d80bafe0062f1086f22b7ab44db707.jpg \
    data/images/da1469b76c684573af380e0459c71cd18ba848b47c1d2030db461138064ee47d.jpg \
    data/images/daac0a1385647d7051a65317970ae255ad709fde354d491ca1684bbf791478b6.jpg \
    data/images/e129541edc944e19df863a264212dfbbc545c196ee78e510a79c492ca80ee342.jpg \
    data/images/e1d543d1cf25cd88aae2a27f2d1ea968952bc19e8e838e4c2a2856b1b0850ba0.jpg \
    data/images/e836ceea6a88ca40e2e7281ede3cddf14bc39e815cfea6e11542f6d544b1f8c5.jpg \
    data/images/e97f13d09d27cfb52cc55c60f2c9fbd262dc257f1127b823425add53b99878a2.jpg \
    data/images/ea3fb8e69b7f446061b91b9c7bb1844fdea97813c6cd79397d89ec67f80eba0a.jpg \
    data/images/ebbf6e3b722268d111d54b3d0d5b66feda784a901c3f49b0bb5da883e2a125c1.jpg \
    data/images/effabbbb0c82997f2bf7b0f5e480514f1853323c6e64db79f0bf448f84bd85aa.jpg \
    data/images/f5a8a77b91ab3b0087112ce611ca63dde466a0bffd03a2d222c1fa12ba174bb4.jpg \
    data/images/f6aed67f7d484642abd0a9d12cc4824a4b66e53553fe83abe646a5848cc139d6.jpg \
    data/images/f851018f59ecc8eee307c93004dc0a2e96cb5b5b245bae89295d12b6d2f2d6c1.jpg \
    data/images/f8947584d2fc1ac5b9b77f6b892d96c2ced7e68657a0ac0dfc0166c1cfd25bb5.jpg \
    data/images/fcc292a5c6273c19cc2f6beb76e9ba438502ae26d2e6293eb352d3eced1f8f3e.jpg \
    data/images/febaf9c66f88248b5d7a9ccaf23a56c0529d5b0df5e109eda85fa25041acfd13.jpg \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-hdpi/splash.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-ldpi/splash.png \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-mdpi/splash.png \
    data/images/86576546d6e2a31acd0d0a51d1eba0cd5269a72b756e64638b354ae4af17b35b.png \
    data/images/f78a4ad81e8d08f18828d5ffbf8d7ce73e60b7f63040889135f3ba60c3001957.png \
    media/Android/Icon

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

ICON = media/OSX.icns

ios {
    QMAKE_INFO_PLIST = Info.plist
    ios_icon.files += $$files($$PWD/media/iOS/LaunchImage*.png)
    ios_xcassets.files = $$files($$PWD/media/iOS/*.xcassets)
    QMAKE_ASSET_CATALOGS += $$files($$PWD/media/iOS/*.xcassets)  # There's a warning about this, but no documentation on how to use it yet.
    ios_icon.files += $$PWD/views/Launch_Screen.storyboard
    QMAKE_BUNDLE_DATA += ios_icon
    QMAKE_BUNDLE_DATA += ios_xcassets
    DEFINES += "IOS_SPECIFIC"
}

macx {
# For reloading of the stylesheet during development
    message("* Adding file system watcher")
    DEFINES += "ADD_FS_WATCHER"
}

!macx: {
#    These settings will remove qDebug statements and other statements that take too long for mobile processors
    message("* Using settings for Mobile.")
    DEFINES += "MOBILE_DEPLOYMENT"
}

android {
    message("* Using settings for Android.")
#   Need this next line?
#    ANDROID_ABIS = arm64-v8a armeabi-v7a x86_64 x86
    DEFINES += ANDROID_SPECIFIC
}

include(../android_openssl/openssl.pri)
