<h1 align="center">
  <br>
  <img src="https://raw.githubusercontent.com/Alex217/GagBook/master/sailfish/harbour-gagbook.png" width="86px" height="86px" alt="GagBook">
  <br>
  <br>
  GagBook
  <br>
  <br>
</h1>

GagBook is an unofficial mobile app for the [9GAG](https://9gag.com) website, currently available
for SailfishOS, MeeGo Harmattan and Symbian^3.

This is a fork of the original GagBook app initially developed by [@dicksonleong](https://github.com/dicksonleong) and later maintained by [@llelectronics](https://github.com/llelectronics).
I intend to update this fork with bug fixes and new features.

GagBook gets its data from the undisclosed official 9GAG API.
The webscraping method using QtWebKitWidgets is deprecated after 9GAG changed its website to a JS rendered page back in December 2017.

## Features

- Simple, smooth and native UI
- View posts in different sections, e.g. hot, trending, fresh, etc.
- Full screen view of images with pinch-to-zoom
- View comments for each post
- Save images
- Login (with email only) and voting
- Share posts using Harmattan's integrated sharing
- Video support (atm only SailfishOS supports MP4 files on 9GAG)

## Limitation / Known Issues / To Do

- **GagBook may fail to work anytime when 9GAG updates their website** (if this happens, you will see "error: Unable to parse response")
- Image upload is not supported
- Large image will be stretched down due to hardware limitation

## Contacts

- **Me**: Alex217@t-online.de
- **llelectronics**: leszek.lesner@web.de
- **dicksonleong**: dicksonleong8@gmail.com

## License

GagBook is licensed under the Simplified BSD License, see the [LICENSE.md](LICENSE.md) file for more details.

GagBook uses the [Qt-Json library](https://github.com/ereilin/qt-json), which is also licensed
under the Simplified BSD License, see the file "qt-json/LICENSE" for more info.

[VideoComponent.qml](sailfish/qml/VideoComponent.qml) was implemented by [@llelectronics](https://github.com/llelectronics) and is licensed under GPLv3.

## Credits

Thanks to [@and3rson](https://github.com/and3rson) for his work on [nineapi](https://github.com/and3rson/nineapi).
