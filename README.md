<h1 align="center">
  <br>
  <img src="https://raw.githubusercontent.com/Alex217/GagBook/master/sailfish/harbour-gagbook.png" width="86px" height="86px" alt="GagBook">
  <br>
  <br>
</h1>

# GagBook

GagBook is an unofficial mobile app for the [9GAG](https://9gag.com) website, currently available
for SailfishOS, MeeGo Harmattan and Symbian^3.

This is a fork of the original GagBook app initially developed by [@dicksonleong](https://github.com/dicksonleong) and later maintained by [@llelectronics](https://github.com/llelectronics).
I intend to update this fork with bug fixes and new features.

GagBook gets its data from 9GAG by scraping the website, or using [InfiniGAG](https://github.com/k3min/infinigag)
API. This might be unethical but the app initially has been developed because there is no official 9GAG app for Sailfish OS, MeeGo Harmattan
and Symbian^3 and 9GAG also does not provide an official API for developers.

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
