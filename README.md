![](Photos/IMG5)
![only  board](Photos/IMG1)

---------Simple game station V0.1 beta------------
ENG
 !!Archived!!

Framework: Arduino
List of Components:
1x ESP32 WROOM board
10x buttons
1x switch
1x TP4056 module
1x TFT SPI 1.8" screen
1x ME6211 stabilizer
1x 18650 battery
1x 18650 holder
1x buzzer

Notification:
It's V0.1 Beta, I only wrote the code skeleton, without proper optimization. In the future, I will optimize it and add more functionality.
Now my game station can successfully start CHIP-8 and S-CHIP games (ANT, Tetris, Space Invaders were started successfully).

Development Roadmap:
ui and sd card file browser optimization.
Dynamic directory scanning for microsd files.
Battery voltage monitoring via ADC.
Framebuffer rendering optimization (dirty rectangle updates).

RUS
11 Архивный проект !!

фреймворк: Arduino
Список компонентов:
1x плата ESP32 WROOM
10x кнопок
1x переключатель
1x модуль TP4056
1x TFT SPI дисплей 1.8"
1x стабилизатор ME6211
1x аккумулятор 18650
1x держатель для аккумулятора 18650
1x зуммер

Уведомление:
Это версия V0.1 Beta, я написал только костяк кода без должной оптимизации. В будущем я ее оптимизирую и добавлю функционала.
Теперь моя игровая станция может успешно запускать игры для CHIP-8 и S-CHIP (успешно запущены ANT, Tetris, Space Invaders).

План разработки:

Оптимизация пользовательского интерфейса и файлового менеджера сд карты.

Динамическое сканирование директорий для файлов microsd.

Мониторинг напряжения аккумулятора через АЦП.

Оптимизация рендеринга буфера кадра (обновление измененных прямоугольных областей).
