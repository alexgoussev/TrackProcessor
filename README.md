Как скомпилировать TrackProcessor?

1. Зарегистрироваться по ссылке https://www.embarcadero.com/ru/products/cbuilder/starter/free-download и получить на емайл регистрационный код для бесплатного Embarcadero C++Builder Starter Edition.

2. Скачать последнюю версию Embarcadero C++Builder Starter Edition 10.2 Tokyo по ссылке http://altd.embarcadero.com/download/radstudio/10.2/delphicbuilder10_2__1.iso (6.1 Гб)

3. Распаковать скачанный iso-файл и установить C++Builder Starter Edition запустив Setup.exe из поддиректории Install.

4. Запустить C++Builder и открыть проект TrackProcessor.cbproj


Примечания.

* Если при сборке проекта появляется ошибка компоновщика:
[ilink32 Warning] Warning: Error detected (LME200)
[ilink32 Error] Error: Unable to perform link
то скачайте со страницы http://cc.embarcadero.com/Item/30459 утилиту LAMarker и модифицируйте с её помощью файл компоновщика ilink32.exe (он находится в поддиректории bin установочной директории C++Builder), запустив утилиту из командной строки:
LAMarker.exe -M -Filink32.exe
Если не помогло, попробуйте советы из пунктов 2 и 3 сообщения по ссылке https://forums.embarcadero.com/message.jspa?messageID=867865#867865

* Для нормальной работы автоматического скачивания файлов SRTM при использовании команды меню "Set SRTM Altitude" скопируйте файлы libeay32.dll и ssleay32.dll из директории openssl в рабочую директорию программы.

* В процессе работы TrackProcessor создаст в рабочей директории файл с настройками "TrackProcessor.ini" две поддиректории - "hgt" и "cache".
